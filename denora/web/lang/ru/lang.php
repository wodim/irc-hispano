<?php
/* Russian Langfile for phpDenora 1.3.0

 Author:	lawer		<lawer@maglan.ru>
 Updates:	Bbb		<bbb@maglan.ru>
 Updates:	Kein	<kein-of@yandex.ru>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'ru_RU.UTF-8', 'ru.UTF-8', 'rus.UTF-8', 'ru_RU', 'ru', 'rus');
$charset = "utf-8";
$denora_format_time = "%A %d %B %Y, %I:%M %p %Z";
$denora_format_short = "%D %T";

/* Top Menu */
DEFINE('_TM_HOME','Главная');
DEFINE('_TM_NETWORK','IRC Сеть');
DEFINE('_TM_GRAPHS','Графики');
DEFINE('_TM_CHANS','Каналы');
DEFINE('_TM_USERS','Пользователи');
DEFINE('_TM_SEARCH','Поиск');
DEFINE('_TM_CONFIG','Настройки');
DEFINE('_TM_TITLE','IRC статистика');

/* Side Menus */
DEFINE('_SM_NETSTATUS','IRC сеть');
DEFINE('_SM_NETDETAILS','Детали');
DEFINE('_SM_SERVERS','Серверы');
DEFINE('_SM_SRVLIST','Список серверов');
DEFINE('_SM_SRVDETAILS','Подробности сервера');
DEFINE('_SM_MOTD','Сообщение дня (MOTD)');
DEFINE('_SM_NETSTATS','IRC статистика');
DEFINE('_SM_CUSTOM','Выборочно');
DEFINE('_SM_NETSPLIT','NetSplit.de графики');
DEFINE('_SM_SEARCHIRC','searchirc.com графики');
DEFINE('_SM_CHANLIST','Список каналов');
DEFINE('_SM_TOPCHANS','Статистика каналов');
DEFINE('_SM_SEARCHCHAN','Поиск канала');
DEFINE('_SM_CHANNELSTATUS','Статус канала');
DEFINE('_SM_CHANNELACT','Активность канала');
DEFINE('_SM_WHOIN','Кто в сети');
DEFINE('_SM_TOPUSERS','Статистика пользователя');
DEFINE('_SM_SEARCHUSER','Поиск пользователя');
DEFINE('_SM_GLOBALSTATS','Вся статистика');
DEFINE('_SM_CHANSTATS','Установки канала');
DEFINE('_SM_CHANINFO','Информация канала');
DEFINE('_SM_TLDPIE','Статистика по странам');
DEFINE('_SM_CLIENTPIE','Статистика пользователя');
DEFINE('_SM_INVITES','Приглашений на канал');
DEFINE('_SM_BANS','Баны на каналах');
DEFINE('_SM_EXCEPTS','Исключения канала');
DEFINE('_SM_TSTATS','Всего');
DEFINE('_SM_MSTATS','За месяц');
DEFINE('_SM_WSTATS','За неделю');
DEFINE('_SM_DSTATS','Сегодня');
DEFINE('_SM_ASTATS','Daily Average');
DEFINE('_SM_LANGUAGE','Язык');
DEFINE('_SM_THEME','Скин');
DEFINE('_SM_SET','Применить');
DEFINE('_SM_BEHAVIOR','Предпочтения');
DEFINE('_SM_OPERS','Операторы');
DEFINE('_SM_PYSTATS','За прошлый год');
DEFINE('_SM_PMSTATS','За прошлый месяц');
DEFINE('_SM_PWSTATS','За прошлую неделю');
DEFINE('_SM_NSWEEKS','Последние 2 недели');
DEFINE('_SM_NSMONTHS','Последние 2 months');
DEFINE('_SM_NSYEARS','Последние 2 года');
DEFINE('_SM_NSHISTORY','Complete History');

/* Front Page */
DEFINE('_FP_RANKING','Текущий ранг');
DEFINE('_FP_BIGGESTCHANS','%s самых Посещаемых Каналов Сейчас');
DEFINE('_FP_TOPCHANS','Топ %s Самых Общительных Каналов');
DEFINE('_FP_TOPUSERS','Топ %s Активных Пользователей Сегодня');
DEFINE('_FP_STATUS','Статус IRC Сети');
DEFINE('_FP_MAXSTATS','Max Stats');
DEFINE('_FP_USERS','Пользователей:');
DEFINE('_FP_CHANS','Каналов:');
DEFINE('_FP_OPERS','Операторов:');
DEFINE('_FP_SERVERS','Серверов:');
DEFINE('_FP_MAX','Max:');
DEFINE('_FP_QNAVI','Quick Navigation');
DEFINE('_FP_QSTATS','Статистика');
DEFINE('_FP_LSTATUS','Текущий статус');
DEFINE('_FP_QUSER','Пользователь');
DEFINE('_FP_QCHAN','Канал');
DEFINE('_FP_GO','Вперед!');

/* Titles on Pages */
DEFINE('_PT_LUSERS','Статус IRC Сети');
DEFINE('_PT_SERVERS','Список Серверов');
DEFINE('_PT_DETAILS','Информация по серверу %s');
DEFINE('_PT_MOTD','Сообщение Дня');
DEFINE('_PT_CHANLIST','Список каналов (/list)');
DEFINE('_PT_TCSTATS','Активность каналов');
DEFINE('_PT_CHANNELACT','Average Channel Activity');
DEFINE('_PT_WHOIN','Currently on %s');
DEFINE('_PT_SEARCHCHAN','Поиск каналов');
DEFINE('_PT_USERACT','Средняя активность пользователя');
DEFINE('_PT_SEARCHUSER','User Search');
DEFINE('_PT_TSTATS','Активность пользователей');
DEFINE('_PT_STATSFOR','Статистика %s в %s');
DEFINE('_PT_TLDPIE','Статистика по странам в %s');
DEFINE('_PT_CLIENTPIE','Статистика по используемым клиентам в %s');
DEFINE('_PT_MSTATS','в этом месяце');
DEFINE('_PT_WSTATS','на этой неделе');
DEFINE('_PT_DSTATS','сегодня');
DEFINE('_PT_SEARCHRESULTS','Результаты поиска');
DEFINE('_PT_OPERS','Список операторов');
DEFINE('_PT_NSU2W','Relation of users and channels during the last 2 weeks');
DEFINE('_PT_NSC2W','Channels during the last 2 weeks');
DEFINE('_PT_NSS2W','Servers during the last 2 weeks');
DEFINE('_PT_NSU2M','Relation of users and channels during the last 2 months');
DEFINE('_PT_NSC2M','Channels during the last 2 months');
DEFINE('_PT_NSS2M','Servers during the last 2 months');
DEFINE('_PT_NSU2Y','Relation of users and channels during the last 2 years');
DEFINE('_PT_NSC2Y','Channels during the last 2 years');
DEFINE('_PT_NSS2Y','Servers during the last 2 years');
DEFINE('_PT_NSHYS','Complete measured history about users and channels');

/* Network Status */
DEFINE('_NS_USERDET','Пользователей: %s Макс. сегодня: %s Макс. всего: %s было %s');
DEFINE('_NS_CHANDET','Каналов: %s Максимум: %s было %s');
DEFINE('_NS_OPERDET','Операторов: %s Максимум: %s было %s');
DEFINE('_NS_SERVDET','Серверов: %s Максимум: %s было %s');

/* Search */
DEFINE('_SP_PROMPT','Введите критерий поиска.');
DEFINE('_SP_SUBMIT','Поиск');
DEFINE('_SP_CHANINFO','Channel Info');
DEFINE('_SP_CHANSTATS','Channel Stats');
DEFINE('_SP_USERINFO','User Info');
DEFINE('_SP_USERSTATS','User Stats');

/* Configuration Page */
DEFINE('_CP_SETSORT','Критерий поиска по умолчанию');
DEFINE('_CP_SETTYPE','По умолчанию статистика');

/* General Definitions */
DEFINE('_GD_NUMBER','№.');
DEFINE('_GD_NICK','ник');
DEFINE('_GD_HOST','hostname');
DEFINE('_GD_CHANNEL','канал');
DEFINE('_GD_USERS','users');
DEFINE('_GD_LETTERS','букв');
DEFINE('_GD_WORDS','слов');
DEFINE('_GD_LINES','строк');
DEFINE('_GD_ACTIONS','действий');
DEFINE('_GD_SMILEYS','смайлов');
DEFINE('_GD_KICKS','киков');
DEFINE('_GD_MODES','режимов');
DEFINE('_GD_TOPICS','топиков');
DEFINE('_GD_ON','в');
DEFINE('_GD_TYPE','type');
DEFINE('_GD_TOTAL','total');
DEFINE('_GD_OTHER','other');
DEFINE('_GD_UNKNOWN','unknown');
DEFINE('_GD_NEVER','never');
DEFINE('_GD_NONE','none');
DEFINE('_GD_STATUS','Статус');
DEFINE('_GD_YES','Yes');
DEFINE('_GD_NO','No');
DEFINE('_GD_HOUR','Hour');
DEFINE('_GD_DAY','Day');
DEFINE('_GD_MONTH','Month');
DEFINE('_GD_YEAR','Year');
DEFINE('_GD_OR','или');
DEFINE('_GD_AND','и');

/* Channel Status */
DEFINE('_CS_INFO0','Channel: %s');
DEFINE('_CS_INFO1','Topic: %s');
DEFINE('_CS_INFO2','Set by: %s On: %s');
DEFINE('_CS_INFO3','Current users: %s Peak: %s on %s');
DEFINE('_CS_INFO4','Channel modes: %s');
DEFINE('_CS_INFO5','Invite Exceptions: %s Bans: %s Ban Exceptions: %s');

/* User Info */
DEFINE('_UI_UINFO','Информация о пользователе %s (stats group %s)');
DEFINE('_UI_NODATA','The user %s appears to be %s and there is no information cached.');
DEFINE('_UI_UCHANS','Monitored channels: %s');
DEFINE('_UI_WHOIS1','Ник: %s Имя: %s');
DEFINE('_UI_WHOIS2','Маска: %s');
DEFINE('_UI_WHOIS3','Статус: %s');
DEFINE('_UI_WHOIS4','Подключился: %s');
DEFINE('_UI_WHOIS5','Покинул: %s');
DEFINE('_UI_WHOIS6','Away статус: %s');
DEFINE('_UI_WHOIS7','Message: %s');
DEFINE('_UI_WHOIS8','Использует клиент: %s');
DEFINE('_UI_WHOIS9','Подключен к: %s на сервер: %s');
DEFINE('_UI_WHOIS10','Additional Information: %s');

/* Oper list */
DEFINE('_OL_NICK','Ник');
DEFINE('_OL_LEVEL','Уроверь');
DEFINE('_OL_ONLINE','В online с');
DEFINE('_OL_LASTQUIT','Покинул');
DEFINE('_OL_BOT','Bot Service');
DEFINE('_OL_SERVICE','Network Service');
DEFINE('_OL_AWAY','Away');
DEFINE('_OL_AVAILABLE','Available for Help');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Показаны каналы %s-%s из %s');
DEFINE('_PS_USERSHOW','Показаны пользователи %s-%s из %s');
DEFINE('_PS_ASCENDING','Ascending');
DEFINE('_PS_DESCENDING','Descending');

/* Errors */
DEFINE('_ER_ERROR','ERROR');
DEFINE('_ER_WARNING','WARNING');
DEFINE('_ER_INFO','INFO');
DEFINE('_ER_NODATA','No Data');
DEFINE('_ER_NOINVITES','No Invite Exceptions');
DEFINE('_ER_NOBANS','No Bans');
DEFINE('_ER_NOEXCEPTS','No Ban Exceptions');
DEFINE('_ER_NORESULTS','No Results');
DEFINE('_ER_MINCHARS','Для поиска необходимо минимум %s символа');
DEFINE('_ER_MODE','The requested operation mode %s is unknown');
DEFINE('_ER_NOSTATS','No statistical data collected for this user/channel');
DEFINE('_ER_NOCHAN','This channel does not exist');
DEFINE('_ER_EMPTYCHAN','This channel is currently empty');
DEFINE('_ER_DENIED','Access Denied');

/* API */
DEFINE('_DA_CURRENTUSR','Пользователей');
DEFINE('_DA_MAXUSR','Максимум');
DEFINE('_DA_SERVER','Имя сервера');
DEFINE('_DA_SRVDET1','Используется %s Uptime %s');
DEFINE('_DA_SRVDET2','Текущий статус %s');
DEFINE('_DA_SRVDET3','с %s');
DEFINE('_DA_SRVDET4','Последний split: %s');
DEFINE('_DA_SRVDET5','Текущий пинг: %s  Максимальный: %s был %s');
DEFINE('_DA_SRVDET6','Текущее кол-во пользователей: %s  Максимально: %s было %s');
DEFINE('_DA_SRVDET7','Текущие операторы: %s  Максимально: %s было %s');
DEFINE('_DA_SRVDET8','"Ircop Kills": %s  "Server Kills": %s');
DEFINE('_DA_UPTIME','%s days and %s hours');

/* Graphs */
DEFINE('_GR_CUSTOM','Custom Graphs');
DEFINE('_GR_SELECT','Выбор...');
DEFINE('_GR_GENERATE','Создать');
DEFINE('_GR_SDATE','Начальная дата');
DEFINE('_GR_EDATE','Конечная дата');
DEFINE('_GR_OPTIONS','Опции');
DEFINE('_GR_MIN','Min');
DEFINE('_GR_MAX','Max');
DEFINE('_GR_AVG','Avg');

/* Other */
DEFINE('_OT_GENTIME','Page generated in %s seconds using %s sql queries');
DEFINE('_OT_CGENTIME','Compressed page generated in %s seconds using %s sql queries');

?>
