/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Stub header for OPLUS project identification
 * Provides empty implementations for non-OPLUS devices
 */
#ifndef _OPLUS_PROJECT_H_
#define _OPLUS_PROJECT_H_

static inline unsigned int get_project(void) { return 0; }
static inline unsigned int is_project(int project) { return 0; }

#endif /* _OPLUS_PROJECT_H_ */
