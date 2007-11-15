/* Copyright (C) 2003 Stephane Thiell
 *
 * This file is part of pxyservd (from pxys)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id: session.h,v 1.1.1.1 2003/12/30 17:09:33 mbuna Exp $
 */
#ifndef INCLUDED_SESSION_H_
#define INCLUDED_SESSION_H_

/* This module encapsulates the connection with the IRC hub server (a
 * "session").
 */

#include <peak/stream.h>

/* Initialize the session module and start a new IRC session.
 * Will use the global config to find an hub.
 * Returns only when a stopping signal is received or session_stop() is called.
 */
extern void session_run(void);

/* Reload session (ie. configuration) - do not disconnect from server
 */
extern void session_reload(void);

/* Stop the current running session. You shouldn't call this function at
 * interrupt time.
 * You can pass a short description message or NULL here.
 */
extern void session_stop(const char *message);

/* Global uplink stream object, use with care.
*/
extern peak_stream gIRCStream;


#endif /* INCLUDED_SESSION_H_ */
