/* $Id: slab_alloc.c 256 2008-12-01 14:40:06Z dfmartinez $ */

#include "runmalloc.h"
#include "slab_alloc.h"

#include <string.h>


char *SlabStringAlloc(size_t size)
{
  return RunMalloc(size);
}

void SlabStringAllocDup(char **old, char *new, size_t max_len)
{
  int len;

  if (*old)
    RunFree(*old);

  len = strlen(new);
  if ((!max_len) || (len <= max_len))
  {
    *old = RunMalloc(len + 1);
    strcpy(*old, new);
    return;
  }

/* Nos pasamos de taman~o */
  *old = RunMalloc(max_len + 1);
  strncpy(*old, new, max_len);
  (*old)[max_len] = '\0';
}

void SlabStringFree(char *string)
{
  RunFree(string);
}
