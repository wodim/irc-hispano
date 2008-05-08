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

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include "exception.h"

extern void *memory_alloc(size_t size);
extern void *memory_realloc(void *p, size_t size);
extern void *_memory_alloc(size_t size);
extern void memory_free(void *p);
extern void memory_set_functions(void *(*malloc_user_fun)(size_t),
				 void (*free_user_fun)(void*));

USE_EXCEPTION(memory_default);
			      
#endif  /* MEMORY_H */
