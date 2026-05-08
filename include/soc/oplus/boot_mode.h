/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Stub header for OPLUS boot mode
 * Provides empty implementations for non-OPLUS devices
 */
#ifndef _OPLUS_BOOT_MODE_H_
#define _OPLUS_BOOT_MODE_H_

enum {
	MSM_BOOT_MODE__NORMAL = 0,
	MSM_BOOT_MODE__SILENCE = 1,
	MSM_BOOT_MODE__SAU = 2,
};

static inline int get_boot_mode(void) { return MSM_BOOT_MODE__NORMAL; }

#endif /* _OPLUS_BOOT_MODE_H_ */
