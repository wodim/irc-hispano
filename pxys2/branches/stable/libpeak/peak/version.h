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
 * $Id: version.h,v 1.1.1.1 2003/12/30 02:29:39 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_VERSION_H_
#define INCLUDED_PEAK_VERSION_H_

/*!
 * @defgroup version Version
 * 
 * Stuffs to access peak's info and version in your application.
 */

/*!
 * @ingroup version
 * MNNFFPPS: major minor fix patch status
 */
#define PEAK_VERSION_NUMBER         0x0010000fL
#define PEAK_VERSION_DYLIB          "0.1.0"
#define PEAK_VERSION_DYLIB_COMPAT   ""


#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup version
 * @brief Get peak's version number.
 *
 * Useful for even more precise compatibility check.
 * 
 * @result A number identifying libpeak's version.
 */
extern long peak_version_get_number();


/*!
 * @ingroup version
 * @brief Get peak's version as a string.
 *
 * @result A pointer to a string of the version of peak. You don't have to free
 *         it as it's still owned by the library.
 */
extern const char* peak_version_get_string();


#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_VERSION_H_ */
