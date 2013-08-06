; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
; TODO don't use chinese in PRODUCT_UNINST_KEY 
!define PRODUCT_NAME "New Chewing IM"
!define PRODUCT_VERSION "0.4-dev"
!define PRODUCT_PUBLISHER "PCMan (洪任諭), seamxr, andyhorng, sky008888, kcwu"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define TMPDIR "$TEMP\ChewingInst"

;SetCompressor /SOLID lzma
SetCompressor zlib

!include "LogicLib.nsh"
; MUI 1.67 compatible ------
;!define MUI_VERBOSE 100
!include "MUI.nsh"

  ; MUI Settings
  !define MUI_ABORTWARNING
  !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"
  
  BGGradient 0000FF 000000 FFFFFF

  ; Welcome page
  !insertmacro MUI_PAGE_WELCOME
  
  ; License page
  !define MUI_LICENSEPAGE_RADIOBUTTONS
  !insertmacro MUI_PAGE_LICENSE "$(LICENSE_FILE)"
  
  ; Directory page
  ; !insertmacro MUI_PAGE_DIRECTORY
  
  ; Instfiles page
  !insertmacro MUI_PAGE_INSTFILES
  
  ; Finish page
  !define MUI_FINISHPAGE_LINK_LOCATION "http://chewing.im/"
  !define MUI_FINISHPAGE_LINK "$(VISIT_WEBSITE) ${MUI_FINISHPAGE_LINK_LOCATION}"
  !insertmacro MUI_PAGE_FINISH
  
  ; Uninstaller pages
  !insertmacro MUI_UNPAGE_INSTFILES
  
  
  ; language selection
  !define MUI_LANGDLL_REGISTRY_ROOT "HKLM" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\Chewing" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"
  ; Language
  !include "nsis\lang.nsh"
  ; first is default
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "TradChinese"
  !include "nsis\translations\English.nsh"
  !include "nsis\translations\TradChinese.nsh"
  
  ; Reserve files
  !insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
  !insertmacro MUI_RESERVEFILE_LANGDLL

; MUI end ------

Name "$(NAME)"
OutFile "win32-chewing-${PRODUCT_VERSION}.exe"
InstallDir "$SYSDIR\IME\Chewing"
ShowInstDetails show
ShowUnInstDetails show

Function uninstOld
  ClearErrors
  ${If} ${FileExists} "$SYSDIR\Chewing.ime"
    Delete "$SYSDIR\Chewing.ime"
    ${If} ${Errors}
      MessageBox MB_ICONSTOP|MB_OK "無法移除已存在的新酷音client端。$\n通常是因為舊版的新酷音client端已經被某些程式載入而無法移除。$\n請關閉所有程式或重新開機後，再安裝一次即可。"
      Abort
    ${EndIf}
  ${EndIf}


  ;  shutdown chewing server.
  ExecWait '"$SYSDIR\IME\Chewing\Installer.exe" /uninstall'

  ;  run uninstaller
  ReadRegStr $R0 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString"
  ;MessageBox MB_OK|MB_ICONINFORMATION "R0 = '$R0'" IDOK
  ${If} $R0 != ""
    ClearErrors
    ExecWait '$R0 /S _?=$INSTDIR' ;Do not copy the uninstaller to a temp file
 
    ${Unless} ${Errors}
      Delete $R0
    ${EndIf}
  ${EndIf}
  
;  ; uninst.exe will copy itself to $TEMP\~nsu.tmp\Au_.exe and execute that copy.
;  ; Therefore, ExecWait "$INSTDIR\uninst.exe" is useless since it terminates after
;  ; executing the copy Au_.exe.  We should ExecWait Au_.exe instead.
;  ; This is a dirty hack to mimic the behavior of default NSIS uninst.exe.
;  ; Reference: http://nsis.cvs.sourceforge.net/nsis/NSIS/Source/exehead/Main.c?view=markup
;  SetOverwrite on
;  SetOutPath "$TEMP\~nsu.tmp"
;  CopyFiles /SILENT "$SYSDIR\IME\Chewing\uninst.exe" "$TEMP\~nsu.tmp\Au_.exe"
;
;  ; This is really dirty! :-(
;  ; uninst.exe of NSIS will try Au_.exe, Bu_.exe, Cu_.exe, ...Zu_.exe until success.
;  ; There is little chance that Au_.exe cannot be use, so I omit this.
;  ExecWait '"$TEMP\~nsu.tmp\Au_.exe" /S _?=$SYSDIR\IME\Chewing\'
;  Delete "$TEMP\~nsu.tmp\Au_.exe"
;  RMDir "$TEMP\~nsu.tmp"
;  ClearErrors

  ; Ensure the old IME is deleted.
  Delete "$SYSDIR\Chewing.ime"
  ${If} ${Errors}
    Call OnInstError
  ${EndIf}
FunctionEnd

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY

  ReadRegStr $0 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion"
  ${If} $0 != ""
    MessageBox MB_OKCANCEL|MB_ICONQUESTION "$(DETECTED_OLD_VERSION) $0 $(REINSTALL_CONFIRM)" IDOK +2
      Abort
      Call uninstOld
  ${EndIf}
FunctionEnd

Function OnInstError
    MessageBox MB_ICONSTOP|MB_OK "$(INSTALL_FAIL)"
    Abort
FunctionEnd

;!define LIBCHEWING_DATA_PATH	"..\libchewingdata"

;----------------------------------------------------------------------------------------------

Section "MainSection" SEC01
  ClearErrors
  ${If} ${Errors}
    Call OnInstError
  ${EndIf}
  SetOverwrite on

;  File "..\libchewing-data\utf-8\us_freq.dat"
;  File /oname=ph_index.dat "..\libchewing-data\utf-8\ph_index.dat"
;  File /oname=fonetree.dat "..\libchewing-data\utf-8\fonetree.dat"
;  File "..\libchewing-data\utf-8\dict.dat"
;  File /oname=ch_index.dat "..\libchewing-data\utf-8\ch_index.dat"

; Generate data files on installation to reduce the size of installer.
  ;SetOutPath "${TMPDIR}"
  ;File "big52utf8\Release\big52utf8.exe"
  ;${If} ${Errors}
  ;  Call OnInstError
  ;${EndIf}

  ;File "${LIBCHEWING_DATA_PATH}\utf-8\tsi.src"
  ;File "${LIBCHEWING_DATA_PATH}\utf-8\phone.cin"
  ;File "${LIBCHEWING_DATA_PATH}\utf-8\dat2bin.exe"
  ;ExecWait '"${TMPDIR}\dat2bin.exe"'
  ;${If} ${Errors}
  ;  Call OnInstError
  ;${EndIf}

  ; Rename will fail if destination file exists. So, delete them all.
  ;Delete "$SYSDIR\IME\Chewing\*"
  ; If the files to delete don't exist, error flag if *NOT* set.
  ;${If} ${Errors}
  ;  Call OnInstError
  ;${EndIf}

  SetOutPath "$SYSDIR\IME\Chewing"
  ;Rename "${TMPDIR}\dat2bin.exe" 'dat2bin.exe'
  ;Rename "${TMPDIR}\ch_index.dat_bin" 'ch_index.dat'
  ;Rename "${TMPDIR}\dict.dat" 'dict.dat'
  ;Rename "${TMPDIR}\us_freq.dat" 'us_freq.dat'
  ;Rename "${TMPDIR}\ph_index.dat_bin" 'ph_index.dat'
  ;Rename "${TMPDIR}\fonetree.dat_bin" 'fonetree.dat'
  ;${If} ${Errors}
  ;  Call OnInstError
  ;${EndIf}

  File "Data\statuswnd.bmp"
  File "License.txt"	; TODO handle translated license file
  File "UserGuide\chewing.chm"
  File "Installer\Release\Installer.exe"
  File "ChewingServer\Release\ChewingServer.exe"
  File "HashEd-UTF8\Release\HashEd.exe"
  File "OnlineUpdate\Release\Update.exe"

  SetOverwrite off
  ; TODO should not place on system-wise directory
  File "C:\Program Files\libchewing\share\chewing\symbols.dat"
  File "C:\Program Files\libchewing\share\chewing\swkb.dat"
  SetOverwrite on
  File "C:\Program Files\libchewing\share\chewing\ch_index_begin.dat"
  File "C:\Program Files\libchewing\share\chewing\ch_index_phone.dat"
  File "C:\Program Files\libchewing\share\chewing\dict.dat"
  File "C:\Program Files\libchewing\share\chewing\fonetree.dat"
  File "C:\Program Files\libchewing\share\chewing\ph_index.dat"
  File "C:\Program Files\libchewing\share\chewing\us_freq.dat"
  File "C:\Program Files\libchewing\lib\chewing.dll"
  
  ExecWait '"$SYSDIR\IME\Chewing\Installer.exe" /privilege'

  SetOutPath "$SYSDIR"
  File "ChewingIME\Release\Chewing.ime"
  ${If} ${Errors}
    File /oname=Chewing-tmp.ime "ChewingIME\Release\Chewing.ime"
    Rename /REBOOTOK Chewing-tmp.ime Chewing.ime
  ${EndIf}
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  CreateDirectory "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)"
  CreateShortCut "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)\$(LNK_USER_MANUAL).lnk" "$INSTDIR\Chewing.chm"
  CreateShortCut "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)\$(LNK_DB_EDITOR).lnk" "$INSTDIR\HashEd.exe"
  CreateShortCut "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)\$(LNK_UPDATE_CHECKER).lnk" "$INSTDIR\Update.exe"
  CreateShortCut "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)\$(LNK_UNINSTALL).lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$SYSDIR\IME\Chewing\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

  ExecWait '"$SYSDIR\IME\Chewing\Installer.exe"'

  SetShellVarContext current
  ${Unless} ${FileExists} $APPDATA\Chewing\uhash.dat
    ${If} ${FileExists} $APPDATA\Chewing\hash.dat
        ExecWait '"${TMPDIR}\big52utf8.exe" $APPDATA\Chewing\hash.dat'
    ${EndIf}
  ${EndIf}

  Delete "${TMPDIR}\*"
  RMDir "${TMPDIR}"

  ${If} ${Errors}
    Call OnInstError
  ${EndIf}

SectionEnd

Function un.onUninstSuccess
;  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(UNINSTALL_SUCCESS)" /SD IDOK
FunctionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE

  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "$(UNINSTALL_CONFIRM)" /SD IDYES IDYES +2
  Abort
FunctionEnd

Section Uninstall
  ;  shutdown chewing server.
  ExecWait '"$SYSDIR\IME\Chewing\Installer.exe" /uninstall'

  Delete "$INSTDIR\License.txt"	; TODO handle translated license file
  Delete "$INSTDIR\statuswnd.bmp"
  Delete "$INSTDIR\ch_index.dat"
  Delete "$INSTDIR\ch_index_phone.dat"
  Delete "$INSTDIR\ch_index_begin.dat"
  Delete "$INSTDIR\dict.dat"
  Delete "$INSTDIR\fonetree.dat"
  Delete "$INSTDIR\ph_index.dat"
  Delete "$INSTDIR\us_freq.dat"
  Delete "$INSTDIR\Chewing.chm"
  Delete "$INSTDIR\Installer.exe"
  Delete "$INSTDIR\ChewingServer.exe"
  Delete "$INSTDIR\chewing.dll"
  Delete "$INSTDIR\HashEd.exe"
  Delete "$INSTDIR\Update.exe"

  Delete "$INSTDIR\symbols.dat"
  Delete "$INSTDIR\swkb.dat"
  Delete "$INSTDIR\dat2bin.exe"

  Delete "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)\$(LNK_USER_MANUAL).lnk"
  Delete "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)\$(LNK_DB_EDITOR).lnk"
  Delete "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)\$(LNK_UNINSTALL).lnk"
  Delete "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)\$(LNK_UPDATE_CHECKER).lnk"

  RMDir "$SMPROGRAMS\$(STARTMENU_FOLDER_NAME)"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"

  Delete "$INSTDIR\uninst.exe"
  RMDir "$SYSDIR\IME\Chewing"

  ; Put Delete Chewing.ime in last line, or other files will not be deleted
  ; because the uninstaller aborts when there is any error.
  ClearErrors
  Delete "$SYSDIR\Chewing.ime"
  ${If} ${Errors}
    Delete /REBOOTOK "$SYSDIR\Chewing.ime"
  ${EndIf}

  SetAutoClose true
SectionEnd
