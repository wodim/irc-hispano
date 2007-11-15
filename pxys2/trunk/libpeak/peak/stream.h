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
 * $Id: stream.h,v 1.1.1.1 2003/12/30 02:29:27 mbuna Exp $
 */
#ifndef INCLUDED_PEAK_STREAM_H_
#define INCLUDED_PEAK_STREAM_H_

/*!
 * @defgroup stream Socket streams
 * This API provides a functional interface to asynchronously processed socket
 * streams. For simplicity, unlike other implementations, one stream instance
 * can be readable and writable at the same time. Once configured, a stream
 * must be scheduled with a task. The library will then notify the
 * application using a notification callback, which will be called by one of
 * the task's thread.
 */
/*!
 * @defgroup stream_common Stream common
 * @ingroup stream
 * @brief Creation methods and other common methods for socket streams.
 */
/*!
 * @defgroup stream_low Non-buffered operations
 * @ingroup stream
 * @brief Low level read/write primitives.
 */
/*!
 * @defgroup stream_line Line-mode operations
 * @ingroup stream
 * @brief Line-mode based stream methods.
 */
/*!
 * @defgroup stream_buf Write-buffered operations
 * @ingroup stream
 * @brief Write-buffered stream methods.
 */
/*!
 * @defgroup stream_sched Scheduling
 * @ingroup stream
 * @brief Scheduling task for streams.
 * 
 * Well that's the way to really use 'em.
 */

#include <peak/stdint.h>
#include <peak/task.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <unistd.h>

struct iovec;

/*!
 * @ingroup stream
 * @brief Opaque stream pointer type.
 */
typedef struct __peak_stream * peak_stream;

/*!
 * @ingroup stream
 * @brief Stream creation options
 */
enum {
  /*!
   * @brief Default: SOCK_STREAM socket (reliable stream), ie. TCP.
   */
  PEAK_STREAM_OPT_DEFAULT = 0,
  
  /*!
   * @brief UDP socket stream (not a reliable stream).
   */
  PEAK_STREAM_OPT_DGRAM = 1 << 0,

  /*!
   * @brief Listening socket stream. Will use the address argument as the
   *        local address (where to bind).
   */
  PEAK_STREAM_OPT_LISTEN = 1 << 1,
  
  /*!
   * @brief Line mode, useful for line-based text streams; the callback is
   *        called for every line received.
   */
  PEAK_STREAM_OPT_LINEMODE = 1 << 2,
  
  /*!
   * @brief Auto schedule, avoid a call to peak_stream_schedule. Useful for
   *        accepted socket.
   */
  PEAK_STREAM_OPT_AUTOSCHEDULE = 1 << 3
  };

/*!
 * @ingroup stream
 * @brief Types of event passed to your stream event callback function.
 */
enum {
  /*!
   * @brief Never used.
   */
  PEAK_STREAM_EVT_NONE = 0,
  /*!
   * @brief Open event. The stream is now open (connection established).
   */
  PEAK_STREAM_EVT_OPEN,
  /*!
   * @brief Accept event. A new stream can be created using
   *        peak_stream_accept_create().
   */
  PEAK_STREAM_EVT_ACCEPT,
  /*!
   * @brief Read event. There's something to read on the stream.
   * 
   * Until you don't really read it, you won't be notified again with this
   * event type.
   */
  PEAK_STREAM_EVT_READ,
  /*!
   * @brief Write event. You can efficiently write on the stream NOW.
   *
   * Until you don't really write something, you won't be notified again with
   * this event type.
   */
  PEAK_STREAM_EVT_WRITE,
  /*!
   * @brief End of stream event. No further bytes can be read/written.
   *
   * You should release the stream with peak_release().
   */
  PEAK_STREAM_EVT_END,
  /*!
   * @brief An error occured on the stream's connection.
   *
   * You should release the stream with peak_release().
   */
  PEAK_STREAM_EVT_ERROR,
  /*!
   * @brief Stream's event timed out.
   *
   * Sent only when you enable stream's event timeout checking with
   * peak_stream_set_timeout(). You might release the stream with
   * peak_release(). If you ignore it, another event of type
   * \a PEAK_STREAM_EVT_TIMEDOUT will be sent after \p timeout seconds
   * as defined in peak_stream_set_timeout().
   */
  PEAK_STREAM_EVT_TIMEDOUT
  };

/*!
 * @ingroup stream_buf
 * @brief Possible error codes passed to your stream error callback function.
 * 
 * These are currently only used with write-buffered configured streams.
 */
enum {
  /*!
   * @brief All allowed message buffers of a write-buffered stream have
   *        been consumed!
   */
  PEAK_STREAM_ERR_MAX_MSGBUF_EXCEEDED = -10001

  };

/*!
 * @ingroup stream_low
 * @brief Internal library format buffer size.
 *
 * The length of a message formatted with peak_stream_write_format() or
 * peak_stream_write_vformat() cannot be highter than that.
 */
#define WRITE_FORMAT_BUFFER_SIZE 1024

/*!
 * @ingroup stream_line
 * @brief Internal library buffer size for line mode.
 *
 * The length of the line obtained via peak_stream_get_line() cannot be
 * highter than that.
 */
#define LINEMODE_BUFFER_SIZE 1024

/*!
 * @ingroup stream
 * @brief Stream client event callback.
 * 
 * Defines a pointer to your notification callback function that handles
 * interesting events that occur on a scheduled stream.
 */
typedef void (*peak_stream_event_callback)(peak_stream s,
                                           int type,
                                           void *context);

/*!
 * @ingroup stream_buf
 * @brief Error callback for write-buffered streams.
 * 
 * Defines a pointer to your error handling callback function that will be
 * triggered when a stream error happens.\n
 * Warning: NEVER release a stream inside this callback!
 */
typedef void (*peak_stream_error_callback)(peak_stream s,
                                           int error_type,
                                           void *context);

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @ingroup stream_common
 * @brief Creates a new socket stream from a socket address.
 * 
 * @param name    Pointer to a socket's name/address. For listening sockets,
 *                this is the name/local address of the socket, but for
 *                outgoing sockets this is the name/address of the other
 *                socket. Several socket protocol types are supported (PF_INET,
 *                PF_INET6).
 * @param namelen Length of the structure pointed by name.
 * @param opt     Stream creation options.
 * @param cb      A pointer to your notification callback function that handles
 *                events that will occur on this stream once scheduled.
 * @param context An extra application-defined pointer that will be passed
 *                to your event callback function (it's not used by the
 *                library).
 *
 * @return        A newly allocated peak_stream reference or NULL if the
 *                stream cannot be created.
 */
extern peak_stream peak_stream_socket_create(
  struct sockaddr *name,
  int namelen,
  uint32_t opt,
  peak_stream_event_callback cb,
  void *context
  );

/*!
 * @ingroup stream_common
 * @brief Creates a new socket stream from a incoming connection on a
 *        listening stream.
 * 
 * @param ss      The server's socket stream that got a PEAK_STREAM_EVT_ACCEPT
 *                event.
 * @param opt     Stream creation options.
 * @param cb      A pointer to your notification callback function that handles
 *                events that will occur on this stream once scheduled.
 * @param context An extra application-defined pointer that will be passed
 *                to your event callback function (it's not used by the
 *                library).
 *
 * @return        A newly allocated peak_stream reference or NULL if the
 *                stream cannot be created.
 */
extern peak_stream peak_stream_accept_create(
  peak_stream ss,
  uint32_t opt,
  peak_stream_event_callback cb,
  void *context
  );

/*!
 * @ingroup stream_common
 * @brief Creates a new socket stream from a "raw" file descriptor (which
 *        must be a valid opened socket).
 * 
 * @param fd      The socket file descriptor.
 * @param opt     Stream creation options.
 * @param cb      A pointer to your notification callback function that handles
 *                events that will occur on this stream once scheduled.
 * @param context An extra application-defined pointer that will be passed
 *                to your event callback function (it's not used by the
 *                library).
 *
 * @return        A newly allocated peak_stream reference or NULL if the
 *                stream cannot be created.
 */
extern peak_stream peak_stream_raw_create(
  int fd,
  uint32_t opt,
  peak_stream_event_callback cb,
  void *context
  );

/*!
 * @ingroup stream_common
 * @brief Test is a socket stream is open.
 * 
 * An open stream is a connected socket for connection oriented streams.
 * 
 * @param s      The stream reference.
 * 
 * @retval 0     Stream not open.
 * @retval 1     Stream open.
 */
extern int peak_stream_is_open(peak_stream s);

/*!
 * @ingroup stream_common
 * @brief Get last stream system error code.
 * 
 * Returns the last socket or system error code as defined in sys/errno.h.
 * 
 * @param s      The stream reference.
 * 
 * @return       The error code.
 */
extern int peak_stream_get_error(peak_stream s);


/*!
 * @ingroup stream_common
 * @brief Get peer's name (address).
 * 
 * @param s       The stream reference.
 * @param name    A pointer to an allocated space for the address, enough
 *                to hold it (eg. a struct sockaddr_in for an IPv4
 *                address).
 * @param namelen Input/output argument, should be initialized to indicate
 *                the amount of size pointed to by \a name. On return it
 *                contains the actual size of the name returned (in bytes).
 * 
 * @retval 0     if the operation is successful.
 * @retval -1    if the buffer \a name is too small.
 */
extern int peak_stream_get_address(
  peak_stream s,
  struct sockaddr *name,
  int *namelen
  );

/*!
 * @ingroup stream_common
 * @brief Assign a name (address) to an unnamed socket.
 * 
 * This function allow you to bind the stream to a local address which is not
 * the default (eg. for a vhost). Not for use with streams with the
 * \p PEAK_STREAM_OPT_LISTEN option flag.
 * 
 * @param s       The stream reference.
 * @param name    A pointer to a structure describing the name (address) to
 *                assign to the stream socket.
 * @param namelen The length of the pointed structure by name.
 *
 * @retval 0     if the operation is successful.
 * @retval -1    otherwise.
 */
extern int peak_stream_set_address(
  peak_stream s,
  struct sockaddr *name,
  int namelen
  );


/*!
 * @ingroup stream_common
 * @brief Connect the stream.
 * 
 * @param s       The stream reference.
 *
 * @retval 0      if the operation is successful.
 * @retval -1     if the connection to the remote host failed.
 */
extern int peak_stream_connect(
  peak_stream s
  );

/*!
 * @ingroup stream_common
 * @brief Disconnect the stream.
 * 
 * @param s       The stream reference.
 */
extern void peak_stream_disconnect(
  peak_stream s
  );

/*!
 * @ingroup stream_common
 * @brief Get the context pointer of a stream.
 * 
 * @param s       The stream reference.
 *
 * @return        The application-defined pointer.
 */
extern void* peak_stream_get_context(
  peak_stream s
  );

/*!
 * @ingroup stream_common
 * @brief Change the context pointer of a stream.
 * 
 * @param s       The stream reference.
 * @param context An extra application-defined pointer that will be passed
 *                to your event callback function (it's not used by the
 *                library).
 *
 */
extern void peak_stream_set_context(
  peak_stream s,
  void *context
  );


/*!
 * @ingroup stream_low
 * @brief Read bytes on a stream.
 * 
 * @param s       The stream reference.
 * @param buffer  Pointer to an allocated buffer.
 * @param bufsize Size of the allocated buffer.
 * 
 * @return The number of bytes actually read or -1 is an error occured.
 */
extern int peak_stream_read(
  peak_stream s,
  void *buffer,
  size_t bufsize
  );

/*!
 * @ingroup stream_low
 * @brief Read bytes on a stream (advanced).
 * 
 * @param s       The stream reference.
 * @param buffer  Pointer to an allocated buffer.
 * @param bufsize Size of the allocated buffer.
 * @param from    Buffer to a sockaddr structure or NULL.
 * @param fromlen Length of the "from" buffer.
 * 
 * @return The number of bytes actually read or -1 is an error occured.
 */
extern int peak_stream_recvfrom(
  peak_stream s,
  void *buffer,
  size_t bufsize,
  struct sockaddr *from,
  int *fromlen
  );

/*!
 * @ingroup stream_low
 * @brief Write bytes to a stream.
 * 
 * Warning: this function doesn't do write-buffered. For that feature, you
 * need to configure the stream for it, using peak_stream_set_buffered() and
 * then use peak_stream_write_buffered().
 *
 * @param s       The stream reference.
 * @param buffer  The bytes to write.
 * @param bufsize Number of bytes to write.
 * 
 * @return The number of bytes which where written or -1 if an error occured.
 */
extern int peak_stream_write(
  peak_stream s,
  const void *buffer,
  size_t bufsize
  );

/*!
 * @ingroup stream_low
 * @brief Format string and write bytes to a stream.
 * 
 * Warning: this function doesn't do write-buffered. For that feature, you
 * need to configure the stream for it, using peak_stream_set_buffered() and
 * then use peak_stream_msgbuf_make().
 *
 * @param s       The stream reference.
 * @param format  The format followed by the arguments.
 * 
 * @return The number of bytes which where written or -1 if an error occured.
 */
extern int peak_stream_write_format(
  peak_stream s,
  const char *format,
  ...
  );

/*!
 * @ingroup stream_low
 * @brief Format string with arguments and write bytes to a stream.
 * 
 * Warning: this function doesn't do write-buffered. For that feature, you
 * need to configure the stream for it, using peak_stream_set_buffered() and
 * then use peak_stream_msgbuf_vmake().
 *
 * @param s       The stream reference.
 * @param format  The format followed by the arguments.
 * @param vl      The arguments.
 * 
 * @return The number of bytes which where written or -1 if an error occured.
 */
extern int peak_stream_write_vformat(
  peak_stream s,
  const char *format,
  va_list vl
  );

/*!
 * @ingroup stream_low
 * @brief Gather data from iov array and write the bytes to a stream.
 * 
 * @param s       The stream reference.
 * @param iov     A pointer to one or more iovec structures.
 * @param iovcnt  Number of iovec structures provided.
 * 
 * @return The number of bytes which where written or -1 if an error occured.
 */
extern int peak_stream_writev(
  peak_stream s,
  const struct iovec *iov,
  int iovcnt
  );

/*!
 * @ingroup stream_line
 * @brief Test if the stream is currently in line mode.
 * 
 * @param s       The stream reference.
 *
 * @retval 0      if linemode is disabled.
 * @retval 1      if linemode is enabled: you can use peak_stream_get_line().
 */
extern int peak_stream_is_linemode_enabled(
  peak_stream s
  );

/*!
 * @ingroup stream_line
 * @brief Set line mode dynamically.
 * 
 * Note that you cannot disable linemode with the current implementation.
 * 
 * @param s       The stream reference.
 */
extern void peak_stream_set_linemode(
  peak_stream s
  );

/*!
 * @ingroup stream_line
 * @brief Get a pointer to the next line read on a stream.
 *
 * This function loan you a pointer to a mutable buffer. You are allowed to
 * modify it (at least the bytes until the 0 sentinel character). It's
 * useful and efficient for inline tokenizers. Please note this buffer is
 * only valid until the next stream operation.
 * 
 * @param s       The stream reference.
 *
 * @return        A pointer to the line read (mutable buffer).
 */
extern char* peak_stream_get_line(
  peak_stream s
  );

/*!
 * @ingroup stream_common
 * @brief Get amount of bytes actually read from the stream.
 *
 * Statistical function.
 * 
 * @param s       The stream reference.
 *
 * @return        Number of bytes read since the stream's creation.
 */
extern size_t peak_stream_get_read_count(peak_stream s);

/*!
 * @ingroup stream_common
 * @brief Get amount of bytes actually written to the stream.
 *
 * Statistical function.
 * 
 * @param s       The stream reference.
 *
 * @return        Number of bytes written since the stream's creation.
 */
extern size_t peak_stream_get_write_count(peak_stream s);

/*!
 * @ingroup stream_common
 * @brief Configure event timeout checking for a specified stream.
 * 
 * This is a convenience feature. First, because an explicit timer isn't
 * needed to check stream's timeout in your application anymore. But for
 * thread safety too: when peak is not explicitely configured to use only
 * one thread, an independant timer might be processed at the same time
 * with an event on the stream, so you need to add mutual exclusive region
 * (implementation is possible with peak_task_exclusivity()), and verify all
 * it's well synchronized. With peak_stream_set_timeout() however, the timeout
 * event acts like a normal stream events and cannot be processed at the same
 * time  with another on the same stream.
 * 
 * @param s        The stream reference.
 * @param timeout  Time in seconds until an event of type
 *                 \a PEAK_STREAM_EVT_TIMEDOUT will be sent. Use 0 to disable
 *                 timeout checking.
 */
extern void peak_stream_set_timeout(peak_stream s, int timeout);

/*!
 * @ingroup stream_buf
 * @brief Enable or disable buffered write mode for a specified stream.
 * 
 * @param s        The stream reference.
 * @param enable   Enable (1) or disable (0) buffered mode ?
 * @param msg_size Size in bytes of a single message. Take care when setting
 *                 this when you want to write lines: it's the maximum number
 *                 of characters a line can contain when using the
 *                 peak_stream_msgbuf's API. Use 0 to get an optimized value
 *                 if you don't use the peak_stream_msgbuf's API.
 * @param max_size Set a limit of the total size in bytes of all messages, as
 *                 you wish, depending on you needs and traffic. Also known as
 *                 the SendQ's total size, but remember a few more bytes are
 *                 allocated internally to implement the buffered writes.
 * @param cb       A pointer to your errors handling callback function for
 *                 write-buffered related errors or NULL if you don't care.
 *                 But be careful, the write stream isn't reliable anymore if
 *                 you don't handle write-buffered errors because messages
 *                 might be dropped in secret.
 */
extern void peak_stream_set_buffered(
  peak_stream s,
  int enable,
  size_t msg_size,
  size_t max_size,
  peak_stream_error_callback cb
  );

/*!
 * @ingroup stream_buf
 * @brief Get info on current buffered write mode settings.
 *
 * You may only call this function if the stream \a s is configured in
 * write-buffered mode. \a msg_size and \a max_msgs can be configured using
 * the peak_stream_set_buffered() function.
 * 
 * @param s          The stream reference.
 * @param msg_size   If not \p NULL, the value pointed will contain the max
 *                   size in bytes of each message buffers.
 * @param max_msgs   If not \p NULL, the value pointed will contain the
 *                   maximum number of message.
 * @param alloc_msgs If not \p NULL, the value pointed will contain the
 *                   number of actual allocated message buffer
 *                   (\a alloc_msgs <= \a max_msgs).
 * @param queue_msgs If not \p NULL, the value pointed will contain the
 *                   current number of message buffers queued (used). Obviously
 *                   you can get the free count of message buffers with
 *                   (\a max_msgs - \a queue_msgs).
 * @param queue_size If not \p NULL, the value pointed will contain the
 *                   current write queue size in bytes (also named "SendQ").
 */
extern void peak_stream_msgbuf_get_info(
  peak_stream s,
  size_t *msg_size,
  size_t *max_msgs,
  size_t *alloc_msgs,
  size_t *queue_msgs,
  size_t *queue_size
  );

/*!
 * @ingroup stream_buf
 * @brief Copy and commit a buffer to the stream for writing.
 *
 * This is the write-buffered version of peak_stream_write(). If max_msgs
 * (previously configured with peak_stream_set_buffered()) is not reached,
 * this function will send the whole buffer without error.\n
 * If you want maximum efficiency and if it's adapted for you, please use the
 * peak_stream_msgbuf_new() and peak_stream_msgbuf_commit() pair, as they
 * provide zero-copy buffering.
 * 
 * @param s       The stream reference.
 * @param buffer  The bytes to write.
 * @param bufsize Number of bytes to write.
 *
 * @return The number of bytes which where written or -1 if an error occured.
 */
extern void peak_stream_write_buffered(
  peak_stream s,
  const void *buffer,
  size_t bufsize
  );

/*!
 * @ingroup stream_buf
 * @brief Get a new message buffer.
 *
 * Low-level function to get a pointer to a new msgbuf, of msg_size bytes
 * (previously configured with peak_stream_set_buffered()). This function is
 * usually O(1), except for the few first calls or it needs to do real
 * allocations. You can fill it partially or completely and then you MUST
 * call peak_stream_msgbuf_commit() to commit the message for sending.
 * 
 * @param s       The stream reference.
 *
 * @return A pointer to a new msgbuf or NULL is max_msgs is reached internally.
 */
extern void* peak_stream_msgbuf_new(
  peak_stream s
  );

/*!
 * @ingroup stream_buf
 * @brief Commit a message buffer.
 *
 * After a message buffer (obtained with the help of peak_stream_msgbuf_new())
 * has been filled partially or completely, you MUST call this function
 * to commit the message for sending. This function is O(1).
 * 
 * @param s       The stream reference.
 * @param buffer  A filled buffer obtained with peak_stream_msgbuf_new().
 * @param length  Actual length of data you want to commit, you might use
 *                a value of 0 to properly cancel the allocated msgbuf, but
 *                obviously it's not something efficient to do.
 *
 * @return A pointer to a new msgbuf or NULL is max_msgs is reached internally.
 */
extern void peak_stream_msgbuf_commit(
  peak_stream s,
  void *buffer,
  int length
  );

/*!
 * @ingroup stream_buf
 * @brief Make a formatted message buffer and commit it for writing.
 *
 * This convenience function allows you to write a formatted string to the
 * stream in write-buffered mode.
 * 
 * @param s       The stream reference.
 * @param format  Format string.
 *
 */
extern void peak_stream_msgbuf_make(
  peak_stream s,
  const char *format,
  ...
  );

/*!
 * @ingroup stream_buf
 * @brief Make a formatted message buffer and commit it for writing.
 *
 * This convenience function allows you to write a formatted string to the
 * stream in write-buffered mode.
 * 
 * @param s       The stream reference.
 * @param format  Format string.
 * @param vl      Arguments.
 *
 */
extern void peak_stream_msgbuf_vmake(
  peak_stream s,
  const char *format,
  va_list vl
  );


/*!
 * @ingroup stream_sched
 * @brief Schedule a task for a stream.
 *
 * Schedule a task on which you can be notified. It will call your event
 * callback function when an event is available for this stream.
 * 
 * @param s        The stream reference.
 * @param task     The task reference (the current implementation only
 *                 support peak_task_self()).
 * 
 */
extern void peak_stream_schedule(
  peak_stream s,
  peak_task task
  );

/*!
 * @ingroup stream_sched
 * @brief Unschedule a task for a stream.
 * 
 * @param s        The stream reference.
 * @param task     The task reference (the current implementation only
 *                 support peak_task_self()).
 */
extern void peak_stream_unschedule(
  peak_stream s,
  peak_task task
  );



#if defined(__cplusplus)
}
#endif

#endif /* INCLUDED_PEAK_STREAM_H_ */
