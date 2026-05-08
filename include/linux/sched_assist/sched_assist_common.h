/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Stub header for OPLUS sched assist
 * Provides empty implementations for non-OPLUS devices
 */
#ifndef _SCHED_ASSIST_COMMON_H_
#define _SCHED_ASSIST_COMMON_H_

static inline bool test_task_ux(struct task_struct *task) { return false; }

#endif /* _SCHED_ASSIST_COMMON_H_ */
