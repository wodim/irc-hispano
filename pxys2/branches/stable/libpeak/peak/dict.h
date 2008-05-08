/* PEAK Library
 *
 * Copyright (c) 2003, 2004
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
 * $Id: dict.h,v 1.2 2004/01/05 18:46:15 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_DICT_H_
#define INCLUDED_PEAK_DICT_H_

/*!
 * @ingroup db
 * @defgroup dict Dictionary
 * 
 * @par
 * Generic object-oriented dictionary support. It implements a container which
 * pairs pointer-sized keys with pointer-sized values. Values are accessed
 * via arbitrary user-defined keys.
 */

#include <peak/stdint.h>


/*!
 * @ingroup dict
 * @brief Opaque dictionary pointer type.
 */
typedef struct __peak_dict * peak_dict;

/*!
 * @ingroup dict
 * @brief Dictionary's key or value retain callback.
 */
typedef void * (*peak_dict_retain_callback)(const void *value);

/*!
 * @ingroup dict
 * @brief Dictionary's key or value release callback.
 */
typedef void (*peak_dict_release_callback)(const void *value);

/*!
 * @ingroup dict
 * @brief Dictionary's key equal callback.
 * 
 * Should return 0 if the objects match or another value otherwise (eg. 1).
 */
typedef int (*peak_dict_equal_callback)(const void *val1, const void *val2);

/*!
 * @ingroup dict
 * @brief Dictionary's key callback.
 * 
 * Should return a good 32-bits hash value.
 */
typedef uint32_t (*peak_dict_hash_callback)(const void *value);

/*!
 * @ingroup dict
 * @brief Structure containing the callbacks for keys of a dictionary.
 */
typedef struct
  {
  /*!
   * @brief Retain callback.
   * 
   * If not \p NULL, it's called with the key as argument when an entry is
   * added in the dictionary.
   */
  peak_dict_retain_callback retain;
  /*!
   * @brief Release callback.
   * 
   * If not \p NULL, it's called with the key as argument when an entry is
   * removed from the dictionary.
   */
  peak_dict_release_callback release;
  /*!
   * @brief Equal callback.
   * 
   * It's used to compare keys when accessing a value. If \p NULL, pointer
   * equality is used.
   */
  peak_dict_equal_callback equal;
  /*!
   * @brief Hash callback.
   * 
   * It's used to hash keys when adding an entry. If \p NULL, pointer hashing
   * is used.
   */
  peak_dict_hash_callback hash;
  } peak_dict_key_ops_s;

/*!
 * @ingroup dict
 * @brief Structure containing the callbacks for values of a dictionary.
 */
typedef struct
  {
  /*!
   * @brief Retain callback.
   * 
   * If not \p NULL, it's called with the value as argument when an entry is
   * added in the dictionary.
   */
  peak_dict_retain_callback retain;
  /*!
   * @brief Release callback.
   * 
   * If not \p NULL, it's called with the value as argument when an entry is
   * removed from the dictionary.
   */
  peak_dict_release_callback release;
  } peak_dict_value_ops_s;


/*!
 * @ingroup dict
 * Predefined callbacks structure appropriate for use when keys of a
 * dictionary are unknown objects and you want nothing to be performed as
 * retain/release, and you can't provide better equality or hashing function
 * than pointer-related ones.
 */
extern const peak_dict_key_ops_s peak_dict_null_key_ops;

/*!
 * @ingroup dict
 * Predefined callbacks structure appropriate for use when keys of a
 * dictionary are constant strings.
 */
extern const peak_dict_key_ops_s peak_dict_string_key_ops;

/*!
 * @ingroup dict
 * Predefined callbacks structure appropriate for use when keys of a
 * dictionary are strings to be copied by the dictionary.
 */
extern const peak_dict_key_ops_s peak_dict_string_copy_key_ops;

/*!
 * @ingroup dict
 * Predefined callbacks structure appropriate for use when keys of a
 * dictionary are constant strings ignoring case.
 */
extern const peak_dict_key_ops_s peak_dict_stringcase_key_ops;

/*!
 * @ingroup dict
 * Predefined callbacks structure appropriate for use when keys of a
 * dictionary are strings ignoring case to be copied by the dictionary.
 */
extern const peak_dict_key_ops_s peak_dict_stringcase_copy_key_ops;

/*!
 * @ingroup dict
 * Predefined callbacks structure appropriate for use when values of a
 * dictionary are unknown objects.
 */
extern const peak_dict_value_ops_s peak_dict_null_value_ops;

/*!
 * @ingroup dict
 * Predefined callbacks structure appropriate for use when values of a
 * dictionary are constant strings.
 */
extern const peak_dict_value_ops_s peak_dict_string_value_ops;

/*!
 * @ingroup dict
 * Predefined callbacks structure appropriate for use when values of a
 * dictionary are strings to be copied by the dictionary.
 */
extern const peak_dict_value_ops_s peak_dict_string_copy_value_ops;

/*!
 * @ingroup dict
 * @brief Structure containing a key and value pair to be used at the
 *        creation of the dictionary.
 * 
 * It's a convenience to allow application's developers to easily create
 * dictionaries where entries are predefined in an array containing
 * \a peak_dict_init_entry structures.
 * 
 * @par
 * For example,
 * @verbatim
 * static const peak_dist_init_entry init_values[] = {
 *     { "key1", "value1" },
 *     { "key2", "value2" }
 * }; @endverbatim
 */
typedef struct
  {
  /*! @brief the key */
  const void *key;
  /*! @brief the value */
  const void *value;
  } peak_dict_init_entry;

/* Convenience
 */
#define PEAK_DICT_INIT_ENTRY_COUNT(t) (sizeof(t)/sizeof(peak_dict_init_entry))

/*!
 * @ingroup dict
 * @brief Apply function pointer type. See peak_dict_apply().
 */
typedef void (*peak_dict_apply_func)(const void *key, const void *value,
                                     void *context);

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @ingroup dict
 * @brief Create a new dictionary.
 * 
 * All peak's dictionary are created mutable, you can always add or remove
 * elements. Immutable dictionary might be added in the future.
 * 
 * @param key_ops     A pointer to an user-defined operations structure
 *                    peak_dict_key_ops_s on keys. A copy of the pointed
 *                    structure is made, so you can reuse or deallocate the
 *                    structure after this call.
 * @param value_ops   A pointer to an user-defined operations structure
 *                    peak_dict_value_ops_s on values. A copy of the pointed
 *                    structure is made, so you can reuse or deallocate the
 *                    structure after this call.
 * @param init_values A C array of peak_dict_init_entry to be immediately
 *                    added in the dictionary or \p NULL if you want to create
 *                    an empty dictionary. The array can be freed after the
 *                    call. The key's retain callback, if any, is called for
 *                    each item. Same thing for each value.
 * @param num_values  Number of peak_dict_init_entry structures in the
 *                    \a init_values C array. Use 0 if you want an empty
 *                    dictionary.
 * 
 * @result A pointer to a new allocated dictionary or \p NULL if the operation
 *         failed. Use peak_release() when you want to dispose it.
 */
extern peak_dict peak_dict_create(const peak_dict_key_ops_s *key_ops,
                                  const peak_dict_value_ops_s *value_ops,
                                  const peak_dict_init_entry *init_values,
                                  int num_values);

/*!
 * @ingroup dict
 * @brief Returns the number of values currently in the dictionary.
 * 
 * @param dict        The dictionary reference.
 * 
 * @result            Number of values currently in the dictionary.
 */
extern int peak_dict_get_count(peak_dict dict);

/*!
 * @ingroup dict
 * @brief Retrieves the value associated with the given key.
 * 
 * This method will use the hash and equal key callbacks provided when the
 * dictionary was created.
 * 
 * @param dict        The dictionary reference.
 * @param key         The key for which to find a match in the dictionary.
 * 
 * @result            Number of values currently in the dictionary.
 */
extern const void * peak_dict_get_value(peak_dict dict, const void *key);

/*!
 * @ingroup dict
 * @brief Retrieves all key and value pairs.
 * 
 * This method fills the two buffers with the keys and values from the
 * dictionary. The keys and values C arrays are parallel to each other but
 * the order or the pairs is not defined.
 * 
 * @param dict        The dictionary reference.
 * @param keys        A buffer large enough to hold all pointer-sized keys
 *                    or \p NULL.
 * @param values      A buffer large enough to hold all pointer-sized values
 *                    or \p NULL.
 */
extern void peak_dict_get_all(peak_dict dict, const void **keys,
                              const void **values);

/*!
 * @ingroup dict
 * @brief Call a function once for each value in the dictionary.
 * 
 * @param dict        The dictionary reference.
 * @param applier     The callback function.
 * @param context     A pointer-sized user-defined value, which is passed
 *                    to the applier function. This parameter is not used
 *                    by the library itself otherwise.
 */
extern void peak_dict_apply(peak_dict dict, peak_dict_apply_func applier,
                            void *context);

/*!
 * @ingroup dict
 * @brief "Add if absent" a key and value pair to the dictionary.
 * 
 * @param dict        The dictionary reference.
 * @param key         The key to add. It's retained by the dictionary using
 *                    the retain callback provided when the dictionary was
 *                    created.
 * @param value       The value to add, associated with the previous \a key.
 *                    It's retained by the dictionary using the retain
 *                    callback provided when the dictionary was created.
 */
extern void peak_dict_add(peak_dict dict, const void *key, const void *value);

/*!
 * @ingroup dict
 * @brief "Add if absent, replace is present" a key and value pair.
 * 
 * @param dict        The dictionary reference.
 * @param key         The key to set. If the key isn't already in the
 *                    dictionary, it's retained using the retain callback
 *                    provided when the dictionary was created.
 * @param value       The value to set, associated with the previous \a key.
 *                    It's retained by the dictionary using the retain
 *                    callback provided when the dictionary was created.
 */
extern void peak_dict_set(peak_dict dict, const void *key, const void *value);

/*!
 * @ingroup dict
 * @brief "Replace if present" a key and value pair.
 * 
 * @param dict        The dictionary reference.
 * @param key         The key of the value to replace in the dictionary.
 * @param value       The value to replace, associated with the previous
 *                    \a key. The old one is released by the dictionary using
 *                    the release callback and the new one is retained
 *                    using the retain callback, both provided when the
 *                    dictionary was created.
 */
extern void peak_dict_replace(peak_dict dict, const void *key,
                              const void *value);

/*!
 * @ingroup dict
 * @brief Remove a key and value pair from the dictionary.
 * 
 * @param dict        The dictionary reference.
 * @param key         The key of the value to remove in the dictionary.
 *                    They are released by the dictionary using the release
 *                    callbacks provided when the dictionary was created, if
 *                    any.
 */
extern void peak_dict_remove(peak_dict dict, const void *key);

/*!
 * @ingroup dict
 * @brief Remove all values from the dictionary.
 * 
 * @param dict        The dictionary reference.
 */
extern void peak_dict_clear(peak_dict dict);



#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_DICT_H_ */
