/* PEAK Library
 *
 * Copyright (c) 2003
 *      Stephane Thiell <mbuna@bugged.org>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * $Id: internal.h,v 1.2 2004/01/08 16:39:55 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_INTERNAL_H_
#define INCLUDED_PEAK_INTERNAL_H_

/* For Mach/Darwin
 * The private_extern directive makes symbol_name a private external symbol.
 * When the link editor combines this module with other modules (and the
 * -keep_private_externs command-line option is not specified) the symbol
 * turns it from global to static. 
 */

#if defined(__MACH__) && defined(__MWERKS__) && !defined(__private_extern__)
# define __private_extern__ __declspec(private_extern)
#endif

#if !defined(__MACH__) || !defined(__GNUC__)
/* No distinction is done by other systems. */
#define __private_extern__
#endif

#include "alloc.h"
#include "errors.h"
#include "runtime.h"


#endif /* INCLUDED_PEAK_INTERNAL_H_ */
