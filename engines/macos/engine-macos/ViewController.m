/* -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; -*- */

/*
 * OpenNovel
 * Copyright (C) 2024, The Authors. All rights reserved.
 */

#import "ViewController.h"
#import "GameView.h"
#import "GameRenderer.h"

// Base
#import "opennovel.h"

// Standard C
#import <wchar.h>

// HAL
#import "aunit.h"

static ViewController *theViewController;
static FILE *openLog(void);
static void show_log(void);

static NSString *logFilePath;

@interface ViewController ()
@end

@implementation ViewController
{
    // The GameView (a subclass of MTKView)
    GameView *_view;

    // The GameRender (common for AppKit and UIKit)
    GameRenderer *_renderer;
    
    // The screen information
    float _screenScale;
    NSSize _screenSize;
    NSPoint _screenOffset;
    
    // The view frame (saved before entering a full screen mode)
    NSRect _savedViewFrame;
    
    // The temporary window frame size (for resizing)
    NSSize _resizeFrame;
    
    // The full screen status
    BOOL _isFullScreen;
    
    // The control key status
    BOOL _isControlPressed;
    
    // The video player objects and status
    AVPlayer *_avPlayer;
    AVPlayerLayer *_avPlayerLayer;
    BOOL _isVideoPlaying;
}

//
// Called when the view is loaded.
//
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Initialize the engine.
    if(!init_file())
        exit(1);
    if(!init_conf())
        exit(1);
    if(!init_aunit())
        exit(1);
    if(!on_event_init())
        exit(1);
    
    // Create an MTKView.
    _view = (GameView *)self.view;
    _view.enableSetNeedsDisplay = YES;
    _view.device = MTLCreateSystemDefaultDevice();
    _view.clearColor = MTLClearColorMake(0.0, 0, 0, 1.0);
    _renderer = [[GameRenderer alloc] initWithMetalKitView:_view andController:self];
    if(!_renderer) {
        NSLog(@"Renderer initialization failed");
        return;
    }
    [_renderer mtkView:_view drawableSizeWillChange:_view.drawableSize];
    _view.delegate = _renderer;
    [self updateViewport:_view.frame.size];
    
    // Setup a rendering timer.
    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
                                     target:self
                                   selector:@selector(timerFired:)
                                   userInfo:nil
                                    repeats:YES];
}

//
// Called when the view is layouted.
//
- (void)viewDidLayout {
    [super viewDidLayout];
    
    self.view.window.delegate = self;
}

//
// Called when the view appeared.
//
- (void)viewDidAppear {
    self.view.window.delegate = self;
    
    // Set the window position and size.
    NSRect screenRect = [[NSScreen mainScreen] visibleFrame];
    NSRect contentRect = NSMakeRect(screenRect.origin.x + (screenRect.size.width - conf_game_width) / 2,
                                    screenRect.origin.y + (screenRect.size.height - conf_game_height) / 2,
                                    conf_game_width,
                                    conf_game_height);
    NSRect windowRect = [self.view.window frameRectForContentRect:contentRect];
    [self.view.window setFrame:windowRect display:TRUE];
    
    // Enable the window maximization.
    [self.view.window setCollectionBehavior:[self.view.window collectionBehavior] | NSWindowCollectionBehaviorFullScreenPrimary];
    
    // Set the window title.
    [self.view.window setTitle:[[NSString alloc] initWithUTF8String:conf_game_title]];
    
    // Accept keyboard and mouse inputs.
    [self.view.window makeKeyAndOrderFront:nil];
    [self.view.window setAcceptsMouseMovedEvents:YES];
    [self.view.window makeFirstResponder:self];
    
    // Set the app name in the main menu.
    [self setAppName];

    // Recalculate the view size.
    [self updateViewport:_view.frame.size];
}

//
// A helper to set the app name.
//
- (void)setAppName {
    NSMenu *menu = [[[NSApp mainMenu] itemAtIndex:0] submenu];
    [menu setTitle:[[NSString alloc] initWithUTF8String:conf_game_title]];
}

//
// Called every frame.
//
- (void)timerFired:(NSTimer *)timer {
    // 
    [_view setNeedsDisplay:TRUE];
}

//
// Called when the window close button is pressed.
//
- (BOOL)windowShouldClose:(id)sender {
    @autoreleasepool {
        bool isEnglish = !compare_locale("ja");

        NSAlert *alert = [[NSAlert alloc] init];
        [alert addButtonWithTitle:isEnglish ? @"Yes" : @"はい"];
        [alert addButtonWithTitle:isEnglish ? @"No" : @"いいえ"];
        [alert setMessageText:isEnglish ? @"Are you sure you want to exit?" : @"終了しますか？"];
        [alert setAlertStyle:NSAlertStyleWarning];
        if ([alert runModal] == NSAlertFirstButtonReturn)
            return YES;
        else
            return NO;
    }
}

//
// Called when the window is going to be closed.
//
- (void)windowWillClose:(NSNotification *)notification {
    // Save.
    save_global_data();
    save_seen();

    // Exit the event loop.
    [NSApp stop:nil];

    // Magic: Post an empty event and make sure to exit the main loop.
    [NSApp postEvent:[NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                        location:NSMakePoint(0, 0)
                                   modifierFlags:0
                                       timestamp:0
                                    windowNumber:0
                                         context:nil
                                         subtype:0
                                           data1:0
                                           data2:0]
             atStart:YES];
}

//
// Called when the Quit menu item is pressed.
//
- (IBAction)onQuit:(id)sender {
    if ([self windowShouldClose:sender])
        [NSApp stop:nil];
}

//
// Called before entering the full screen mode.
//
- (NSSize)window:(NSWindow *)window willUseFullScreenContentSize:(NSSize)proposedSize {
    // Calculate the viewport size.
    [self updateViewport:proposedSize];
    
    // Set the frame of the video player.
    if (_avPlayerLayer != nil)
        [_avPlayerLayer setFrame:NSMakeRect(_screenOffset.x, _screenOffset.y, _screenSize.width, _screenSize.height)];
    
    // Returns the screen size.
    return proposedSize;
}

//
// Called when entering the full screen mode.
//
- (void)windowWillEnterFullScreen:(NSNotification *)notification {
    // Set the flag.
    _isFullScreen = YES;
    
    // Save the window frame.
    _savedViewFrame = self.view.frame;

    // MAGIC: set the app name again to avoid the reset of the app name on the menu bar.
    [self setAppName];
}

//
// Called before returning to the windowed mode.
//
- (void)windowWillExitFullScreen:(NSNotification *)notification {
    // Set the flag.
    _isFullScreen = NO;
    
    // Reset the video player size.
    if(_avPlayerLayer != nil)
        [_avPlayerLayer setFrame:NSMakeRect(0, 0, _savedViewFrame.size.width, _savedViewFrame.size.height)];
    
    // Recalculate the viewport size.
    [self updateViewport:_savedViewFrame.size];
}

//
// Called when entered the full screen mode.
//
- (void)windowDidEnterFullScreen:(NSNotification *)notification {
    [self setAppName];
}

//
// Called when returned to the windowed mode.
//
- (void)windowDidLeaveFullScreen:(NSNotification *)notification {
    [self setAppName];
}

//
// Called when the window size is changing, including a window resize by mouse device.
//
- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize {
    // Get a new view rect from a window rect.
    NSRect contentRect = [self.view.window contentRectForFrameRect:NSMakeRect(0, 0, frameSize.width, frameSize.height)];

    // Calculate the content rect.
    float aspect = (float)conf_game_height / (float)conf_game_width;
    if (self.view.window.frame.size.width != frameSize.width)
        contentRect.size.height = contentRect.size.width * aspect;
    else
        contentRect.size.width = contentRect.size.height / aspect;

    // Save the contentRect.size for windowDidResize.
    _resizeFrame = contentRect.size;

    // Return the window size.
    NSRect windowRect = [self.view.window frameRectForContentRect:NSMakeRect(0, 0, contentRect.size.width, contentRect.size.height)];
    return windowRect.size;
}

//
// Called after a window resize.
//
- (void)windowDidResize:(NSNotification *)notification {
    // Recalculate the viewport size.
    [self updateViewport:_resizeFrame];
}

//
// Calculate the viewport size.
//
- (void)updateViewport:(NSSize)newViewSize {
    // For when the view is not initialized.
    if (newViewSize.width == 0 || newViewSize.height == 0) {
        _screenScale = 1.0f;
        _screenSize = NSMakeSize(conf_game_width, conf_game_height);
        _screenOffset.x = 0;
        _screenOffset.y = 0;
        return;
    }
    
    // Get the game aspect ratio.
    float aspect = (float)conf_game_height / (float)conf_game_width;
    
    // 1. Calculate by width-first.
    _screenSize.width = newViewSize.width;
    _screenSize.height = _screenSize.width * aspect;
    _screenScale = (float)conf_game_width / _screenSize.width;
    
    // 2. If the height is not enough, use height-first.
    if(_screenSize.height > newViewSize.height) {
        _screenSize.height = newViewSize.height;
        _screenSize.width = _screenSize.height / aspect;
        _screenScale = (float)conf_game_height / _screenSize.height;
    }

    // Multiply the scaling factor.
    _screenSize.width *= _view.layer.contentsScale;
    _screenSize.height *= _view.layer.contentsScale;
    newViewSize.width *= _view.layer.contentsScale;
    newViewSize.height *= _view.layer.contentsScale;

    // Calculate the offset.
    _screenOffset.x = (newViewSize.width - _screenSize.width) / 2.0f;
    _screenOffset.y = (newViewSize.height - _screenSize.height) / 2.0f;
}

//
// Called when a mouse is moved.
//
- (void)mouseMoved:(NSEvent *)event {
    NSPoint point = [event locationInWindow];
    int x = (int)((point.x - self.screenOffset.x) * _screenScale);
    int y = (int)((point.y - self.screenOffset.y) * _screenScale);
    on_event_mouse_move(x, conf_game_height - y);
}

//
// Called when a mouse is moved by a drag.
//
- (void)mouseDragged:(NSEvent *)event {
    NSPoint point = [event locationInWindow];
    int x = (int)((point.x - self.screenOffset.x) * _screenScale);
    int y = (int)((point.y - self.screenOffset.y) * _screenScale);
    on_event_mouse_move(x, conf_game_height - y);
}

//
// Called when a modifier key is pressed or released.
//
- (void)flagsChanged:(NSEvent *)theEvent {
    // Get the Control key state.
    BOOL bit = ([theEvent modifierFlags] & NSEventModifierFlagControl) == NSEventModifierFlagControl;
    
    // Notify when the Control key state is changed.
    if (!_isControlPressed && bit) {
        _isControlPressed = YES;
        on_event_key_press(KEY_CONTROL);
    } else if (_isControlPressed && !bit) {
        _isControlPressed = NO;
        on_event_key_release(KEY_CONTROL);
    }
}

//
// Called when a keyboard is pressed.
//
- (void)keyDown:(NSEvent *)theEvent {
    if ([theEvent isARepeat])
        return;
    
    int kc = [self convertKeyCode:[theEvent keyCode]];
    if (kc != -1)
        on_event_key_press(kc);
}

//
// Called when a keyboard is released.
//
- (void)keyUp:(NSEvent *)theEvent {
    int kc = [self convertKeyCode:[theEvent keyCode]];
    if (kc != -1)
        on_event_key_release(kc);
}

//
// A helper to convert a keycode.
//
- (int)convertKeyCode:(int)keyCode {
    switch(keyCode) {
        case 49: return KEY_SPACE;
        case 36: return KEY_RETURN;
        case 123: return KEY_LEFT;
        case 124: return KEY_RIGHT;
        case 125: return KEY_DOWN;
        case 126: return KEY_UP;
        case 53: return KEY_ESCAPE;
    }
    return -1;
}

//
// GameViewControllerProtocol
//

//
// Set the window title.
//
- (void)setWindowTitle:(NSString *)name {
    [self.view.window setTitle:name];
}

//
// Get the view scale.
//
- (float)screenScale {
    return _screenScale;
}

//
// Get the screen offset.
//
- (NSPoint)screenOffset {
    return _screenOffset;
}

//
// Get the screen size.
//
- (NSSize)screenSize {
    return _screenSize;
}

//
// Convert a screen point to window point.
//
- (NSPoint)windowPointToScreenPoint:(NSPoint)windowPoint {
    float retinaScale = _view.layer.contentsScale;
    
    int x = (int)(windowPoint.x - (_screenOffset.x / retinaScale)) * _screenScale;
    int y = (int)(windowPoint.y - (_screenOffset.y / retinaScale)) * _screenScale;
    
    return NSMakePoint(x, conf_game_height - y);
}

//
// Returns whether we are in the full screen mode or not.
//
- (BOOL)isFullScreen {
    return _isFullScreen;
}

//
// Enter the full screen mode.
//
- (void)enterFullScreen {
    if (!_isFullScreen) {
        [self.view.window toggleFullScreen:self.view];
        [self setAppName];
        _isFullScreen = YES;
    }
}

//
// Leave the full screen mode.
//
- (void)leaveFullScreen {
    if (_isFullScreen) {
        [self.view.window toggleFullScreen:self.view];
        [self setAppName];
        _isFullScreen = NO;
    }
}

//
// Returns whether we are playing a video or not.
//
- (BOOL)isVideoPlaying {
    return _isVideoPlaying;
}

//
// Play a video.
//
- (void)playVideoWithPath:(NSString *)path skippable:(BOOL)isSkippable {
    // プレーヤーを作成する
    NSURL *url = [NSURL URLWithString:[@"file://" stringByAppendingString:path]];
    AVPlayerItem *playerItem = [[AVPlayerItem alloc] initWithURL:url];
    _avPlayer = [[AVPlayer alloc] initWithPlayerItem:playerItem];
    
    // プレーヤーのレイヤーを作成する
    [self.view setWantsLayer:YES];
    _avPlayerLayer = [AVPlayerLayer playerLayerWithPlayer:_avPlayer];
    [_avPlayerLayer setFrame:theViewController.view.bounds];
    [self.view.layer addSublayer:_avPlayerLayer];
    
    // 再生終了の通知を送るようにする
    [NSNotificationCenter.defaultCenter addObserver:self
                                           selector:@selector(onPlayEnd:)
                                               name:AVPlayerItemDidPlayToEndTimeNotification
                                             object:playerItem];
    
    // 再生を開始する
    [_avPlayer play];
    
    _isVideoPlaying = YES;
}

//
// Called when a video playback is finished.
//
- (void)onPlayEnd:(NSNotification *)notification {
    [_avPlayer replaceCurrentItemWithPlayerItem:nil];
    _isVideoPlaying = NO;
}

//
// Stop a video playback.
//
- (void)stopVideo {
    if (_avPlayer != nil) {
        [_avPlayer replaceCurrentItemWithPlayerItem:nil];
        _isVideoPlaying = NO;
        _avPlayer = nil;
        _avPlayerLayer = nil;
    }
}

@end

//
// HAL
//

//
// Make a save directory.
//
bool make_sav_dir(void)
{
    @autoreleasepool {
        if (conf_release) {
            // If we are in the release mode, use home folder.
            NSString *path = NSHomeDirectory();
            path = [path stringByAppendingString:@"/Library/Application Support/"];
            path = [path stringByAppendingString:[[NSString alloc] initWithUTF8String:conf_game_title]];
            path = [path stringByAppendingString:@"/"];
            path = [path stringByAppendingString:[[NSString alloc] initWithUTF8String:SAVE_DIR]];
            [[NSFileManager defaultManager] createDirectoryAtPath:path
                                      withIntermediateDirectories:YES
                                                       attributes:nil
                                                            error:NULL];
            return true;
        }

        // If we are in the normal mode, use the folder where the app exists.
        NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
        NSString *basePath = [bundlePath stringByDeletingLastPathComponent];
        NSString *savePath = [NSString stringWithFormat:@"%@/%s", basePath, SAVE_DIR];
        NSError *error;
        [[NSFileManager defaultManager] createDirectoryAtPath:savePath
                                  withIntermediateDirectories:NO
                                                   attributes:nil
                                                        error:&error];
        return true;
    }
}

//
// Get a real path for a file.
//
char *make_valid_path(const char *dir, const char *fname)
{
    @autoreleasepool {
        // For package.pak, find the bundle first.
        if (dir == NULL && fname != NULL && strcmp(fname, PACKAGE_FILE) == 0) {
            NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
            NSString *filePath = [NSString stringWithFormat:@"%@/Contents/Resources/package.pak", bundlePath];
            if ([[NSFileManager defaultManager] fileExistsAtPath:filePath]) {
                // Get the size of package.pak.
                NSError *attributesError = nil;
                NSDictionary *fileAttributes = [[NSFileManager defaultManager] attributesOfItemAtPath:filePath error:&attributesError];
                NSNumber *fileSizeNumber = [fileAttributes objectForKey:NSFileSize];
                long long fileSize = [fileSizeNumber longLongValue];

                // If package.pak is not a dummy file, use it.
                if (fileSize > 100) {
                    const char *cstr = [filePath UTF8String];
                    char *ret = strdup(cstr);
                    if (ret == NULL) {
                        log_memory();
                        return NULL;
                    }
                    return ret;
                }
            }
        }

        // For an mp4 file, find the bundle first.
        if (dir != NULL && strcmp(dir, MOV_DIR) == 0 && fname != NULL) {
            NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
            NSString *filePath = [NSString stringWithFormat:@"%@/Contents/Resources/%s/%s", bundlePath, MOV_DIR, fname];
            if ([[NSFileManager defaultManager] fileExistsAtPath:filePath]) {
                const char *cstr = [filePath UTF8String];
                char *ret = strdup(cstr);
                if (ret == NULL) {
                    log_memory();
                    return NULL;
                }
                return ret;
            }
        }

        // If we are in the release mode and are going to open a save file, use the home folder.
        if (conf_release && dir != NULL && strcmp(dir, SAVE_DIR) == 0) {
            assert(fname != NULL);
            NSString *path = NSHomeDirectory();
            path = [path stringByAppendingString:@"/Library/Application Support/"];
            path = [path stringByAppendingString:[[NSString alloc] initWithUTF8String:conf_game_title]];
            path = [path stringByAppendingString:@"/"];
            path = [path stringByAppendingString:[[NSString alloc] initWithUTF8String:dir]];
            path = [path stringByAppendingString:@"/"];
            path = [path stringByAppendingString:[[NSString alloc] initWithUTF8String:fname]];
            char *ret = strdup([path UTF8String]);
            if (ret == NULL) {
                log_memory();
                return NULL;
            }
            return ret;
        }

        // Other files.
        NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
        NSString *basePath = [bundlePath stringByDeletingLastPathComponent];
        NSString *filePath;
        if (dir != NULL) {
            if (fname != NULL)
                filePath = [NSString stringWithFormat:@"%@/%s/%s", basePath, dir, fname];
            else
                filePath = [NSString stringWithFormat:@"%@/%s", basePath, dir];
        } else {
            if (fname != NULL)
                filePath = [NSString stringWithFormat:@"%@/%s", basePath, fname];
            else
                filePath = basePath;
        }
        const char *cstr = [filePath UTF8String];
        char *ret = strdup(cstr);
        if (ret == NULL) {
            log_memory();
            return NULL;
        }
        return ret;
    }
}

//
// Show an INFO log.
//
bool log_info(const char *s, ...)
{
    char buf[1024];
    va_list ap;
    
    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    // Write to the log file.
    FILE *fp = openLog();
    if (fp != NULL) {
        fprintf(stderr, "%s", buf);
        fprintf(fp, "%s", buf);
        fflush(fp);
    }

    // Open the log when exit.
    atexit(show_log);

    return true;
}

//
// Show a WARN log.
//
bool log_warn(const char *s, ...)
{
    char buf[1024];
    va_list ap;

    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    // Write to the log file.
    FILE *fp = openLog();
    if (fp != NULL) {
        fprintf(stderr, "%s", buf);
        fprintf(fp, "%s", buf);
        fflush(fp);
    }

    // Open the log when exit.
    atexit(show_log);

    return true;
}

//
// Show an ERROR log.
//
bool log_error(const char *s, ...)
{
    char buf[1024];
    va_list ap;

    va_start(ap, s);
    vsnprintf(buf, sizeof(buf), s, ap);
    va_end(ap);

    // Write to the log file.
    FILE *fp = openLog();
    if (fp != NULL) {
        fprintf(stderr, "%s", buf);
        fprintf(fp, "%s", buf);
        fflush(fp);
    }

    // Open the log when exit.
    atexit(show_log);

    return true;
}

// Open the log file.
static FILE *openLog(void)
{
    static FILE *fp = NULL;
    const char *cpath;

    // If already opened.
    if (fp != NULL)
        return fp;

    // If we are in the release mode.
    if (conf_release && conf_game_title != NULL) {
        // Use the "Aplication Support" folder.
        NSString *path = NSHomeDirectory();
        path = [path stringByAppendingString:@"/Library/Application Support/"];
        path = [path stringByAppendingString:[[NSString alloc] initWithUTF8String:conf_game_title]];
        [[NSFileManager defaultManager] createDirectoryAtPath:path
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:NULL];
        path = [path stringByAppendingString:@"/"];
        path = [path stringByAppendingString:[[NSString alloc] initWithUTF8String:LOG_FILE]];
        logFilePath = path;
        cpath = [path UTF8String];
        fp = fopen(cpath, "w");
        if (fp == NULL) {
            NSAlert *alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Error"];
            [alert setInformativeText:@"Cannot open log file."];
            [alert addButtonWithTitle:@"OK"];
            [alert runModal];
        }
        return fp;
    }

    // Use the folder where .app bundle exists.
    NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
    NSString *basePath = [bundlePath stringByDeletingLastPathComponent];
    logFilePath = [NSString stringWithFormat:@"%@/%s", basePath, LOG_FILE];
    cpath = [logFilePath UTF8String];
    fp = fopen(cpath, "w");
    if (fp == NULL) {
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Error"];
        [alert setInformativeText:@"Cannot open log file."];
        [alert addButtonWithTitle:@"OK"];
        [alert runModal];
    }
    return fp;
}

//
// Show the log file.
//
static void show_log(void)
{
    [[NSWorkspace sharedWorkspace] openFile:logFilePath];
}

//
// Not used in the macOS app.
//
void set_continuous_swipe_enabled(bool is_enabled)
{
    UNUSED_PARAMETER(is_enabled);
}
