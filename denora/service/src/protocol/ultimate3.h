/* Ulimtate IRCD 3
 *
 * (c) 2004-2011 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id: ultimate3.h 386 2011-01-04 17:16:43Z hal9000 $
 *
 */

void ultimate3_cmd_nick(char *nick, char *name, const char *modes);
void ultimate3_cmd_mode(char *source, char *dest, char *buf);
void ultimate3_cmd_bot_nick(char *nick, char *user, char *host, char *real, char *modes);
void ultimate3_cmd_notice(char *source, char *dest, char *buf);
void ultimate3_cmd_privmsg(char *source, char *dest, char *buf);
void ultimate3_cmd_serv_notice(char *source, char *dest, char *msg);
void ultimate3_cmd_serv_privmsg(char *source, char *dest, char *msg);
void ultimate3_cmd_quit(char *source, char *buf);
void ultimate3_cmd_pong(char *servname, char *who);
void ultimate3_cmd_join(char *user, char *channel, time_t chantime);
void ultimate3_cmd_part(char *nick, char *chan, char *buf);
void ultimate3_cmd_global(char *source, char *buf);
void ultimate3_cmd_squit(char *servname, char *message);
void ultimate3_cmd_connect(void);
void ultimate3_cmd_eob(void);
void ultimate3_cmd_ctcp(char *source, char *dest, char *buf);
void ultimate3_cmd_version(char *server);
void ultimate3_cmd_stats(char *sender, const char *letter, char *server);
void ultimate3_cmd_motd(char *sender, char *servr);
void ultimate3_cmd_ping(char *server);
void ultimate3_cmd_pass(char *pass);
void ultimate3_cmd_server(char *servname, int hop, char *descript);
void ultimate3_cmd_capab(void);
void ultimate3_cmd_burst(void);
void ultimate3_cmd_svinfo(void);
void IRCDModeInit(void);
int denora_event_436(char *source, int ac, char **av);
int denora_event_away(char *source, int ac, char **av);
int denora_event_ping(char *source, int ac, char **av);
int denora_event_motd(char *source, int ac, char **av);
int denora_event_join(char *source, int ac, char **av);
int denora_event_kick(char *source, int ac, char **av);
int denora_event_kill(char *source, int ac, char **av);
int denora_event_mode(char *source, int ac, char **av);
int denora_event_quit(char *source, int ac, char **av);
int denora_event_squit(char *source, int ac, char **av);
int denora_event_topic(char *source, int ac, char **av);
int denora_event_whois(char *source, int ac, char **av);
int denora_event_part(char *source, int ac, char **av);
int denora_event_server(char *source, int ac, char **av);
int denora_event_nick(char *source, int ac, char **av);
int denora_event_gnotice(char *source, int ac, char **av);
int denora_event_privmsg(char *source, int ac, char **av);
int denora_event_capab(char *source, int ac, char **av);
int denora_event_sjoin(char *source, int ac, char **av);
int denora_event_svinfo(char *source, int ac, char **av);
int denora_event_chghost(char *source, int ac, char **av);
int denora_event_sethost(char *source, int ac, char **av);
int denora_event_chgident(char *source, int ac, char **av);
int denora_event_setident(char *source, int ac, char **av);
int denora_event_chgname(char *source, int ac, char **av);
int denora_event_setname(char *source, int ac, char **av);
int denora_event_svsinfo(char *source, int ac, char **av);
int denora_event_snick(char *source, int ac, char **av);
int denora_event_vhost(char *source, int ac, char **av);
int denora_event_tkl(char *source, int ac, char **av);
int denora_event_eos(char *source, int ac, char **av);
int denora_event_eob(char *source, int ac, char **av);
int denora_event_pass(char *source, int ac, char **av);
int denora_event_netinfo(char *source, int ac, char **av);
int denora_event_error(char *source, int ac, char **av);
int denora_event_netctrl(char *source, int ac, char **av);
int denora_event_notice(char *source, int ac, char **av);
int denora_event_snotice(char *source, int ac, char **av);
int denora_event_sqline(char *source, int ac, char **av);
int denora_event_smo(char *source, int ac, char **av);
int denora_event_myid(char *source, int ac, char **av);
int denora_event_vctrl(char *source, int ac, char **av);
int denora_event_tctrl(char *source, int ac, char **av);
int denora_event_snetinfo(char *source, int ac, char **av);
int denora_event_globops(char *source, int ac, char **av);
int denora_event_burst(char *source, int ac, char **av);
int denora_event_sdesc(char *source, int ac, char **av);
int denora_event_svsmode(char *source, int ac, char **av);
int denora_event_sgline(char *source, int ac, char **av);
int denora_event_pong(char *source, int ac, char **av);
int denora_event_client(char *source, int ac, char **av);

int DenoraInit(int argc, char **argv);
void moduleAddIRCDMsgs(void);
void moduleAddIRCDCmds(void);
void DenoraFini(void);
char *ultimate3_nickip(char *host);


