/*
 * irc_string.h from Undernet ircu ircd_string.h
 *
 * $Id: irc_string.h,v 1.2 2003/12/30 21:03:55 mbuna Exp $
 */
#ifndef INCLUDED_irc_string_h
#define INCLUDED_irc_string_h
#ifndef INCLUDED_irc_chattr_h
#include "irc_chattr.h"
#endif
#include <dbprim/dbprim.h>

extern int ircd_strcmp(const char *a, const char *b);
extern int ircd_strncmp(const char *a, const char *b, size_t n);

static inline unsigned long
ircd_strcomp(hash_table_t *table, db_key_t *key1, db_key_t *key2)
  {
  return (!(dk_len(key1) == dk_len(key2)
            && ircd_strcmp(dk_key(key1), dk_key(key2)) == 0));
  }

#endif /* INCLUDED_irc_string_h */
