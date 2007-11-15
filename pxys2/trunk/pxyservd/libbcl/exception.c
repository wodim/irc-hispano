/* Exception handling in standard C
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
#include <stdio.h>
#include <setjmp.h>

#include "exception.h"

#ifdef BASIC_C_LIBRARY
# include "memory.h"
#else
DEFINE_EXCEPTION(memory_default);

static void *
_memory_alloc(size_t size)
{
    return malloc(size);
}

static void 
memory_free(void *p)
{
    free(p);
}
#endif /* BASIC_C_LIBRARY */

#define NO_EXCEPTION_NAME "(none)"

DEFINE_EXCEPTION(any)
DEFINE_EXCEPTION(error)

struct context
{
    jmp_buf context_buffer;
    struct context *previous;
    exception e;
};

int exception_raised_flag = 0;

static exception exception_name_storage;
static void *exception_parameter_storage;
static int exception_line_storage;
static char *exception_file_storage;

/*
  Here is linked the stack of contexts
*/
static struct context *top = NULL;

/*
  This hook refers the function used to emit an error message on
  uncaught exception
*/
static void 
default_send_uncaught_message(char *name, char *message, char *file, int line)
{
    fprintf(stderr, 
	    "Uncaught exception <%s> raised from \"%s:%d\"\n",
            name, file, line);
    if (message != NULL)
    fprintf(stderr, ">> %s\n", message);
}

static void (*send_uncaught_message)(char *name, char *message, 
				     char *file, int line) 
    = default_send_uncaught_message;


void
exception_push(exception e)
{
    struct context *c = _memory_alloc(sizeof (struct context));

    if (c == NULL)
	RAISE(memory_default, NULL);

    c->previous = top;
    c->e = e;
    top = c;
}

void *
exception_top_jmp_buf(void)
{
    if (top == NULL)
        return NULL;
    return top->context_buffer;
}

static int 
exception_search(exception e)
{
    for (;;)
    {
        if (top == NULL)
            return 0;
        if (top->e == e || top->e == EXCEPTION_INTERNAL_NAME(any))
            return 1;
        exception_pop();
    }
}

void 
exception_raise(exception e, void *parameter, char *file, int line)
{
    if (exception_search(e))
    {
	exception_name_storage = e;
	exception_parameter_storage = parameter;
	exception_file_storage = file;
	exception_line_storage = line;
	longjmp(exception_top_jmp_buf(), 1);
    }
    else
    {
	char *p = NULL;

	if (e == EXCEPTION_INTERNAL_NAME(error))
	    p = parameter;
	send_uncaught_message(e, p, file, line);
	exit(EXIT_FAILURE);
    }
}

void
exception_raise_catched_exception(void)
{
    exception_raise(exception_name_storage, 
		    exception_parameter_storage,
		    exception_file_storage,
		    exception_line_storage);
}

int
exception_raised(exception e)
{
    if (exception_raised_flag == 0)
	return 0;
    if (e == EXCEPTION_INTERNAL_NAME(any))
	return 1;
    else
	return exception_name_storage == e;
}

char *
exception_current_name(void)
{
    if (exception_raised_flag == 0)
	return NULL;
    else
	return exception_name_storage;
}

void *
exception_current_parameter(void)
{
    if (exception_raised_flag == 0)
	return NULL;
    else
	return exception_parameter_storage;
}

void 
exception_pop(void)
{
    struct context *c = top->previous;

    memory_free(top);
    top = c;
}
