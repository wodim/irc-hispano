; Script generated by the HM NIS Edit Script Wizard.
; $Id: winstaller.nsi 399 2011-02-01 11:53:08Z hal9000 $

SetCompressor /SOLID lzma

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Denora"
!define PRODUCT_VERSION "1.4.5"
!define PRODUCT_PUBLISHER "Denora"
!define PRODUCT_WEB_SITE "http://www.denorastats.org/"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\denora.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "C:\Users\Sebastian\Documents\denora\docs\COPYING"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\QUICKSTART.txt"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "Spanish"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Denora-1.4.5-x86.exe"
InstallDir "$PROGRAMFILES\Denora"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"  
  
  SetOverwrite on
  File "C:\Users\Sebastian\Documents\denora\.BANNER"
  File "C:\Users\Sebastian\Documents\denora\aclocal.m4"
  File "C:\Users\Sebastian\Documents\denora\Changes"
  File "C:\Users\Sebastian\Documents\denora\Changes.conf"
  File "C:\Users\Sebastian\Documents\denora\Changes.lang"
  File "C:\Users\Sebastian\Documents\denora\Changes.sql"
  SetOutPath "$INSTDIR\contrib\chanstats"
  File "C:\Users\Sebastian\Documents\denora\contrib\chanstats\channels.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\chanstats\chanstats.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\chanstats\common.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\chanstats\mysql.ini.php"
  SetOutPath "$INSTDIR\contrib\glines"
  File "C:\Users\Sebastian\Documents\denora\contrib\glines\glines.php"
  SetOutPath "$INSTDIR\contrib\pie_chart"
  File "C:\Users\Sebastian\Documents\denora\contrib\pie_chart\common.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\pie_chart\index.html"
  File "C:\Users\Sebastian\Documents\denora\contrib\pie_chart\mysql.ini.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\pie_chart\piegraph.class.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\pie_chart\tld_chan.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\pie_chart\tld_overall.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\pie_chart\version_chan.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\pie_chart\version_network.php"
  SetOutPath "$INSTDIR\contrib"
  File "C:\Users\Sebastian\Documents\denora\contrib\README"
  File "C:\Users\Sebastian\Documents\denora\contrib\SQL_QUERIES"
  SetOutPath "$INSTDIR\contrib\tools"
  File "C:\Users\Sebastian\Documents\denora\contrib\tools\README"
  File "C:\Users\Sebastian\Documents\denora\contrib\tools\smconv.pl"
  SetOutPath "$INSTDIR\contrib\web-api"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\denora.cfg.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\denora.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\index.php"
  SetOutPath "$INSTDIR\contrib\web-api\ircds"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\asuka.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\bahamut.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\beware.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\charybdis.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\dreamforge.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\fqircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\hybrid.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\hyperion.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\inspircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\ircdreams.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\ircu.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\liquidircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\musircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\nefarious.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\neoircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\ngircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\plexus.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\ptlink.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\quakeircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\rageircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\ratbox.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\shadowircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\solidircd.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\sorcery.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\ultimate2.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\ultimate3.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\unreal31.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\unreal32.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\ircds\viagra.php"
  SetOutPath "$INSTDIR\contrib\web-api"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\README.txt"
  File "C:\Users\Sebastian\Documents\denora\contrib\web-api\sql.php"
  SetOutPath "$INSTDIR\contrib\whois"
  File "C:\Users\Sebastian\Documents\denora\contrib\whois\dowhois.php"
  File "C:\Users\Sebastian\Documents\denora\contrib\whois\index.html"
  SetOutPath "$INSTDIR\contrib\xmlrpc"
  File "C:\Users\Sebastian\Documents\denora\contrib\xmlrpc\xmlrpc_example.php"
  SetOutPath "$INSTDIR\data\bin"
  File "C:\Users\Sebastian\Documents\denora\data\bin\cp-recursive"
  File "C:\Users\Sebastian\Documents\denora\data\bin\denorarc.in"
  File "C:\Users\Sebastian\Documents\denora\data\bin\langtool"
  File "C:\Users\Sebastian\Documents\denora\data\bin\mydbgen"
  File "C:\Users\Sebastian\Documents\denora\data\bin\register"
  SetOutPath "$INSTDIR\data\core"
  File "C:\Users\Sebastian\Documents\denora\data\core\cron_chanstats.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\cron_daily.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\event_229.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\event_242.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\event_351.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\event_motd.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\event_uline.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_ctcp.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_daily.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_map.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_misc.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_netstat.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_servdtl.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_server.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_tld.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_top10chan.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_top10kicks.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_top10topics.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\html_topchan.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_admin.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_chanstats.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_core.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_envinfo.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_exclude.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_export.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_fantasy_gstats.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_fantasy_gtop.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_fantasy_gtop10.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_fantasy_help.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_fantasy_peak.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_fantasy_stats.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_fantasy_top.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_fantasy_top10.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_help.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_html.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_login.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_modules.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_set.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_status.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\ss_userdebug.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\xmlrpc_channeldata.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\xmlrpc_channelslist.dll"
  File "C:\Users\Sebastian\Documents\denora\data\core\xmlrpc_get.dll"
  SetOutPath "$INSTDIR\data"
  File "C:\Users\Sebastian\Documents\denora\data\example.chk"
  File "C:\Users\Sebastian\Documents\denora\data\example.conf"
  File "C:\Users\Sebastian\Documents\denora\data\example.de.conf"
  File "C:\Users\Sebastian\Documents\denora\data\example.es.conf"
  File "C:\Users\Sebastian\Documents\denora\data\example.it.conf"
  File "C:\Users\Sebastian\Documents\denora\data\example.ru.conf"
  File "C:\Users\Sebastian\Documents\denora\data\GeoIP.dat"
  File "C:\Users\Sebastian\Documents\denora\data\GeoIPv6.dat"
  File "C:\Users\Sebastian\Documents\denora\data\index.tpl"
  SetOutPath "$INSTDIR\data\languages"
  File "C:\Users\Sebastian\Documents\denora\data\languages\de"
  File "C:\Users\Sebastian\Documents\denora\data\languages\en_us"
  File "C:\Users\Sebastian\Documents\denora\data\languages\es"
  File "C:\Users\Sebastian\Documents\denora\data\languages\fr"
  File "C:\Users\Sebastian\Documents\denora\data\languages\it"
  File "C:\Users\Sebastian\Documents\denora\data\languages\nl"
  File "C:\Users\Sebastian\Documents\denora\data\languages\ru"
  File "C:\Users\Sebastian\Documents\denora\data\languages\pt_br"  
  SetOutPath "$INSTDIR\data\modules"
  File "C:\Users\Sebastian\Documents\denora\data\modules\asuka.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\bahamut.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\charybdis.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\example.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\hybrid.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\hyperion.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\inspircd11.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\inspircd12.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\ircu.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\nefarious.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\nefarious13.dll"  
  File "C:\Users\Sebastian\Documents\denora\data\modules\ngircd.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\plexus3.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\ratbox.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\solidircd.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\ss_uptime.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\ultimate3.dll"
  File "C:\Users\Sebastian\Documents\denora\data\modules\unreal32.dll"
  SetOutPath "$INSTDIR\data"
  File "C:\Users\Sebastian\Documents\denora\data\stats.motd"
  SetOutPath "$INSTDIR"
  File "C:\Users\Sebastian\Documents\denora\denora.exe"
  CreateDirectory "$SMPROGRAMS\Denora Stats"
  CreateShortCut "$SMPROGRAMS\Denora Stats\Denora.lnk" "$INSTDIR\denora.exe"
  SetOutPath "$INSTDIR\docs"
  File "C:\Users\Sebastian\Documents\denora\docs\COMMANDS"
  File "C:\Users\Sebastian\Documents\denora\docs\COPYING"
  File "C:\Users\Sebastian\Documents\denora\docs\CRON"
  File "C:\Users\Sebastian\Documents\denora\docs\Donation"
  File "C:\Users\Sebastian\Documents\denora\docs\EVENTS"
  File "C:\Users\Sebastian\Documents\denora\docs\FAQ"
  File "C:\Users\Sebastian\Documents\denora\docs\INSTALL"
  File "C:\Users\Sebastian\Documents\denora\docs\IRCD"
  File "C:\Users\Sebastian\Documents\denora\docs\MODCONFIG"
  File "C:\Users\Sebastian\Documents\denora\docs\MODULES"
  File "C:\Users\Sebastian\Documents\denora\docs\MYSQL"
  File "C:\Users\Sebastian\Documents\denora\docs\WIN32.txt"
  File "C:\Users\Sebastian\Documents\denora\docs\XMLRPC"
  File "C:\Users\Sebastian\Documents\denora\docs\XMLRPC_MOD"
  SetOutPath "$INSTDIR"
  File "C:\Users\Sebastian\Documents\denora\mysql.m4"
  File "C:\Users\Sebastian\Documents\denora\README"
  File "C:\Users\Sebastian\Documents\denora\QUICKSTART.txt"
  SetOutPath "$INSTDIR\sql"
  File "C:\Users\Sebastian\Documents\denora\sql\asuka.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\bahamut.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\charybdis.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\denora.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\hybrid.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\hyperion.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\inspircd11.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\inspircd12.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\ircu.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\nefarious.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\nefarious13.sql"  
  File "C:\Users\Sebastian\Documents\denora\sql\ngircd.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\plexus3.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\ratbox.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\scarynet.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\solidircd.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\ultimate3.sql"
  File "C:\Users\Sebastian\Documents\denora\sql\unreal32.sql"
  SetOutPath "$INSTDIR"
  File "C:\Users\Sebastian\Documents\denora\libmysql.dll"
  CreateDirectory "$INSTDIR\data\backups"
  CreateDirectory "$INSTDIR\data\logs"
  CreateDirectory "$INSTDIR\data\modules\runtime"
  CreateDirectory "$INSTDIR\data\core\runtime"  
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\Denora Stats\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\Denora Stats\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\denora.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\denora.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\version.log.old"
  Delete "$INSTDIR\version.log"
  Delete "$INSTDIR\libmysql.dll"
  Delete "$INSTDIR\sql\unreal32.sql"
  Delete "$INSTDIR\sql\ultimate3.sql"
  Delete "$INSTDIR\sql\solidircd.sql"
  Delete "$INSTDIR\sql\scarynet.sql"
  Delete "$INSTDIR\sql\ratbox.sql"
  Delete "$INSTDIR\sql\plexus3.sql"
  Delete "$INSTDIR\sql\ngircd.sql"
  Delete "$INSTDIR\sql\nefarious.sql"
  Delete "$INSTDIR\sql\nefarious13.sql"
  Delete "$INSTDIR\sql\ircu.sql"
  Delete "$INSTDIR\sql\inspircd11.sql"
  Delete "$INSTDIR\sql\inspircd12.sql"
  Delete "$INSTDIR\sql\hyperion.sql"
  Delete "$INSTDIR\sql\hybrid.sql"
  Delete "$INSTDIR\sql\denora.sql"
  Delete "$INSTDIR\sql\charybdis.sql"
  Delete "$INSTDIR\sql\bahamut.sql"
  Delete "$INSTDIR\sql\asuka.sql"
  Delete "$INSTDIR\README"
  Delete "$INSTDIR\mysql.m4"
  Delete "$INSTDIR\docs\XMLRPC_MOD"
  Delete "$INSTDIR\docs\XMLRPC"
  Delete "$INSTDIR\docs\WIN32.txt"
  Delete "$INSTDIR\docs\MYSQL"
  Delete "$INSTDIR\docs\MODULES"
  Delete "$INSTDIR\docs\MODCONFIG"
  Delete "$INSTDIR\docs\IRCD"
  Delete "$INSTDIR\docs\INSTALL"
  Delete "$INSTDIR\docs\FAQ"
  Delete "$INSTDIR\docs\EVENTS"
  Delete "$INSTDIR\docs\Donation"
  Delete "$INSTDIR\docs\CRON"
  Delete "$INSTDIR\docs\COPYING"
  Delete "$INSTDIR\docs\COMMANDS"
  Delete "$INSTDIR\denora.exe.manifest"
  Delete "$INSTDIR\denora.exe"
  Delete "$INSTDIR\data\stats.motd"
  Delete "$INSTDIR\data\modules\unreal32.dll"
  Delete "$INSTDIR\data\modules\ultimate3.dll"
  Delete "$INSTDIR\data\modules\ss_uptime.dll"
  Delete "$INSTDIR\data\modules\solidircd.dll"
  Delete "$INSTDIR\data\modules\ratbox.dll"
  Delete "$INSTDIR\data\modules\plexus3.dll"
  Delete "$INSTDIR\data\modules\ngircd.dll"
  Delete "$INSTDIR\data\modules\nefarious.dll"
  Delete "$INSTDIR\data\modules\nefarious13.dll"
  Delete "$INSTDIR\data\modules\ircu.dll"
  Delete "$INSTDIR\data\modules\inspircd11.dll"
  Delete "$INSTDIR\data\modules\inspircd12.dll"
  Delete "$INSTDIR\data\modules\hyperion.dll"
  Delete "$INSTDIR\data\modules\hybrid.dll"
  Delete "$INSTDIR\data\modules\example.dll"
  Delete "$INSTDIR\data\modules\charybdis.dll"
  Delete "$INSTDIR\data\modules\bahamut.dll"
  Delete "$INSTDIR\data\modules\asuka.dll"
  Delete "$INSTDIR\data\languages\ru"
  Delete "$INSTDIR\data\languages\nl"
  Delete "$INSTDIR\data\languages\it"
  Delete "$INSTDIR\data\languages\fr"
  Delete "$INSTDIR\data\languages\es"
  Delete "$INSTDIR\data\languages\en_us"
  Delete "$INSTDIR\data\languages\de"
  Delete "$INSTDIR\data\languages\pt_br"
  Delete "$INSTDIR\data\index.tpl"
  Delete "$INSTDIR\data\GeoIP.dat"
  Delete "$INSTDIR\data\GeoIPv6.dat"
  Delete "$INSTDIR\data\example.ru.conf"
  Delete "$INSTDIR\data\example.it.conf"
  Delete "$INSTDIR\data\example.es.conf"
  Delete "$INSTDIR\data\example.de.conf"
  Delete "$INSTDIR\data\example.conf"
  Delete "$INSTDIR\data\example.chk"
  Delete "$INSTDIR\data\core\xmlrpc_get.dll"
  Delete "$INSTDIR\data\core\xmlrpc_channelslist.dll"
  Delete "$INSTDIR\data\core\xmlrpc_channeldata.dll"
  Delete "$INSTDIR\data\core\ss_userdebug.dll"
  Delete "$INSTDIR\data\core\ss_status.dll"
  Delete "$INSTDIR\data\core\ss_set.dll"
  Delete "$INSTDIR\data\core\ss_modules.dll"
  Delete "$INSTDIR\data\core\ss_login.dll"
  Delete "$INSTDIR\data\core\ss_html.dll"
  Delete "$INSTDIR\data\core\ss_help.dll"
  Delete "$INSTDIR\data\core\ss_fantasy_top10.dll"
  Delete "$INSTDIR\data\core\ss_fantasy_top.dll"
  Delete "$INSTDIR\data\core\ss_fantasy_stats.dll"
  Delete "$INSTDIR\data\core\ss_fantasy_peak.dll"
  Delete "$INSTDIR\data\core\ss_fantasy_help.dll"
  Delete "$INSTDIR\data\core\ss_fantasy_gtop10.dll"
  Delete "$INSTDIR\data\core\ss_fantasy_gtop.dll"
  Delete "$INSTDIR\data\core\ss_fantasy_gstats.dll"
  Delete "$INSTDIR\data\core\ss_export.dll"
  Delete "$INSTDIR\data\core\ss_exclude.dll"
  Delete "$INSTDIR\data\core\ss_envinfo.dll"
  Delete "$INSTDIR\data\core\ss_core.dll"
  Delete "$INSTDIR\data\core\ss_chanstats.dll"
  Delete "$INSTDIR\data\core\ss_admin.dll"
  Delete "$INSTDIR\data\core\html_topchan.dll"
  Delete "$INSTDIR\data\core\html_top10topics.dll"
  Delete "$INSTDIR\data\core\html_top10kicks.dll"
  Delete "$INSTDIR\data\core\html_top10chan.dll"
  Delete "$INSTDIR\data\core\html_tld.dll"
  Delete "$INSTDIR\data\core\html_server.dll"
  Delete "$INSTDIR\data\core\html_servdtl.dll"
  Delete "$INSTDIR\data\core\html_netstat.dll"
  Delete "$INSTDIR\data\core\html_misc.dll"
  Delete "$INSTDIR\data\core\html_map.dll"
  Delete "$INSTDIR\data\core\html_daily.dll"
  Delete "$INSTDIR\data\core\html_ctcp.dll"
  Delete "$INSTDIR\data\core\event_uline.dll"
  Delete "$INSTDIR\data\core\event_motd.dll"
  Delete "$INSTDIR\data\core\event_351.dll"
  Delete "$INSTDIR\data\core\event_242.dll"
  Delete "$INSTDIR\data\core\event_229.dll"
  Delete "$INSTDIR\data\core\cron_daily.dll"
  Delete "$INSTDIR\data\core\cron_chanstats.dll"
  Delete "$INSTDIR\data\bin\register"
  Delete "$INSTDIR\data\bin\mydbgen"
  Delete "$INSTDIR\data\bin\langtool"
  Delete "$INSTDIR\data\bin\dm"
  Delete "$INSTDIR\data\bin\denorarc.in"
  Delete "$INSTDIR\data\bin\cp-recursive"
  Delete "$INSTDIR\contrib\xmlrpc\xmlrpc_example.php"
  Delete "$INSTDIR\contrib\whois\index.html"
  Delete "$INSTDIR\contrib\whois\dowhois.php"
  Delete "$INSTDIR\contrib\web-api\sql.php"
  Delete "$INSTDIR\contrib\web-api\README.txt"
  Delete "$INSTDIR\contrib\web-api\ircds\viagra.php"
  Delete "$INSTDIR\contrib\web-api\ircds\unreal32.php"
  Delete "$INSTDIR\contrib\web-api\ircds\unreal31.php"
  Delete "$INSTDIR\contrib\web-api\ircds\ultimate3.php"
  Delete "$INSTDIR\contrib\web-api\ircds\ultimate2.php"
  Delete "$INSTDIR\contrib\web-api\ircds\sorcery.php"
  Delete "$INSTDIR\contrib\web-api\ircds\solidircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\shadowircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\ratbox.php"
  Delete "$INSTDIR\contrib\web-api\ircds\rageircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\quakeircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\ptlink.php"
  Delete "$INSTDIR\contrib\web-api\ircds\plexus.php"
  Delete "$INSTDIR\contrib\web-api\ircds\ngircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\neoircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\nefarious.php"
  Delete "$INSTDIR\contrib\web-api\ircds\musircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\liquidircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\ircu.php"
  Delete "$INSTDIR\contrib\web-api\ircds\ircdreams.php"
  Delete "$INSTDIR\contrib\web-api\ircds\inspircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\hyperion.php"
  Delete "$INSTDIR\contrib\web-api\ircds\hybrid.php"
  Delete "$INSTDIR\contrib\web-api\ircds\fqircd.php"
  Delete "$INSTDIR\contrib\web-api\ircds\dreamforge.php"
  Delete "$INSTDIR\contrib\web-api\ircds\charybdis.php"
  Delete "$INSTDIR\contrib\web-api\ircds\beware.php"
  Delete "$INSTDIR\contrib\web-api\ircds\bahamut.php"
  Delete "$INSTDIR\contrib\web-api\ircds\asuka.php"
  Delete "$INSTDIR\contrib\web-api\index.php"
  Delete "$INSTDIR\contrib\web-api\denora.php"
  Delete "$INSTDIR\contrib\web-api\denora.cfg.php"
  Delete "$INSTDIR\contrib\tools\smconv.pl"
  Delete "$INSTDIR\contrib\tools\README"
  Delete "$INSTDIR\contrib\SQL_QUERIES"
  Delete "$INSTDIR\contrib\README"
  Delete "$INSTDIR\contrib\pie_chart\version_network.php"
  Delete "$INSTDIR\contrib\pie_chart\version_chan.php"
  Delete "$INSTDIR\contrib\pie_chart\tld_overall.php"
  Delete "$INSTDIR\contrib\pie_chart\tld_chan.php"
  Delete "$INSTDIR\contrib\pie_chart\piegraph.class.php"
  Delete "$INSTDIR\contrib\pie_chart\mysql.ini.php"
  Delete "$INSTDIR\contrib\pie_chart\index.html"
  Delete "$INSTDIR\contrib\pie_chart\common.php"
  Delete "$INSTDIR\contrib\glines\glines.php"
  Delete "$INSTDIR\contrib\deprecated_protocols\viagra.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\viagra.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\unreal31.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\unreal31.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\ultimate2.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\ultimate2.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\viagra.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\unreal31.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\ultimate2.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\sorcery.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\shadowircd.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\rageircd.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\quakeircd.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\ptlink.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\plexus.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\neoircd.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\musircd.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\liquidircd.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\ircdreams.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\inspircd.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\fqircd.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\dreamforge.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sql\beware.sql"
  Delete "$INSTDIR\contrib\deprecated_protocols\sorcery.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\sorcery.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\shadowircd.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\shadowircd.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\README"
  Delete "$INSTDIR\contrib\deprecated_protocols\rageircd.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\rageircd.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\quakeircd.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\quakeircd.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\ptlink.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\ptlink.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\plexus.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\plexus.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\neoircd.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\neoircd.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\musircd.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\musircd.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\liquidircd.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\liquidircd.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\ircdreams.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\ircdreams.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\inspircd.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\inspircd.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\fqircd.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\fqircd.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\dreamforge.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\dreamforge.c"
  Delete "$INSTDIR\contrib\deprecated_protocols\beware.h"
  Delete "$INSTDIR\contrib\deprecated_protocols\beware.c"
  Delete "$INSTDIR\contrib\chanstats\mysql.ini.php"
  Delete "$INSTDIR\contrib\chanstats\common.php"
  Delete "$INSTDIR\contrib\chanstats\chanstats.php"
  Delete "$INSTDIR\contrib\chanstats\channels.php"
  Delete "$INSTDIR\Changes.sql"
  Delete "$INSTDIR\Changes.lang"
  Delete "$INSTDIR\Changes.conf"
  Delete "$INSTDIR\Changes"
  Delete "$INSTDIR\QUICKSTART.txt"
  Delete "$INSTDIR\aclocal.m4"
  Delete "$INSTDIR\.BANNER"

  Delete "$SMPROGRAMS\Denora Stats\Uninstall.lnk"
  Delete "$SMPROGRAMS\Denora Stats\Website.lnk"
  Delete "$DESKTOP\Denora.lnk"
  Delete "$SMPROGRAMS\Denora Stats\Denora.lnk"

  RMDir "$SMPROGRAMS\Denora Stats"
  RMDir "$INSTDIR\sql"
  RMDir "$INSTDIR\docs"
  RMDir "$INSTDIR\data\modules"
  RMDir "$INSTDIR\data\languages"
  RMDir "$INSTDIR\data\core"
  RMDir "$INSTDIR\data\bin"
  RMDir "$INSTDIR\data"
  RMDir "$INSTDIR\contrib\xmlrpc"
  RMDir "$INSTDIR\contrib\whois"
  RMDir "$INSTDIR\contrib\web-api\ircds"
  RMDir "$INSTDIR\contrib\web-api"
  RMDir "$INSTDIR\contrib\tools"
  RMDir "$INSTDIR\contrib\pie_chart"
  RMDir "$INSTDIR\contrib\glines"
  RMDir "$INSTDIR\contrib\deprecated_protocols\sql"
  RMDir "$INSTDIR\contrib\deprecated_protocols"
  RMDir "$INSTDIR\contrib\chanstats"
  RMDir "$INSTDIR\contrib"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
