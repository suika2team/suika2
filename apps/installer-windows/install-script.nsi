!include "MUI2.nsh"
!define MUI_ICON "icon.ico"
!define MUI_UNICON "icon.ico"

Name "OpenNovel"
OutFile "opennovel-installer.exe"
InstallDir "$APPDATA\Local\OpenNovel"

RequestExecutionLevel user
SetCompressor /SOLID /FINAL lzma
SilentInstall normal

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "doc.rtf"
Page directory
Page instfiles

Section "Install"
  SetOutPath "$INSTDIR"
  RMDir /r "$INSTDIR"
  File "opennovel.exe"
  File "engine.exe"
  File /r "sample"
  File /r "tools"
  File /r "manual"
  File "icon.ico"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  CreateDirectory "$SMPROGRAMS\OpenNovel"
  CreateShortcut "$SMPROGRAMS\OpenNovel\OpenNovel.lnk" "$INSTDIR\OpenNovel.exe" ""
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenNovel" "InstDir" '"$INSTDIR"'
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenNovel" "DisplayName" "OpenNovel"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenNovel" "DisplayIcon" '"$INSTDIR\icon.ico"'
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenNovel" "DisplayVersion" "1"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenNovel" "Publisher" "The OpenNovel Authors"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenNovel" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  WriteRegStr HKCU "Software\Classes\.opennovel" "" "OpenNovel.project"
  WriteRegStr HKCU "Software\Classes\OpenNovel.project" "" "OpenNovel Project"
  WriteRegStr HKCU "Software\Classes\OpenNovel.project\DefaultIcon" "" "$INSTDIR\editor.exe"
  WriteRegStr HKCU "Software\Classes\OpenNovel.project\Shell\open\command" "" '"$INSTDIR\editor.exe" "%1"'
  SetShellVarContext current
  CreateShortCut "$DESKTOP\OpenNovel.lnk" "$INSTDIR\editor.exe"
SectionEnd

Section "Uninstall"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\opennovel.exe"
  Delete "$INSTDIR\engine.exe"
  Delete "$INSTDIR\sample"
  Delete "$INSTDIR\tools"
  Delete "$INSTDIR\manual"
  Delete "$SMPROGRAMS\OpenNovel\OpenNovel.lnk"
  RMDir "$SMPROGRAMS\OpenNovel"
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\OpenNovel"
  Delete "$DESKTOP\OpenNovel.lnk"
  DeleteRegKey HKCU "Software\Classes\.opennovel"
  DeleteRegKey HKCU "Software\Classes\OpenNovel.project"
  DeleteRegKey HKCU "Software\Classes\OpenNovel.project\DefaultIcon"
  DeleteRegKey HKCU "Software\Classes\OpenNovel.project\Shell\open\command"
SectionEnd

Function .OnInstSuccess
  Exec "$INSTDIR\opennovel.exe"
FunctionEnd

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Japanese"
