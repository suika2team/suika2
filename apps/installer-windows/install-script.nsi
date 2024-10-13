!include "MUI2.nsh"
!define MUI_ICON "icon.ico"
!define MUI_UNICON "icon.ico"

Name "Suika2"
OutFile "suika2-installer.exe"
InstallDir "$APPDATA\Local\Suika2"

RequestExecutionLevel user
SetCompressor /SOLID /FINAL lzma
SilentInstall normal

!insertmacro MUI_PAGE_WELCOME
Page directory
Page instfiles

Section "Install"
  SetOutPath "$INSTDIR"
  RMDir /r "$INSTDIR"
  File "editor.exe"
  File "engine.exe"
  File /r "games"
  File /r "tools"
  File /r "manual"
  File "icon.ico"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  CreateDirectory "$SMPROGRAMS\Suika2"
  CreateShortcut "$SMPROGRAMS\Suika2\Suika2.lnk" "$INSTDIR\editor.exe" ""
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Suika2" "InstDir" '"$INSTDIR"'
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Suika2" "DisplayName" "Suika2"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Suika2" "DisplayIcon" '"$INSTDIR\icon.ico"'
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Suika2" "DisplayVersion" "22.0"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Suika2" "Publisher" "The Suika2 Team"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Suika2" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  WriteRegStr HKCU "Software\Classes\.suika2" "" "Suika2.project"
  WriteRegStr HKCU "Software\Classes\Suika2.project" "" "Suika2 Project"
  WriteRegStr HKCU "Software\Classes\Suika2.project\DefaultIcon" "" "$INSTDIR\editor.exe"
  WriteRegStr HKCU "Software\Classes\Suika2.project\Shell\open\command" "" '"$INSTDIR\editor.exe" "%1"'
  SetShellVarContext current
  CreateShortCut "$DESKTOP\Suika2.lnk" "$INSTDIR\editor.exe"
SectionEnd

Section "Uninstall"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\editor.exe"
  Delete "$INSTDIR\engine.exe"
  Delete "$INSTDIR\games"
  Delete "$INSTDIR\tools"
  Delete "$INSTDIR\manual"
  Delete "$SMPROGRAMS\Suika2\Suika2.lnk"
  RMDir "$SMPROGRAMS\Suika2"
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Suika2"
  Delete "$DESKTOP\Suika2.lnk"
  DeleteRegKey HKCU "Software\Classes\.suika2"
  DeleteRegKey HKCU "Software\Classes\Suika2.project"
  DeleteRegKey HKCU "Software\Classes\Suika2.project\DefaultIcon"
  DeleteRegKey HKCU "Software\Classes\Suika2.project\Shell\open\command"
SectionEnd

Function .OnInstSuccess
  Exec "$INSTDIR\editor.exe"
FunctionEnd

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Japanese"
