/* libpeak
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
 * $Id: mem_pool.h,v 1.1.1.1 2003/12/30 02:29:18 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_MEM_POOL_H_
#define INCLUDED_PEAK_MEM_POOL_H_

/*!
 * @defgroup mem_pool Memory pool
 * 
 * @par
 * Memory pool allocator for objects of same size. This interface offers
 * an allocator peak_mem_pool_new() which is O(1) except in some rare cases
 * when the pool is growing (at worst log2(\p n) times for \p n allocations).
 * However, if you know its size in advance, you can precise it in the
 * \p peak_mem_pool creation method peak_mem_pool_create(). You can create
 * as many memory pools you want as interface and implementation are object
 * oriented (like most of peak stuffs).
 */

/*!
 * @ingroup mem_pool
 * @brief Opaque mem_pool pointer type.
 */
typedef struct __peak_mem_pool * peak_mem_pool;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup mem_pool
 * @brief Create a mem_pool object.
 * 
 * @param object_size Size of each object in the memory pool. This cannot be
 *                    changed later. It's commonly \p sizeof(struct
 *                    your_object_structure).
 * @param log_size    Log base 2 of the initial size of memory pool. Use 0
 *                    if you don't know, it will use the minimum.
 * 
 * @result            A new allocated \p peak_mem_pool reference or \p NULL
 *                    if the operation failed.
 */
extern peak_mem_pool peak_mem_pool_create(int object_size, int log_size);

/*!
 * @ingroup mem_pool
 * @brief Allocate an object in constant time.
 * 
 * This method is guaranted O(1) almost all the time, but in rare case it
 * might allocate a chunk of memory to let the memory pool grow.
 * 
 * @param pool        The memory pool reference.
 * 
 * @result            A pointer to \a object_size bytes of memory. The
 *                    allocated space is suitably aligned for storage of any
 *                    common type of object. Please consult libpeak's compile
 *                    time options to enable storage of special object like
 *                    Altivec's vector (which needs 16-bytes align). Also
 *                    please note the returned memory is NOT normally
 *                    initialized to zero bytes.
 */
extern void * peak_mem_pool_new(peak_mem_pool pool);

/*!
 * @ingroup mem_pool
 * @brief Deallocate an object in constant time.
 * 
 * This method is guaranted O(1) but in rare case it might free a chunk of
 * memory.
 * 
 * @param pool        The memory pool reference.
 * @param ptr         The pointer to a memory space to deallocate. This pointer
 *                    must have been previously provided by
 *                    peak_mem_pool_new().
 */
extern void peak_mem_pool_delete(peak_mem_pool pool, void *ptr);

/*!
 * @ingroup mem_pool
 * @brief Statistics: get used pointers count.
 * 
 * This method is guaranted O(1).
 * 
 * @param pool        The memory pool reference.
 * 
 * @result            Number of pointer currently in use (allocated).
 */
extern int peak_mem_pool_get_used_count(peak_mem_pool pool);

/*!
 * @ingroup mem_pool
 * @brief Statistics: get free pointers count.
 * 
 * This method is guaranted O(1).
 * 
 * @param pool        The memory pool reference.
 * 
 * @result            Number of pointer currently unused (free for allocation).
 */
extern int peak_mem_pool_get_free_count(peak_mem_pool pool);

/*!
 * @ingroup mem_pool
 * @brief Statistics: get total pointers count.
 * 
 * This method is guaranted O(1).
 * 
 * @param pool        The memory pool reference.
 * 
 * @result            Size of all pointers really allocated by the memory
 *                    pool. It grows automatically when no more free
 *                    pointers are available, and also might eventually
 *                    decrease if needed.
 */
extern int peak_mem_pool_get_size(peak_mem_pool pool);

#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_MEM_POOL_H_ */
