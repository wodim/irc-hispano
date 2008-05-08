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
 * $Id: alloc.h,v 1.1.1.1 2003/12/30 02:29:05 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_ALLOC_H_
#define INCLUDED_PEAK_ALLOC_H_

/*!
 * @defgroup alloc Allocation
 * 
 * @par
 * Peak's allocation mechanism uses the standard malloc/free pair but can be
 * configured as wish with peak_alloc_configure() providing your
 * own allocation and deallocation primitives.
 * 
 * @par
 * Although written in C, peak is object oriented and features a
 * lightweight support for reference counting in all peak objects. This
 * module offers access to the peak_retain() an peak_release() method.\n
 * Strings, however, are normal C strings and not wrapped by the peak
 * library. For convenience, a peak_strdup() method is provided as well.
 */

#include <sys/types.h>

/*!
 * @ingroup alloc
 * @brief Malloc-like function pointer type.
 */
typedef void * (*peak_alloc_malloc_func)(size_t size);

/*!
 * @ingroup alloc
 * @brief Free-like function pointer type.
 */
typedef void (*peak_alloc_free_func)(void *p);

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup alloc
 * @brief Configure peak's allocation for your program.
 * 
 * Pointed functions must properly align memory and be thread-safe like
 * malloc() and free()...
 * 
 * @param malloc_fun A pointer to your custom malloc-like function.
 * @param free_fun   A pointer to your custom free-like function.
 */
extern void peak_alloc_configure(peak_alloc_malloc_func malloc_fun,
                                 peak_alloc_free_func free_fun);

/*!
 * @ingroup alloc
 * @brief Allocate memory.
 * 
 * @param size Number of bytes of memory to allocate.
 * 
 * @result A pointer to the fresh allocated memory.
 */
extern void * peak_allocate(size_t size);

/*!
 * @ingroup alloc
 * @brief Free memory.
 * 
 * @param ptr Pointer to the memory space to deallocate.
 */
extern void peak_deallocate(void *ptr);

/*!
 * @ingroup alloc
 * @brief Copy a string.
 * 
 * @param str The string to copy/duplicate.
 * 
 * @result A pointer to a new allocated copy of the string. It should be
 *         passed to peak_deallocate() if you want to free it.
 */
extern char * peak_strdup(const char *str);

/*!
 * @ingroup alloc
 * @brief Retain a peak object.
 * 
 * Increase the reference count of the object \a obj.
 * 
 * @param obj Any peak object.
 * 
 * @result For convenience, a pointer to \a obj.
 */
extern void * peak_retain(void *obj);


/*!
 * @ingroup alloc
 * @brief Release a peak object.
 * 
 * Decrease the reference count of the object \a obj. If the refcount is 0, 
 * the object is deallocated.
 * 
 * @param obj Any peak object.
 */
extern void peak_release(void *obj);


/*!
 * @ingroup alloc
 * @brief Get the retain count of a peak object.
 * 
 * @param obj Any peak object.
 *
 * @result Retain count or -1 for constant objects.
 */
extern int peak_get_retcnt(void *obj);


#if defined(__cplusplus)
}
#endif


#endif /* INCLUDED_PEAK_ALLOC_H_ */
