<?php

/* Português Brasileiro Langfile for phpDenora 1.3.0

$Id: lang.php 353 2009-03-04 08:51:01Z hal9000 $



Author:		drak		<drak@clubeirc.com>

*/

/* ensure this file is being included by a parent file */

defined( '_VALID_PARENT' ) or die( 'Direct Access to this location is not allowed.' );



setlocale(LC_ALL, 'pt_BR.UTF-8', 'pt_BR', 'pt.UTF-8', 'por.UTF-8', 'pt_PT', 'pt', 'por');

$charset = "utf-8";

$denora_format_time = "%A %d %B %Y, %H:%M %Z";

$denora_format_short = "%d/%m/%y %H:%M:%S";



/* Top Menu */

DEFINE('_TM_HOME','Início');

DEFINE('_TM_NETWORK','Rede');

DEFINE('_TM_GRAPHS','Gráficos');

DEFINE('_TM_CHANS','Canais');

DEFINE('_TM_USERS','Usuários');

DEFINE('_TM_SEARCH','Procurar');

DEFINE('_TM_CONFIG','Opções');

DEFINE('_TM_TITLE','Estatísticas do IRC');



/* Side Menus */

DEFINE('_SM_NETSTATUS','Estado da Rede');

DEFINE('_SM_NETDETAILS','Detalhes');

DEFINE('_SM_SERVERS','Servidores');

DEFINE('_SM_SRVLIST','Lista dos Servidores');

DEFINE('_SM_SRVDETAILS','Detalhes do Servidor');

DEFINE('_SM_MOTD','MOTD');

DEFINE('_SM_NETSTATS','Gráficos da Rede');

DEFINE('_SM_CUSTOM','Personalizar..');

DEFINE('_SM_NETSPLIT','Gráficos NetSplit.de');

DEFINE('_SM_SEARCHIRC','Gráficos SearchIRC');

DEFINE('_SM_CHANLIST','Lista de Canais');

DEFINE('_SM_TOPCHANS','Estatísticas Canais');

DEFINE('_SM_SEARCHCHAN','Procurar Canal..');

DEFINE('_SM_CHANNELSTATUS','Estado do Canal');

DEFINE('_SM_CHANNELACT','Atividade do Canal');

DEFINE('_SM_WHOIN','Quem está em linha');

DEFINE('_SM_TOPUSERS','Estatísticas dos usuários');

DEFINE('_SM_SEARCHUSER','Procurar Usuário..');

DEFINE('_SM_GLOBALSTATS','Estatísticas Globais');

DEFINE('_SM_CHANSTATS','Estatísticas de Canais');

DEFINE('_SM_CHANINFO','Informação do Canal');

DEFINE('_SM_TLDPIE','Estatísticas dos Países');

DEFINE('_SM_CLIENTPIE','Estatísticas de usuários');

DEFINE('_SM_INVITES','Exceções Convites'); 

DEFINE('_SM_BANS','Expulsões');

DEFINE('_SM_EXCEPTS','Exceções Expulsões');

DEFINE('_SM_TSTATS','Total'); 

DEFINE('_SM_MSTATS','Este Mês');

DEFINE('_SM_WSTATS','Esta Semana');

DEFINE('_SM_DSTATS','Hoje');

DEFINE('_SM_ASTATS','Média Diária');

DEFINE('_SM_LANGUAGE','Idioma');

DEFINE('_SM_THEME','Tema');

DEFINE('_SM_SET','Mudar');

DEFINE('_SM_BEHAVIOR','Comportamento');

DEFINE('_SM_OPERS','IRCops');

DEFINE('_SM_PYSTATS','Ano Passado');

DEFINE('_SM_PMSTATS','Mês Passado');

DEFINE('_SM_PWSTATS','Semana Passada');

DEFINE('_SM_NSWEEKS','Últimas duas semanas');

DEFINE('_SM_NSMONTHS','Últimos dois meses');

DEFINE('_SM_NSYEARS','Últimos dois anos');

DEFINE('_SM_NSHISTORY','Histórico completo');



/* Front Page */

DEFINE('_FP_RANKING','Ranking atual:');

DEFINE('_FP_BIGGESTCHANS','Top %s dos canais mais ativo');

DEFINE('_FP_TOPCHANS','Top %s dos canais de hoje');

DEFINE('_FP_TOPUSERS','Top %s Usuários de hoje');

DEFINE('_FP_STATUS','Estado da Rede');

DEFINE('_FP_MAXSTATS','Máx Estatísticas');

DEFINE('_FP_USERS','Usuários:');

DEFINE('_FP_CHANS','Canais:');

DEFINE('_FP_OPERS','IRCops:');

DEFINE('_FP_SERVERS','Servidores:');

DEFINE('_FP_MAX','Máx:');

DEFINE('_FP_QNAVI','Navegação Rápida');

DEFINE('_FP_QSTATS','Estatísticas rápidas');

DEFINE('_FP_LSTATUS','Estado Atual');

DEFINE('_FP_QUSER','Usuário');

DEFINE('_FP_QCHAN','Canal');

DEFINE('_FP_GO','Ir');





/* Titles on Pages */

DEFINE('_PT_LUSERS','Estado da Rede');

DEFINE('_PT_SERVERS','Lista dos Servidores');

DEFINE('_PT_DETAILS','Detalhes do Servidor para %s');

DEFINE('_PT_MOTD','Mensagem do Dia(MOTD)');

DEFINE('_PT_CHANLIST','Lista de Canais (/list)');

DEFINE('_PT_TCSTATS','Atividade de Canais');

DEFINE('_PT_CHANNELACT','Quantidade de Atividade do Canal');

DEFINE('_PT_WHOIN','Atualmente em %s');

DEFINE('_PT_SEARCHCHAN','Procurar Canal');

DEFINE('_PT_USERACT','Quantidade de Atividade do Usuário');

DEFINE('_PT_SEARCHUSER','Procurar Usuário');

DEFINE('_PT_TSTATS','Atividade do Usuário');

DEFINE('_PT_STATSFOR','Estatísticas para %s desde o %s');

DEFINE('_PT_TLDPIE','Estatísticas dos Países para %s');

DEFINE('_PT_CLIENTPIE','Estatísticas dos usuários para %s');

DEFINE('_PT_MSTATS','este mês');

DEFINE('_PT_WSTATS','esta semana');

DEFINE('_PT_DSTATS','hoje');

DEFINE('_PT_SEARCHRESULTS','Resultado da pesquisa');

DEFINE('_PT_OPERS','IRCops atuais');

DEFINE('_PT_NSU2W','Relação de usuários e canais durante as últimas duas semanas');

DEFINE('_PT_NSC2W','Canais durante as últimas duas semanas');

DEFINE('_PT_NSS2W','Servidores durante as duas últimas semanas');

DEFINE('_PT_NSU2M','Relação de usuários e canais durante os últimos dois meses');

DEFINE('_PT_NSC2M','Canais durante os últimos dois meses');

DEFINE('_PT_NSS2M','Servidores durante as dois últimos meses');

DEFINE('_PT_NSU2Y','Relação de usuários e canais durante os últimos dois anos');

DEFINE('_PT_NSC2Y','Canais durante os dois últimos anos');

DEFINE('_PT_NSS2Y','Servidores durante os dois últimos anos');

DEFINE('_PT_NSHYS','Histórico completo sobre usuários e canais');



/* Network Status */

DEFINE('_NS_USERDET','Usuários atuais: %s Máx hoje: %s Máx: %s desde o %s');

DEFINE('_NS_CHANDET','Canais atuais: %s Máx: %s desde o %s');

DEFINE('_NS_OPERDET','IRCops atuais: %s Máx: %s desde o %s');

DEFINE('_NS_SERVDET','Servidores atuais: %s Máx: %s desde o %s');



/* Search */

DEFINE('_SP_PROMPT','Insira o critério de pesquisa.');

DEFINE('_SP_SUBMIT','procurar');

DEFINE('_SP_CHANINFO','Info Canal');

DEFINE('_SP_CHANSTATS','Stats Canal');

DEFINE('_SP_USERINFO','Info Usuário');

DEFINE('_SP_USERSTATS','Stats Usuário');

/* Configuration Page */

DEFINE('_CP_SETSORT','Fixe o critério da pesquisa');

DEFINE('_CP_SETTYPE','Fixe o tipo da Estatísticas');



/* General Definitions */

DEFINE('_GD_NUMBER','Nº ');

DEFINE('_GD_NICK',' Nick');

DEFINE('_GD_HOST','Host');

DEFINE('_GD_CHANNEL',' Canal');

DEFINE('_GD_USERS','Usuários');

DEFINE('_GD_LETTERS','Letras');

DEFINE('_GD_WORDS','Palavras');

DEFINE('_GD_LINES','Linhas');

DEFINE('_GD_ACTIONS','Ações');

DEFINE('_GD_SMILEYS','Smileys');

DEFINE('_GD_KICKS','Expulsões');

DEFINE('_GD_MODES','Modos');

DEFINE('_GD_TOPICS','Tópicos');

DEFINE('_GD_ON','em');

DEFINE('_GD_TYPE','Tipo');

DEFINE('_GD_TOTAL','Total');

DEFINE('_GD_OTHER','Outros');

DEFINE('_GD_UNKNOWN','Desconhecido');

DEFINE('_GD_NEVER','Nunca');

DEFINE('_GD_NONE','Nenhum'); 

DEFINE('_GD_STATUS','Estado'); 

DEFINE('_GD_YES','Sim');

DEFINE('_GD_NO','Não');

DEFINE('_GD_HOUR','Hora');

DEFINE('_GD_DAY','Dia');

DEFINE('_GD_MONTH','Mês');

DEFINE('_GD_YEAR','Ano');

DEFINE('_GD_OR','o');

DEFINE('_GD_AND','e');



/* Channel Status */

DEFINE('_CS_INFO0','Canal: %s');

DEFINE('_CS_INFO1','Tópico: %s'); 

DEFINE('_CS_INFO2','Tópico colocado por: %s no dia %s'); 

DEFINE('_CS_INFO3','Usuários atuais: %s Máx: %s desde %s'); 

DEFINE('_CS_INFO4','Modos do canal: %s'); 

DEFINE('_CS_INFO5','Exceções Convites: %s Expulsões: %s Exceções Expulsões: %s'); 



/* User Info */

DEFINE('_UI_UINFO','Informação do nickname %s (grupo stats %s)');

DEFINE('_UI_NODATA','O usuário %s parece ser %s e não há ocultação da informação.');

DEFINE('_UI_UCHANS','Canais: %s');

DEFINE('_UI_WHOIS1','Nick %s Real Name: %s');

DEFINE('_UI_WHOIS2','Máscara: %s');

DEFINE('_UI_WHOIS3','Estado: %s');

DEFINE('_UI_WHOIS4','Conectado Desde: %s');

DEFINE('_UI_WHOIS5','O Último Quit: %s');

DEFINE('_UI_WHOIS6','Mensagem do Ausente: %s');

DEFINE('_UI_WHOIS7','Mensagem: %s');

DEFINE('_UI_WHOIS8','Cliente Usado: %s');

DEFINE('_UI_WHOIS9','Conectado desde: %s Servidor: %s');

DEFINE('_UI_WHOIS10','Informação adicional: %s');



/* Oper list */

DEFINE('_OL_NICK','Nick');

DEFINE('_OL_LEVEL','Status');

DEFINE('_OL_ONLINE','Online desde:');

DEFINE('_OL_LASTQUIT','Última Mensagem de saída:');

DEFINE('_OL_BOT','Serviço Bot');

DEFINE('_OL_SERVICE','Serviço da Rede');

DEFINE('_OL_AWAY','Ausente');

DEFINE('_OL_AVAILABLE','Disponível para ajuda');



/* Page Sorting */

DEFINE('_PS_CHANSHOW','Mostrar Canais %s-%s de %s'); 

DEFINE('_PS_USERSHOW','Mostrar utentes %s-%s de %s'); 

DEFINE('_PS_ASCENDING','Ascendente'); 

DEFINE('_PS_DESCENDING','Descendente');

/* Errors */

DEFINE('_ER_ERROR','ERROR');

DEFINE('_ER_WARNING','WARNING');

DEFINE('_ER_INFO','INFO');

DEFINE('_ER_NODATA','Nenhum dado');

DEFINE('_ER_NOINVITES','Nenhuma excepçao invitaçao');

DEFINE('_ER_NOBANS','Nenhuma expulsao'); 

DEFINE('_ER_NOEXCEPTS','Nenhuma excepçao expulsao'); 

DEFINE('_ER_NORESULTS','Nenhum resultado');

DEFINE('_ER_MINCHARS','Tu precisas como minimo %s caracteres para procurar');

DEFINE('_ER_MODE','A operaçao do modo requerido %s e desconhocida');

DEFINE('_ER_NOSTATS','Nao ha dados estadisticos recolectados para este utente/canal');

DEFINE('_ER_NOCHAN','Este canal nao existe');

DEFINE('_ER_EMPTYCHAN','Este canal esta actualmente vacio');

DEFINE('_ER_DENIED','Accesso denegado');



/* API */

DEFINE('_DA_CURRENTUSR','Utentes Actuais');

DEFINE('_DA_MAXUSR','Máx Utentes');

DEFINE('_DA_SERVER','Nome do Servidor');

DEFINE('_DA_SRVDET1','Usar %s Uptime: %s');

DEFINE('_DA_SRVDET2','Actualmente %s');

DEFINE('_DA_SRVDET3','desde o %s');

DEFINE('_DA_SRVDET4','Ultimo split: %s');

DEFINE('_DA_SRVDET5','Ping actual: %s  Máx: %s desde o %s');

DEFINE('_DA_SRVDET6','Utentes Actuais: %s Máx: %s desde o %s');

DEFINE('_DA_SRVDET7','Operadores Actuais: %s Máx: %s desde o %s');

DEFINE('_DA_SRVDET8','Kills dos IRCops: %s - Kills do Servidor: %s');

DEFINE('_DA_UPTIME','%s dias e %s horas');



/* Graphs */

DEFINE('_GR_CUSTOM','Graficos Personalizados');

DEFINE('_GR_SELECT','Seleccionar...');

DEFINE('_GR_GENERATE','Generar');

DEFINE('_GR_SDATE','Data do Inicio');

DEFINE('_GR_EDATE','Data do Fim');

DEFINE('_GR_OPTIONS','Opçoes');

DEFINE('_GR_MIN','Mínimo');

DEFINE('_GR_MAX','Máximo');

DEFINE('_GR_AVG','Promédio');



/* Other */

DEFINE('_OT_GENTIME','Pagina generada em %s segundos a usar %s consultas SQL');

DEFINE('_OT_CGENTIME','Pagina comprimida generada em %s segundos a usar %s consultas SQL');



?>

