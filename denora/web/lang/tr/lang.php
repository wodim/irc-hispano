<?php
/* Turkish Langfile for phpDenora 1.3.0
$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $

 Author:	tronic 		<tronic@hotmail.com.tr>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'tr_TR', 'tur', 'tr');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %H:%M %Z";
$denora_format_short = "%d/%m/%y %H:%M:%S";

/* Top Menu */
DEFINE('_TM_HOME','Anasayfa');
DEFINE('_TM_NETWORK','Network');
DEFINE('_TM_GRAPHS','Grafikleri');
DEFINE('_TM_CHANS','Kanallar');
DEFINE('_TM_USERS','Kullanicilar');
DEFINE('_TM_SEARCH','Arama');
DEFINE('_TM_CONFIG','Ayarlar');
DEFINE('_TM_TITLE','IRC Istatistikleri');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Network Durumu');
DEFINE('_SM_NETDETAILS','Detaylar');
DEFINE('_SM_SERVERS','Sunucular');
DEFINE('_SM_SRVLIST','Sunucu Listesi');
DEFINE('_SM_SRVDETAILS','Sunucu Detayi');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Network Grafikleri');
DEFINE('_SM_CUSTOM','Diðer...');
DEFINE('_SM_NETSPLIT','NetSplit.de Grafikleri');
DEFINE('_SM_SEARCHIRC','SearchIRC Grafikleri');
DEFINE('_SM_CHANLIST','Kanal Listesi');
DEFINE('_SM_TOPCHANS','Kanal Istatistikleri');
DEFINE('_SM_SEARCHCHAN','Kanal Arama...');
DEFINE('_SM_CHANNELSTATUS','Kanal Durumu');
DEFINE('_SM_CHANNELACT','Kanal Etklinligi');
DEFINE('_SM_WHOIN','Kimler Çevrimiçi?');
DEFINE('_SM_TOPUSERS','Kullanici Istatistikleri');
DEFINE('_SM_SEARCHUSER','Kullanici Arama...');
DEFINE('_SM_GLOBALSTATS','Genel Istatistikler');
DEFINE('_SM_CHANSTATS','Kanal Istatistikleri');
DEFINE('_SM_CHANINFO','Kanal Bilgisi');
DEFINE('_SM_TLDPIE','Country Statistics');
DEFINE('_SM_CLIENTPIE','Kullanici Istatistikleri');
DEFINE('_SM_INVITES','Kanal Davet Listesi');
DEFINE('_SM_BANS','Kanal Ban Listesi');
DEFINE('_SM_EXCEPTS','Kanal Ayrim Listesi');
DEFINE('_SM_TSTATS','Toplam');
DEFINE('_SM_MSTATS','Bu ay');
DEFINE('_SM_WSTATS','Bu Hafta');
DEFINE('_SM_DSTATS','Bugün');
DEFINE('_SM_ASTATS','Daily Average'); // TRANSLATE!
DEFINE('_SM_LANGUAGE','Dil');
DEFINE('_SM_THEME','Tema');
DEFINE('_SM_SET','Gönder');
DEFINE('_SM_BEHAVIOR','Hareket');
DEFINE('_SM_OPERS','Operatörler');
DEFINE('_SM_PYSTATS','Geçen Yil');
DEFINE('_SM_PMSTATS','Geçen Ay');
DEFINE('_SM_PWSTATS','Geçen Hafta');
DEFINE('_SM_NSWEEKS','Last Two Weeks'); // TRANSLATE!
DEFINE('_SM_NSMONTHS','Last Two Months'); // TRANSLATE!
DEFINE('_SM_NSYEARS','Last Two Years'); // TRANSLATE!
DEFINE('_SM_NSHISTORY','Complete History'); // TRANSLATE!

/* Front Page */
DEFINE('_FP_RANKING','Current Ranking'); // TRANSLATE!
DEFINE('_FP_BIGGESTCHANS','Current Top %s Channels'); // TRANSLATE!
DEFINE('_FP_TOPCHANS','Top %s Channels Today'); // TRANSLATE!
DEFINE('_FP_TOPUSERS','Top %s Users Today'); // TRANSLATE!
DEFINE('_FP_STATUS','Network Durumu');
DEFINE('_FP_MAXSTATS','Maximun Istatistikler');
DEFINE('_FP_USERS','Kullanicilar:');
DEFINE('_FP_CHANS','Kanallar:');
DEFINE('_FP_OPERS','Operatörler:');
DEFINE('_FP_SERVERS','Sunucular:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','Hýzlý Menu');
DEFINE('_FP_QSTATS','Hýzlý Istatistik');
DEFINE('_FP_LSTATUS','Su anki Durum');
DEFINE('_FP_QUSER','Kullanýcý');
DEFINE('_FP_QCHAN','Kanal');
DEFINE('_FP_GO','Go'); // TRANSLATE!

/* Titles on Pages */
DEFINE('_PT_LUSERS','Network Durumu');
DEFINE('_PT_SERVERS','Sunucu Listesi');
DEFINE('_PT_DETAILS','Sunucu Detayi %s');
DEFINE('_PT_MOTD','Günün Mesaji (MOTD)');
DEFINE('_PT_CHANLIST','Kanal Listesi (/list)');
DEFINE('_PT_TCSTATS','Kanal Aktivitesi');
DEFINE('_PT_CHANNELACT','Ortalama Kanal Aktivitesi');
DEFINE('_PT_WHOIN','Su anda %s');
DEFINE('_PT_SEARCHCHAN','Kanal Arama');
DEFINE('_PT_USERACT','Ortalama Kullanici Aktivitesi');
DEFINE('_PT_SEARCHUSER','Kullanici Arama');
DEFINE('_PT_TSTATS','Kullanici Aktivitesi');
DEFINE('_PT_STATSFOR','Istatistikler %s on %s');
DEFINE('_PT_TLDPIE','Ulke istatistikleri %s');
DEFINE('_PT_CLIENTPIE','Kullanici istatikleri %s');
DEFINE('_PT_MSTATS','Bu ay');
DEFINE('_PT_WSTATS','Bu hafta');
DEFINE('_PT_DSTATS','Bugün');
DEFINE('_PT_SEARCHRESULTS','Arama Sonuçlari');
DEFINE('_PT_OPERS','Su anki Operatorler');
DEFINE('_PT_NSU2W','Relation of users and channels during the last 2 weeks'); // TRANSLATE!
DEFINE('_PT_NSC2W','Channels during the last 2 weeks'); // TRANSLATE!
DEFINE('_PT_NSS2W','Servers during the last 2 weeks'); // TRANSLATE!
DEFINE('_PT_NSU2M','Relation of users and channels during the last 2 months'); // TRANSLATE!
DEFINE('_PT_NSC2M','Channels during the last 2 months'); // TRANSLATE!
DEFINE('_PT_NSS2M','Servers during the last 2 months'); // TRANSLATE!
DEFINE('_PT_NSU2Y','Relation of users and channels during the last 2 years'); // TRANSLATE!
DEFINE('_PT_NSC2Y','Channels during the last 2 years'); // TRANSLATE!
DEFINE('_PT_NSS2Y','Servers during the last 2 years'); // TRANSLATE!
DEFINE('_PT_NSHYS','Complete measured history about users and channels'); // TRANSLATE!

/* Network Status */
DEFINE('_NS_USERDET','Su anki kullanicilar: %s Bugün en fazla: %s En fazla: %s on %s');
DEFINE('_NS_CHANDET','Su anki kanallar: %s En fazla: %s on %s');
DEFINE('_NS_OPERDET','Su anki operatörler: %s En fazla: %s on %s');
DEFINE('_NS_SERVDET','Su anki sunucular: %s En fazla: %s on %s');

/* Search */
DEFINE('_SP_PROMPT','Enter the search criteria. Wildcards are also supported.');
DEFINE('_SP_SUBMIT','Arama');
DEFINE('_SP_CHANINFO','Kanal Bilgisi');
DEFINE('_SP_CHANSTATS','Kanal Istatistikleri');
DEFINE('_SP_USERINFO','Kullanici Bilgisi');
DEFINE('_SP_USERSTATS','Kullanici Istatistikleri');

/* Configuration Page */
DEFINE('_CP_SETSORT','Set the default sorting criteria');
DEFINE('_CP_SETTYPE','Set the default statistics type');

/* General Definitions */
DEFINE('_GD_NUMBER','n.');
DEFINE('_GD_NICK','rumuz');
DEFINE('_GD_HOST','maske');
DEFINE('_GD_CHANNEL','kanal');
DEFINE('_GD_USERS','kullanicilar');
DEFINE('_GD_LETTERS','harfler');
DEFINE('_GD_WORDS','kelimeler');
DEFINE('_GD_LINES','satirlar');
DEFINE('_GD_ACTIONS','actions');
DEFINE('_GD_SMILEYS','göz kirpmalar');
DEFINE('_GD_KICKS','atilmalar');
DEFINE('_GD_MODES','modlar');
DEFINE('_GD_TOPICS','basliklar');
DEFINE('_GD_ON','on');
DEFINE('_GD_TYPE','çesit');
DEFINE('_GD_TOTAL','toplam');
DEFINE('_GD_OTHER','diger');
DEFINE('_GD_UNKNOWN','bilinmeyen');
DEFINE('_GD_NEVER','asla');
DEFINE('_GD_NONE','hiç bir');
DEFINE('_GD_STATUS','Durum');
DEFINE('_GD_YES','Evet');
DEFINE('_GD_NO','Hayir');
DEFINE('_GD_HOUR','Saat');
DEFINE('_GD_DAY','Gün');
DEFINE('_GD_MONTH','Ay');
DEFINE('_GD_YEAR','Yil');
DEFINE('_GD_OR','or'); // TRANSLATE!
DEFINE('_GD_AND','and'); // TRANSLATE!

/* Channel Status */
DEFINE('_CS_INFO0','Kanal: %s');
DEFINE('_CS_INFO1','Baslik: %s');
DEFINE('_CS_INFO2','Set by: %s On: %s');
DEFINE('_CS_INFO3','Su anki kullanicilar: %s Peak: %s on %s');
DEFINE('_CS_INFO4','Su anki modlar: %s');
DEFINE('_CS_INFO5','Davetler: %s Yasaklar: %s Ayricaliklilar: %s');

/* User Info */
DEFINE('_UI_UINFO','%s kullanici bilgisi (stats group %s)');
DEFINE('_UI_NODATA','The user %s appears to be %s and there is no information cached.');
DEFINE('_UI_UCHANS','Izlenen kanallar: %s');
DEFINE('_UI_WHOIS1','Rumuz: %s Gerçek Ad: %s');
DEFINE('_UI_WHOIS2','Maske: %s');
DEFINE('_UI_WHOIS3','Durum: %s');
DEFINE('_UI_WHOIS4','Çevrimiçi süresi: %s');
DEFINE('_UI_WHOIS5','Son çkis: %s');
DEFINE('_UI_WHOIS6','Away Status: %s');
DEFINE('_UI_WHOIS7','Mesaj: %s');
DEFINE('_UI_WHOIS8','Kullandigi sunucu: %s');
DEFINE('_UI_WHOIS9','Kullandigi sunucu: %s On server: %s');
DEFINE('_UI_WHOIS10','Additional Information: %s'); // TRANSLATE!

/* Oper list */
DEFINE('_OL_NICK','Rumuz');
DEFINE('_OL_LEVEL','Seviye');
DEFINE('_OL_ONLINE','Ne zamandir çevrimiçi');
DEFINE('_OL_LASTQUIT','Son çikis');
DEFINE('_OL_BOT','Bot Servisi');
DEFINE('_OL_SERVICE','Network Servisi');
DEFINE('_OL_AWAY','Uzakta');
DEFINE('_OL_AVAILABLE','Yardim uyesi');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Görünen Kanallar %s-%s of %s');
DEFINE('_PS_USERSHOW','Görünen Kullanicilar %s-%s of %s');
DEFINE('_PS_ASCENDING','Artan');
DEFINE('_PS_DESCENDING','Azalan');

/* Errors */
DEFINE('_ER_ERROR','HATA');
DEFINE('_ER_WARNING','UYARI');
DEFINE('_ER_INFO','BILGI');
DEFINE('_ER_NODATA','Kayit Yok');
DEFINE('_ER_NOINVITES','Davet Yok');
DEFINE('_ER_NOBANS','Yasak Yok');
DEFINE('_ER_NOEXCEPTS','Ayricalik Yok');
DEFINE('_ER_NORESULTS','Cevap Yok');
DEFINE('_ER_MINCHARS','You need at least %s chars to search');
DEFINE('_ER_MODE','The requested operation mode %s is unknown');
DEFINE('_ER_NOSTATS','No statistical data collected for this user/channel');
DEFINE('_ER_NOCHAN','Kanal kullanimda degil');
DEFINE('_ER_EMPTYCHAN','Kanal su an bos');
DEFINE('_ER_DENIED','Access Denied'); // TRANSLATE!

/* API */
DEFINE('_DA_CURRENTUSR','Su anki kullanicilar');
DEFINE('_DA_MAXUSR','En fazla kullanici');
DEFINE('_DA_SERVER','Sunucu ismi');
DEFINE('_DA_SRVDET1','Running %s Up since %s');
DEFINE('_DA_SRVDET2','Su an %s');
DEFINE('_DA_SRVDET3','since %s');
DEFINE('_DA_SRVDET4','Son ayrilma: %s');
DEFINE('_DA_SRVDET5','Su anki Ping: %s  Max: %s on %s');
DEFINE('_DA_SRVDET6','Su anki kullanicilar: %s  Max: %s on %s');
DEFINE('_DA_SRVDET7','Su anki Operatorler: %s  Max: %s on %s');
DEFINE('_DA_SRVDET8','Ircop Banlari: %s  Sunucu Banlari: %s');
DEFINE('_DA_UPTIME','%s days and %s hours');

/* Graphs */
DEFINE('_GR_CUSTOM','Custom Graphs');
DEFINE('_GR_SELECT','Seçim...');
DEFINE('_GR_GENERATE','Olusturma');
DEFINE('_GR_SDATE','Baslangiç Tarihi');
DEFINE('_GR_EDATE','Bitis Tarihi');
DEFINE('_GR_OPTIONS','Seçenekler');
DEFINE('_GR_MIN','En az');
DEFINE('_GR_MAX','En fazla');
DEFINE('_GR_AVG','Ortalama');

/* Other */
DEFINE('_OT_GENTIME','Page generated in %s seconds using %s sql queries'); // TRANSLATE!
DEFINE('_OT_CGENTIME','Compressed page generated in %s seconds using %s sql queries'); // TRANSLATE!

?>