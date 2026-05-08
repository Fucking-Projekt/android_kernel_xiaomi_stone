/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Stub header for OPLUS Theia event system
 * Provides empty implementations for non-OPLUS devices
 */
#ifndef _THEIA_SEND_EVENT_H_
#define _THEIA_SEND_EVENT_H_

static inline void theia_send_event(int type, int id, unsigned int pid,
				    const char *log_tag, const char *msg) {}

#endif /* _THEIA_SEND_EVENT_H_ */
