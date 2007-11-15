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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <setjmp.h>

typedef char *exception;

#define EXCEPTION_INTERNAL_NAME(_E_) exception__##_E_##__

extern void exception_push(exception e);
extern void exception_pop(void);
extern void *exception_top_jmp_buf(void);
extern void exception_raise(exception e,void *parameter,char *file,int line);
extern void exception_raise_catched_exception(void);
extern int exception_raised(exception e);
extern char *exception_current_name(void);
extern void *exception_current_parameter(void);

extern int exception_raised_flag;

#define DEFINE_EXCEPTION(_E_) \
    exception EXCEPTION_INTERNAL_NAME(_E_) = #_E_;

#define DEFINE_LOCAL_EXCEPTION(_E_) \
    static exception EXCEPTION_INTERNAL_NAME(_E_) = #_E_;

#define USE_EXCEPTION(_E_) extern exception EXCEPTION_INTERNAL_NAME(_E_)

#define HANDLE(_E_,_EXP_) \
    (exception_push(EXCEPTION_INTERNAL_NAME(_E_)), \
     setjmp(exception_top_jmp_buf()) == 0 \
     ? ((_EXP_),(exception_raised_flag = 0)) \
     : (exception_raised_flag = 1), \
     exception_pop())
  
#define IHANDLE(_E_,_I_) \
    { \
     exception_push(EXCEPTION_INTERNAL_NAME(_E_)); \
     if (setjmp(exception_top_jmp_buf()) == 0) \
     { _I_; exception_raised_flag = 0; } \
     else \
     { exception_raised_flag = 1; } \
     exception_pop(); \
    }

#define RAISE(_E_,_M_) exception_raise(EXCEPTION_INTERNAL_NAME(_E_), _M_, __FILE__, __LINE__)

#define RAISE_AGAIN() exception_raise_catched_exception()

#define EXCEPTION_RAISED(_E_) exception_raised(EXCEPTION_INTERNAL_NAME(_E_))

USE_EXCEPTION(any);
USE_EXCEPTION(error);

#endif  /* EXCEPTION_H */
