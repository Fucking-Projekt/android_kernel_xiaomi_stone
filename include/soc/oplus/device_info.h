/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Stub header for OPLUS device info
 * Provides empty implementations for non-OPLUS devices
 */
#ifndef _OPLUS_DEVICE_INFO_H_
#define _OPLUS_DEVICE_INFO_H_

static inline int register_device_proc(const char *name,
				       const char *version,
				       const char *vendor)
{
	return 0;
}

#endif /* _OPLUS_DEVICE_INFO_H_ */
