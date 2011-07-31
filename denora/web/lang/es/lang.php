<?php
/* Spanish Langfile for phpDenora 1.3.0
$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $

 Author:	DrStein		<an-drstein@dezeserver.nl>
Updates:	Alfred		<alfred.dunhill@gmail.com>
Updates:	Alvaro		<alvaro@xateros.com>
Updates:	Obi_Wan		<webmaster@st-city.net>
Updates:	MaD			<mad@madito.net>
*/
/* ensure this file is being included by a parent file */
defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );

setlocale(LC_ALL, 'es_ES.UTF-8@euro', 'es_ES.UTF-8', 'es.UTF-8', 'spa.UTF-8', 'es_ES@euro', 'es_ES', 'es', 'spa');
$charset = "utf-8";
$denora_format_time = "%H:%M del %A %d de %B del %Y.";
$denora_format_short = "%d/%m/%y %H:%M:%S";

/* Top Menu */
DEFINE('_TM_HOME','Inicio');
DEFINE('_TM_NETWORK','Red');
DEFINE('_TM_GRAPHS','Graficos');
DEFINE('_TM_CHANS','Canales');
DEFINE('_TM_USERS','Usuarios');
DEFINE('_TM_SEARCH','Buscar');
DEFINE('_TM_CONFIG','Opciones');
DEFINE('_TM_TITLE','Estadisticas de IRC');

/* Side Menus */
DEFINE('_SM_NETSTATUS','Estado de la Red');
DEFINE('_SM_NETDETAILS','Detalles');
DEFINE('_SM_SERVERS','Servidores');
DEFINE('_SM_SRVLIST','Lista de Servidores');
DEFINE('_SM_SRVDETAILS','Detalles del Servidor');
DEFINE('_SM_MOTD','MOTD');
DEFINE('_SM_NETSTATS','Graficos de la Red');
DEFINE('_SM_CUSTOM','Personalizar...');
DEFINE('_SM_NETSPLIT','Graficos NetSplit.de');
DEFINE('_SM_SEARCHIRC','Graficos SearchIRC');
DEFINE('_SM_CHANLIST','Lista de Canales');
DEFINE('_SM_TOPCHANS','Estadisticas Canales');
DEFINE('_SM_SEARCHCHAN','Buscar Canal...');
DEFINE('_SM_CHANNELSTATUS','Estado del Canal');
DEFINE('_SM_CHANNELACT','Actividad del Canal');
DEFINE('_SM_WHOIN','Quien esta en linea');
DEFINE('_SM_TOPUSERS','Estadisticas Usuarios');
DEFINE('_SM_SEARCHUSER','Buscar Usuario...');
DEFINE('_SM_GLOBALSTATS','Estadisticas Globales');
DEFINE('_SM_CHANSTATS','Estadisticas Canales');
DEFINE('_SM_CHANINFO','Informacion del Canal');
DEFINE('_SM_TLDPIE','Estadisticas Paises');
DEFINE('_SM_CLIENTPIE','Estadisticas Clientes');
DEFINE('_SM_INVITES','Excepciones de Invitacion'); 
DEFINE('_SM_BANS','Bans');
DEFINE('_SM_EXCEPTS','Excepciones de Bans');
DEFINE('_SM_TSTATS','Total'); 
DEFINE('_SM_MSTATS','Este Mes');
DEFINE('_SM_WSTATS','Esta Semana');
DEFINE('_SM_DSTATS','Hoy');
DEFINE('_SM_ASTATS','Media Diaria');
DEFINE('_SM_LANGUAGE','Idioma');
DEFINE('_SM_THEME','Tema');
DEFINE('_SM_SET','Cambiar');
DEFINE('_SM_BEHAVIOR','Comportamiento');
DEFINE('_SM_OPERS','IRCops');
DEFINE('_SM_PYSTATS','Año Pasado');
DEFINE('_SM_PMSTATS','Mes Pasado');
DEFINE('_SM_PWSTATS','Semana Pasada');
DEFINE('_SM_NSWEEKS','Ultimas dos semanas');
DEFINE('_SM_NSMONTHS','Ultimos dos meses');
DEFINE('_SM_NSYEARS','Ultimos dos años');
DEFINE('_SM_NSHISTORY','Historia completa');

/* Front Page */
DEFINE('_FP_RANKING','Graduación actual');
DEFINE('_FP_BIGGESTCHANS','Top %s Canales Usuarios');
DEFINE('_FP_TOPCHANS','Top %s Canales Hoy');
DEFINE('_FP_TOPUSERS','Top %s Usuarios Hoy');
DEFINE('_FP_STATUS','Estado de la Red');
DEFINE('_FP_MAXSTATS','Max Estadisticas');
DEFINE('_FP_USERS','Usuarios:');
DEFINE('_FP_CHANS','Canales:');
DEFINE('_FP_OPERS','IRCops:');
DEFINE('_FP_SERVERS','Servidores:');
DEFINE('_FP_MAX','Máx:');
DEFINE('_FP_QNAVI','Navegacion Rapida');
DEFINE('_FP_QSTATS','Estadisticas rápidas');
DEFINE('_FP_LSTATUS','Estado Actual');
DEFINE('_FP_QUSER','Usuario');
DEFINE('_FP_QCHAN','Canal');
DEFINE('_FP_GO','Vamos');


/* Titles on Pages */
DEFINE('_PT_LUSERS','Estado de la Red');
DEFINE('_PT_SERVERS','Lista de Servidores');
DEFINE('_PT_DETAILS','Detalles del Servidor para %s');
DEFINE('_PT_MOTD','Mensaje del Dia (MOTD)');
DEFINE('_PT_CHANLIST','Lista de Canales (/list)');
DEFINE('_PT_TCSTATS','Actividad de Canales');
DEFINE('_PT_CHANNELACT','Cantidad de Actividad del Canal');
DEFINE('_PT_WHOIN','Actualmente en %s');
DEFINE('_PT_SEARCHCHAN','Buscar Canal');
DEFINE('_PT_USERACT','Cantidad de Actividad del Usuario');
DEFINE('_PT_SEARCHUSER','Buscar Usuario');
DEFINE('_PT_TSTATS','Actividad del Usuario');
DEFINE('_PT_STATSFOR','Estadisticas para %s desde el %s');
DEFINE('_PT_TLDPIE','Estadisticas de Paises para %s');
DEFINE('_PT_CLIENTPIE','Estadisticas de Clientes para %s');
DEFINE('_PT_MSTATS','este mes');
DEFINE('_PT_WSTATS','esta semana');
DEFINE('_PT_DSTATS','hoy');
DEFINE('_PT_SEARCHRESULTS','Resultado de Busqueda');
DEFINE('_PT_OPERS','IRCops actuales');
DEFINE('_PT_NSU2W','Relacion de usarios y canales durante las ultimas dos semanas');
DEFINE('_PT_NSC2W','Canales durante las ultimas dos semanas');
DEFINE('_PT_NSS2W','Servidores durante las dos ultimas semanas');
DEFINE('_PT_NSU2M','Relacion de usarios y canales durante las ultimos dos meses');
DEFINE('_PT_NSC2M','Canales durante las ultimos dos meses');
DEFINE('_PT_NSS2M','Servidores durante las dos ultimos meses');
DEFINE('_PT_NSU2Y','Relacion de usarios y canales durante los ultimos dos años');
DEFINE('_PT_NSC2Y','Canales durante los dos ultimos años');
DEFINE('_PT_NSS2Y','Servidores durante los dos ultimos años');
DEFINE('_PT_NSHYS','Completa historia mesurada sobre usarios y canales');

/* Network Status */
DEFINE('_NS_USERDET','Usuarios actuales: %s Máx hoy: %s Máx: %s desde las %s');
DEFINE('_NS_CHANDET','Canales actuales: %s Máx: %s desde las %s');
DEFINE('_NS_OPERDET','IRCops actuales: %s Máx: %s desde las %s');
DEFINE('_NS_SERVDET','Servidores actuales: %s Máx: %s desde las %s');

/* Search */
DEFINE('_SP_PROMPT','Ingrese el criterio de busqueda, los comodines tambien son aceptados.');
DEFINE('_SP_SUBMIT','Buscar');
DEFINE('_SP_CHANINFO','Info Canal');
DEFINE('_SP_CHANSTATS','Stats Canal');
DEFINE('_SP_USERINFO','Info Usuario');
DEFINE('_SP_USERSTATS','Stats Usuario');
/* Configuration Page */
DEFINE('_CP_SETSORT','Fije los criterios que clasifican del defecto');
DEFINE('_CP_SETTYPE','Fije el tipo de la estadística del defecto');

/* General Definitions */
DEFINE('_GD_NUMBER','Nº ');
DEFINE('_GD_NICK',' Nick');
DEFINE('_GD_HOST','Host');
DEFINE('_GD_CHANNEL',' Canal');
DEFINE('_GD_USERS','Usuarios');
DEFINE('_GD_LETTERS','Letras');
DEFINE('_GD_WORDS','Palabras');
DEFINE('_GD_LINES','Lineas');
DEFINE('_GD_ACTIONS','Acciones');
DEFINE('_GD_SMILEYS','Smileys');
DEFINE('_GD_KICKS','Expulsiones');
DEFINE('_GD_MODES','Modos');
DEFINE('_GD_TOPICS','Topics');
DEFINE('_GD_ON','en');
DEFINE('_GD_TYPE','Tipo');
DEFINE('_GD_TOTAL','Total');
DEFINE('_GD_OTHER','Otros');
DEFINE('_GD_UNKNOWN','Desconocido');
DEFINE('_GD_NEVER','Nunca');
DEFINE('_GD_NONE','Ninguno'); 
DEFINE('_GD_STATUS','Estado'); 
DEFINE('_GD_YES','Si');
DEFINE('_GD_NO','No');
DEFINE('_GD_HOUR','Hora');
DEFINE('_GD_DAY','Dia');
DEFINE('_GD_MONTH','Mes');
DEFINE('_GD_YEAR','Año');
DEFINE('_GD_OR','o');
DEFINE('_GD_AND','y');

/* Channel Status */
DEFINE('_CS_INFO0','Canal: %s');
DEFINE('_CS_INFO1','Topic: %s'); 
DEFINE('_CS_INFO2','Topic fijado por: %s el dia %s'); 
DEFINE('_CS_INFO3','Usuarios actuales: %s Máx: %s desde el %s'); 
DEFINE('_CS_INFO4','Modos del canal: %s'); 
DEFINE('_CS_INFO5','Excepciones de Invitacion: %s Bans: %s Excepciones de Ban: %s'); 

/* User Info */
DEFINE('_UI_UINFO','Información del nickname %s (usuario stats %s)');
DEFINE('_UI_NODATA','El usuario %s aparece ser %s y no hay escondrijo de la información.');
DEFINE('_UI_UCHANS','Canales: %s');
DEFINE('_UI_WHOIS1','Nick: %s Real Name: %s');
DEFINE('_UI_WHOIS2','Máscara: %s');
DEFINE('_UI_WHOIS3','Estado: %s');
DEFINE('_UI_WHOIS4','Conectado Desde: %s');
DEFINE('_UI_WHOIS5','El Último Quit: %s');
DEFINE('_UI_WHOIS6','Mensaje de Ausente: %s');
DEFINE('_UI_WHOIS7','Mensaje: %s');
DEFINE('_UI_WHOIS8','Cliente Usado: %s');
DEFINE('_UI_WHOIS9','Conectado desde: %s Servidor: %s');
DEFINE('_UI_WHOIS10','Informacion adicional: %s');

/* Oper list */
DEFINE('_OL_NICK','Nick');
DEFINE('_OL_LEVEL','Status');
DEFINE('_OL_ONLINE','Online desde:');
DEFINE('_OL_LASTQUIT','Ultimo Mensaje de Quit:');
DEFINE('_OL_BOT','Servicio Bot');
DEFINE('_OL_SERVICE','Servicio de Red');
DEFINE('_OL_AWAY','Ausente');
DEFINE('_OL_AVAILABLE','Disponible para ayuda');

/* Page Sorting */
DEFINE('_PS_CHANSHOW','Mostrando canales %s-%s de %s'); 
DEFINE('_PS_USERSHOW','Mostrando usuarios %s-%s de %s'); 
DEFINE('_PS_ASCENDING','Ascendente'); 
DEFINE('_PS_DESCENDING','Descendente');

/* Errors */
DEFINE('_ER_ERROR','ERROR');
DEFINE('_ER_WARNING','WARNING');
DEFINE('_ER_INFO','INFO');
DEFINE('_ER_NODATA','Ningun dato');
DEFINE('_ER_NOINVITES','Ninguna excepcion de invitacion');
DEFINE('_ER_NOBANS','Ningun ban'); 
DEFINE('_ER_NOEXCEPTS','Ninguna excepcion de ban'); 
DEFINE('_ER_NORESULTS','Ningun resultado');
DEFINE('_ER_MINCHARS','Tu necesitas al menos %s caracteres para buscar');
DEFINE('_ER_MODE','La operacion modo solicitado %s es desconocido');
DEFINE('_ER_NOSTATS','No hay datos estadisticos recolectados para este usuario/canal');
DEFINE('_ER_NOCHAN','Este canal no existe');
DEFINE('_ER_EMPTYCHAN','Este canal esta actualmente vacio');
DEFINE('_ER_DENIED','Acceso denegado');

/* API */
DEFINE('_DA_CURRENTUSR','Usuarios Actuales');
DEFINE('_DA_MAXUSR','Máx usuarios');
DEFINE('_DA_SERVER','Nombre del servidor');
DEFINE('_DA_SRVDET1','Usando %s Uptime: %s');
DEFINE('_DA_SRVDET2','Actualmente %s');
DEFINE('_DA_SRVDET3','desde el %s');
DEFINE('_DA_SRVDET4','Ultimo split: %s');
DEFINE('_DA_SRVDET5','Ping actual: %s  Máx: %s desde el %s');
DEFINE('_DA_SRVDET6','Usuarios Actuales: %s Máx: %s desde el %s');
DEFINE('_DA_SRVDET7','Operadores Actuales: %s Máx: %s desde el %s');
DEFINE('_DA_SRVDET8','Kills de IRCops: %s - Kills de Servidor: %s');
DEFINE('_DA_UPTIME','%s dias y %s horas');

/* Graphs */
DEFINE('_GR_CUSTOM','Graficos Personalizados');
DEFINE('_GR_SELECT','Seleccionar...');
DEFINE('_GR_GENERATE','Generar');
DEFINE('_GR_SDATE','Fecha de Inicio');
DEFINE('_GR_EDATE','Fecha de Fin');
DEFINE('_GR_OPTIONS','Opciones');
DEFINE('_GR_MIN','Mínimo');
DEFINE('_GR_MAX','Máximo');
DEFINE('_GR_AVG','Promedio');

/* Other */
DEFINE('_OT_GENTIME','Pagina generada en %s segundos usando %s consultas SQL');
DEFINE('_OT_CGENTIME','Pagina comprimida generada en %s segundos usando %s consultas SQL');

?>
