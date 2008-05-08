
#include "irc_string.h"

/*
 * ircd_strcmp - case insensitive comparison of 2 strings
 * NOTE: see ircd_chattr.h for notes on case mapping.
 */
int
ircd_strcmp(const char *a, const char *b)
  {
  const char* ra = a;
  const char* rb = b;
  while (ToLower(*ra) == ToLower(*rb)) {
    if (!*ra++)
      return 0;
    else
      ++rb;
    }
  return (*ra - *rb);
  }

/*
 * ircd_strncmp - counted case insensitive comparison of 2 strings
 * NOTE: see ircd_chattr.h for notes on case mapping.
 */
int
ircd_strncmp(const char *a, const char *b, size_t n)
  {
  const char* ra = a;
  const char* rb = b;
  int left = n;
  if (!left--)
    return 0;
  while (ToLower(*ra) == ToLower(*rb)) {
    if (!*ra++ || !left--)
      return 0;
    else
      ++rb;
    }
  return (*ra - *rb);
  }
