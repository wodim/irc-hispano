/* Encapsulation of memory allocator using a memory_default exception
 * Copyright (C) 1996, 98, 99 Achille Braquelaire (achille@labri.u-bordeaux.fr)
 * 
 * This file is part of BCL (Basic Library for C programs)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation (Inc., 59
 * Temple Place - Suite 330, Boston, MA 02111-1307, USA); either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <stdlib.h>
#include <stddef.h>

#include "memory.h"
#include "exception.h"

static void * (*malloc_fun)(size_t) = malloc;
static void * (*realloc_fun)(void *, size_t) = realloc;
static void (*free_fun)(void *) = free;

DEFINE_EXCEPTION(memory_default)

void *
_memory_alloc(size_t size)
{
    return malloc_fun(size);
}

void *
memory_alloc(size_t size)
{
    void *p = _memory_alloc(size);

    if (p == NULL)
	RAISE(memory_default, "Cannot alloc memory");
    return p;
}

void *
memory_realloc(void *p, size_t size)
{
    p = realloc_fun(p, size);

    if (p == NULL)
	RAISE(memory_default, "Cannot realloc memory");
    return p;
}

void 
memory_free(void *p)
{
    free_fun(p);
}

void
memory_set_functions(void *(*malloc_user_fun)(size_t),
		     void (*free_user_fun)(void*))
{
    malloc_fun = malloc_user_fun;
    free_fun = free_user_fun;
}
