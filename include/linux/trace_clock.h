/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_TRACE_CLOCK_H
#define _LINUX_TRACE_CLOCK_H

/*
 * 3 trace clock variants, with differing scalability/precision
 * tradeoffs:
 *
 *  -   local: CPU-local trace clock
 *  -  medium: scalable global clock with some jitter
 *  -  global: globally monotonic, serialized clock
 */
#include <linux/compiler.h>
#include <linux/types.h>

#include <asm/trace_clock.h>

#ifdef CONFIG_TRACING
extern u64 notrace trace_clock_local(void);
extern u64 notrace trace_clock(void);
extern u64 notrace trace_clock_jiffies(void);
extern u64 notrace trace_clock_global(void);
extern u64 notrace trace_clock_counter(void);
#else
static inline u64 notrace trace_clock_local(void) { return 0; }
static inline u64 notrace trace_clock(void) { return 0; }
static inline u64 notrace trace_clock_jiffies(void) { return 0; }
static inline u64 notrace trace_clock_global(void) { return 0; }
static inline u64 notrace trace_clock_counter(void) { return 0; }
#endif

#endif /* _LINUX_TRACE_CLOCK_H */
