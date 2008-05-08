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
 * $Id: tz.h,v 1.2 2004/01/07 18:22:01 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_TZ_H_
#define INCLUDED_PEAK_TZ_H_

/*!
 * @defgroup tz Time zone
 * 
 * Although it might look a bit off-topic, in order to provide a minimal but
 * good time/date interface, the peak library is able to handle time zone
 * conversion. Thanks to its design, time/date operations are efficient and
 * a lack of time zone support would add unnecessary overhead in
 * application's code.
 */

#include <sys/types.h>

/*!
 * @ingroup tz
 * @brief Opaque time zone pointer type.
 */
typedef struct __peak_tz * peak_tz;


#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup tz
 * @brief Create a time zone object with a name.
 * 
 * @param tz_name  Pathname of a tzfile(5)-format file from which to read
 *                 the conversion information. If the first character of the
 *                 pathname is a slash (`/') it is used as an absolute
 *                 pathname; otherwise, it is used as a pathname relative to
 *                 the system time conversion information directory (often
 *                 \p /usr/share/zoneinfo).
 * 
 * @return         New time zone reference (use peak_release() when you don't
 *                 need it anymore) or \p NULL if the time zone file cannot be
 *                 found or another misc issue is encountered.
 */
extern peak_tz peak_tz_create(const char *tz_name);

/*!
 * @ingroup tz
 * @brief Create a time zone object with current system settings.
 * 
 * @return         The system's time zone reference, possibly a reference
 *                 to the UTC time zone if the library doesn't understand the
 *                 system's configuration or if a system's misconfiguration
 *                 is encountered. Finally returns \p NULL if both attempts
 *                 failed.
 */
extern peak_tz peak_tz_create_system(void);

/*!
 * @ingroup tz
 * @brief Get time zone's name.
 * 
 * @param tz       The time zone reference.
 * 
 * @return         A pointer to a constant string of the name of the time zone
 *                 \a tz. The string is owned by the library so don't release
 *                 it and copy it if you need it for a long time.
 */
extern const char * peak_tz_get_name(peak_tz tz);

/*!
 * @ingroup tz
 * @brief Get time zone's abbreviation at the specified date.
 * 
 * @param tz       The time zone reference.
 * @param t        Unix time. It is required as the abbreviation may be
 *                 different at different dates (eg. EDT/EST, NZDT/NZST, etc.).
 *                 Use peak_time() if you want the current abbreviation.
 * 
 * @return         A pointer to a constant string of the abbreviation of the
 *                 time zone \a tz. The string is owned by the library so
 *                 don't release it and copy it if you need it for a long time.
 */
extern const char * peak_tz_get_abbreviation(peak_tz tz, time_t t);

/*!
 * @ingroup tz
 * @brief Get time zone's GMT offset at a specified date.
 * 
 * @param tz       The time zone reference.
 * @param t        Unix time. It is required as the offset may be different
 *                 at different dates. Use peak_time() if you want the current
 *                 seconds from GMT.
 * 
 * @return         The time interval from GMT in second.
 */
extern time_t peak_tz_get_gmt_offset(peak_tz tz, time_t t);

/*!
 * @ingroup tz
 * @brief Returns whether or not a time zone is in daylight savings time at
 *        a specified date.
 * 
 * @param tz       The time zone reference.
 * @param t        Unix time's date for which you want to know if the time
 *                 zone is in daylight savings time.
 * 
 * @retval 0       Normal period.
 * @retval 1       Daylight Savings Time (DST) period.
 */
extern int peak_tz_is_dst(peak_tz tz, time_t t);

#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_TZ_H_ */
