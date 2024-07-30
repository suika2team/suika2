/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

/*
 * Packager
 */

#include "opennovel.h"
#include "package.h"

/* Obfuscation Key */
#include "key.h"

#ifdef OPENNOVEL_TARGET_WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

/* Max path size */
#define PATH_SIZE		(256)

/* Size of file count which is written at top of an archive */
#define FILE_COUNT_BYTES	(8)

/* Size of file entry */
#define ENTRY_BYTES		(256 + 8 + 8)

/* Directory names */
const char *dir_names[] = {
	"story",
	"image",
	"sound",
	"font",
	"script",
	"video"
};

/* Size of directory names */
#define DIR_COUNT	((int)(sizeof(dir_names) / sizeof(const char *)))

/* File entry */
struct file_entry entry[FILE_ENTRY_SIZE];

/* File count */
static uint64_t file_count;

/* Current processing file's offset in archive file */
static uint64_t offset;

/* Next random number. */
static uint64_t next_random;

/* forward declaration */
static bool get_file_names(const char *base_dir, const char *dir);
static bool get_file_sizes(const char *base_dir);
static bool write_archive_file(const char *base_dir);
static bool write_file_entries(FILE *fp);
static bool write_file_bodies(const char *base_dir, FILE *fp);
static void set_random_seed(uint64_t index);
static char get_next_random(void);

#ifdef OPENNOVEL_TARGET_WIN32
const wchar_t *conv_utf8_to_utf16(const char *utf8_message);
const char *conv_utf16_to_utf8(const wchar_t *utf16_message);
#endif

/*
 * Create package.
 */
bool create_package(const char *base_dir)
{
	int i;

	file_count = 0;
	offset = 0;
	next_random = 0;

	/* Get list of files. */
	for (i = 0; i < DIR_COUNT; i++) {
		if (!get_file_names(base_dir, dir_names[i])) {
			printf("Failed to open folder %s.\n", dir_names[i]);
			return false;
		}
	}
	snprintf(entry[file_count++].name, FILE_NAME_SIZE, "project.txt");

	/* Get all file sizes and decide all offsets in archive. */
	if (!get_file_sizes(base_dir))
		return false;

	/* Write archive file. */
	if (!write_archive_file(base_dir))
		return false;

	return true;
}

#if defined(OPENNOVEL_TARGET_WIN32)
/*
 * For Windows:
 */

static bool get_file_names_recursive(const wchar_t *base_dir, const wchar_t *dir, int depth);

/* Get file list in directory (for Windows) */
static bool get_file_names(const char *base_dir, const char *dir)
{
	wchar_t base_buf[PATH_SIZE];
	wchar_t dir_buf[PATH_SIZE];

	wcsncpy(base_buf, conv_utf8_to_utf16(base_dir), PATH_SIZE - 1);
	base_buf[PATH_SIZE - 1] = L'\0';

	wcsncpy(dir_buf, conv_utf8_to_utf16(dir), PATH_SIZE - 1);
	dir_buf[PATH_SIZE - 1] = L'\0';

	return get_file_names_recursive(base_buf, dir_buf, 0);
}

static bool get_file_names_recursive(const wchar_t *base_dir, const wchar_t *dir, int depth)
{
	HANDLE hFind;
	WIN32_FIND_DATAW wfd;
	wchar_t curdir[PATH_SIZE];
	wchar_t findpath[PATH_SIZE];
	char u8dir[PATH_SIZE];
	char *separator;

	/* Make path. */
	if (wcscmp(base_dir, L"") == 0) {
		/* Make Utf-16 current directory path. */
		_snwprintf(curdir, PATH_SIZE, L"%s", dir);

		/* Make Utf-8 current directory path. */
		snprintf(u8dir, PATH_SIZE, "%s", conv_utf16_to_utf8(dir));
	} else {
		/* Make Utf-16 current directory path. */
		_snwprintf(curdir, PATH_SIZE, L"%s\\%s", base_dir, dir);

		/* Make Utf-8 current directory path. */
		snprintf(u8dir, PATH_SIZE, "%s\\", conv_utf16_to_utf8(base_dir));
		strncat(u8dir, conv_utf16_to_utf8(dir), PATH_SIZE - 1);
		u8dir[PATH_SIZE - 1] = '\0';
	}
	/* Replace '\\' with '/' in u8dir. */
	while ((separator = strstr(u8dir, "\\")) != NULL)
		*separator = '/';

	/* Get directory content. */
	_snwprintf(findpath, PATH_SIZE, L"%s\\*.*", curdir);
	findpath[PATH_SIZE - 1] = L'\0';
	hFind = FindFirstFileW(findpath, &wfd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		log_dir_not_found(u8dir);
		return false;
	}
	do
	{
		if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif
			snprintf(entry[file_count].name, FILE_NAME_SIZE, "%s/%s", u8dir, conv_utf16_to_utf8(wfd.cFileName));
			printf("%s\n", entry[file_count].name);
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic pop
#endif
			file_count++;
		}
		else if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			wfd.cFileName[0] != L'.')
		{
			get_file_names_recursive(curdir, wfd.cFileName, depth + 1);
		}
	} while(FindNextFileW(hFind, &wfd));

	FindClose(hFind);
	return true;
}

#else

/*
 * For macOS and Linux:
 */

static bool get_file_names_recursive(const char *base_dir, const char *dir, int depth);

/* Get directory file list (for Mac and Linux) */
static bool get_file_names(const char *base_dir, const char *dir)
{
	return get_file_names_recursive(base_dir, dir, 0);
}

static bool get_file_names_recursive(const char *base_dir, const char *dir, int depth)
{
	char new_path[1024];
	struct dirent **names;
	int i, count;
	bool succeeded;

	/* Make a path. */
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif
	if (strcmp(base_dir, "") == 0)
		snprintf(new_path, sizeof(new_path), "%s", dir);
	else
		snprintf(new_path, sizeof(new_path), "%s/%s", base_dir, dir);
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic pop
#endif

	/* Get directory content. */
	count = scandir(new_path, &names, NULL, alphasort);
	if (count < 0 && depth == 0) {
		log_dir_not_found(dir);
		return false;
	}
	succeeded = true;
	for (i = 0; i < count; i++) {
		if (names[i]->d_name[0] == '.') {
			/* Ignore . and .. (also .*)*/
			continue;
		}
		if (count >= FILE_ENTRY_SIZE) {
			log_too_many_files();
			succeeded = false;
			break;
		}
		if (names[i]->d_type == DT_DIR) {
			if (!get_file_names_recursive(new_path, names[i]->d_name, depth + 1)) {
				succeeded = false;
				break;
			}
		} else {
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif
			snprintf(entry[file_count].name, FILE_NAME_SIZE, "%s/%s", new_path, names[i]->d_name);
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic pop
#endif
			printf("%s\n", entry[file_count].name);
			file_count++;
		}
	}
	for (i = 0; i < count; i++)
		free(names[i]);
	free(names);
	return succeeded;
}
#endif

/* Get sizes of each files. */
static bool get_file_sizes(const char *base_dir)
{
	FILE *fp;
	uint64_t i;

	UNUSED_PARAMETER(base_dir);

	/* Get each file size, and calc offsets. */
	offset = FILE_COUNT_BYTES + ENTRY_BYTES * file_count;
	for (i = 0; i < file_count; i++) {
		/*
		 * Make a path and open the file.
		 */
#if defined(OPENNOVEL_TARGET_WIN32)
		/* Make a path on Windows. */
		char *path = strdup(entry[i].name);
		char *slash;
		if (path == NULL) {
			log_memory();
			return false;
		}
		slash = strchr(path, '/');
		while (slash != NULL) {
			*slash = '\\';
			slash = strchr(slash + 1, '/');
		}
		fp = fopen(path, "rb");
		free(path);
#else
		/* Make path on Mac/Linux. */
		char abspath[1024];
		if (strcmp(base_dir, "") == 0)
			snprintf(abspath, sizeof(abspath), "%s", entry[i].name);
		else
			snprintf(abspath, sizeof(abspath), "%s/%s", base_dir, entry[i].name);
		fp = fopen(abspath, "r");
#endif

		/* Check the fp. */
		if (fp == NULL) {
			log_file_open(entry[i].name);
			return false;
		}

		/* Get the file size. */
		fseek(fp, 0, SEEK_END);
		entry[i].size = (uint64_t)ftell(fp);
		entry[i].offset = offset;
		fclose(fp);

		/* Increment the offset. */
		offset += entry[i].size;
	}
	return true;
}

/* Write archive file. */
static bool write_archive_file(const char *base_dir)
{
	FILE *fp;
	bool success;

#ifdef OPENNOVEL_TARGET_WIN32
	fp = fopen(PACKAGE_FILE, "wb");
	UNUSED_PARAMETER(base_dir);
#else
	char abspath[1024];
	if (strcmp(base_dir, "") == 0)
		snprintf(abspath, sizeof(abspath), "%s", PACKAGE_FILE);
	else
		snprintf(abspath, sizeof(abspath), "%s/%s", base_dir, PACKAGE_FILE);
	fp = fopen(abspath, "w");
#endif
	if (fp == NULL) {
		log_file_open(PACKAGE_FILE);
		return false;
	}

	success = false;
	do {
		if (fwrite(&file_count, sizeof(uint64_t), 1, fp) < 1)
			break;
		if (!write_file_entries(fp))
			break;
		if (!write_file_bodies(base_dir, fp))
			break;
		fclose(fp);
		success = true;
	} while (0);

	if (!success)
		log_file_write(PACKAGE_FILE);

	return true;
}

/* Write file entries. */
static bool write_file_entries(FILE *fp)
{
	char xor[FILE_NAME_SIZE];
	uint64_t i;
	int j;

	for (i = 0; i < file_count; i++) {
		set_random_seed(i);
		for (j = 0; j < FILE_NAME_SIZE; j++)
			xor[j] = entry[i].name[j] ^ get_next_random();

		if (fwrite(xor, FILE_NAME_SIZE, 1, fp) < 1)
			return false;
		if (fwrite(&entry[i].size, sizeof(uint64_t), 1, fp) < 1)
			return false;
		if (fwrite(&entry[i].offset, sizeof(uint64_t), 1, fp) < 1)
			return false;
	}
	return true;
}

/* Write file bodies. */
static bool write_file_bodies(const char *base_dir, FILE *fp)
{
	char buf[8192];
	FILE *fpin;
	uint64_t i;
	size_t len, obf;

	for (i = 0; i < file_count; i++) {
#ifdef OPENNOVEL_TARGET_WIN32
		char *path = strdup(entry[i].name);
		char *slash;
		if (path == NULL) {
			log_memory();
			return false;
		}
		slash = strchr(path, '/');
		while (slash != NULL) {
			*slash = '\\';
			slash = strchr(slash + 1, '/');
		}
		fpin = fopen(path, "rb");
		UNUSED_PARAMETER(base_dir);
#else
		char abspath[1024];
		if (strcmp(base_dir, "") == 0)
			snprintf(abspath, sizeof(abspath), "%s", entry[i].name);
		else
			snprintf(abspath, sizeof(abspath), "%s/%s", base_dir,
				 entry[i].name);
		fpin = fopen(abspath, "r");
#endif
		if (fpin == NULL) {
			log_file_open(entry[i].name);
			return false;
		}
		set_random_seed(i);
		do  {
			len = fread(buf, 1, sizeof(buf), fpin);
			if (len > 0) {
				for (obf = 0; obf < len; obf++)
					buf[obf] ^= get_next_random();
				if (fwrite(buf, len, 1, fp) < 1) {
					log_file_write(entry[i].name);
					return false;
				}
			}
		} while (len == sizeof(buf));
#ifdef _WIN32
		free(path);
#endif
		fclose(fpin);
	}
	return true;
}

/* Set random seed. */
static void set_random_seed(uint64_t index)
{
	uint64_t i, lsb;

	next_random = OBFUSCATION_KEY;
	for (i = 0; i < index; i++) {
		next_random ^= 0xafcb8f2ff4fff33fULL;
		lsb = next_random >> 63;
		next_random = (next_random << 1) | lsb;
	}
}

/* Get next random number. */
char get_next_random(void)
{
	char ret;

	ret = (char)next_random;

	next_random = (((OBFUSCATION_KEY & 0xff00) * next_random +
			(OBFUSCATION_KEY & 0xff)) % OBFUSCATION_KEY) ^
		      0xfcbfaff8f2f4f3f0;

	return ret;
}

/*
 * Standalone main
 */
#ifdef USE_STANDALONE_PACK

int main(int argc, char *argv[])
{
	printf("Hello, this is a packager!\n");

	/* Create a package. */
	if (!create_package("")) {
		printf("Failed.\n");
		return 1;
	}

	printf("Suceeded.\n");

#ifdef _WIN32
	getchar();
#endif

	return 0;
}

#ifdef _WIN32

#define CONV_MESSAGE_SIZE 65536

static wchar_t wszMessage[CONV_MESSAGE_SIZE];
static char szMessage[CONV_MESSAGE_SIZE];

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpszCmd,
	int nCmdShow)
{
	return main(1, NULL);
}

const wchar_t *conv_utf8_to_utf16(const char *utf8_message)
{
	assert(utf8_message != NULL);
	MultiByteToWideChar(CP_UTF8, 0, utf8_message, -1, wszMessage,
			    CONV_MESSAGE_SIZE - 1);
	return wszMessage;
}

const char *conv_utf16_to_utf8(const wchar_t *utf16_message)
{
	assert(utf16_message != NULL);
	WideCharToMultiByte(CP_UTF8, 0, utf16_message, -1, szMessage,
			    CONV_MESSAGE_SIZE - 1, NULL, NULL);
	return szMessage;
}

#endif /* _WIN32 */

/* Stub for platform.c */
bool log_error(const char *s, ...) { UNUSED_PARAMETER(s); return true; }
bool log_warn(const char *s, ...) { UNUSED_PARAMETER(s); return true; }
bool log_info(const char *s, ...) { UNUSED_PARAMETER(s); return true; }
void log_dir_many_files(void) { log_error("Too many files.\n"); }
const char *get_system_locale(void) { return "other"; }
char *make_valid_path(const char *dir, const char *fname) { return strdup(""); }

/* Stub for script.c */
const char *get_script_file_name(void) { return ""; }
int get_line_num(void) { return 0; }
const char *get_line_string(void) { return ""; }
int get_command_index(void) { return 0; }
void translate_failed_command_to_message(int index) { }

/* Stub for main.c */
void dbg_set_error_state(void) { }
int dbg_get_parse_error_count(void) { return 0; }
void translate_command_to_message_for_runtime_error(int index) { UNUSED_PARAMETER(index); }

#endif /* USE_STANDALONE_PACK */
