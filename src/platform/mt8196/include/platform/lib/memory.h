/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright(c) 2024 MediaTek. All rights reserved.
 *
 * Author: Hailong Fan <hailong.fan@mediatek.com>
 */

#ifdef __SOF_LIB_MEMORY_H__

#ifndef __PLATFORM_LIB_MEMORY_H__
#define __PLATFORM_LIB_MEMORY_H__

#include <rtos/cache.h>

/* data cache line alignment */
#define PLATFORM_DCACHE_ALIGN sizeof(void *)

/* physical DSP addresses */
#define SRAM_BASE			0x4e100000
#define SRAM_SIZE			0x80000

/* reset vector + rodata + module_init + text + data + bss */
#define SOF_DATA_SIZE			0x80000
#define VECTOR_SIZE			0x700
#define SRAM_START			(SRAM_BASE + VECTOR_SIZE)

#define DRAM_BASE			0x90000000
#define DRAM0_SIZE			0x500000
#define MAILBOX_BASE			(DRAM_BASE + DRAM0_SIZE)
#define DSP_SYS_SIZE			0xA00000

#define UUID_ENTRY_ELF_BASE		(SRAM_BASE + SOF_DATA_SIZE)
#define UUID_ENTRY_ELF_SIZE		0x6000

#define LOG_ENTRY_ELF_BASE		(UUID_ENTRY_ELF_BASE + UUID_ENTRY_ELF_SIZE)
#define LOG_ENTRY_ELF_SIZE		0x20000
#define EXT_MANIFEST_ELF_BASE		(LOG_ENTRY_ELF_BASE + LOG_ENTRY_ELF_SIZE)
#define EXT_MANIFEST_ELF_SIZE		(SRAM_BASE + SRAM_SIZE - EXT_MANIFEST_ELF_BASE)

/*
 * The Memory Layout on MT8189 are organised like this :-
 *
 * +--------------------------------------------------------------------------+
 * | Offset              | Region         |  Size                             |
 * +---------------------+----------------+-----------------------------------+
 * | SRAM_START          | RO Data        |  SOF_DATA_SIZE                    |
 * |                     | text           |                                   |
 * |                     | Data           |                                   |
 * |                     | BSS            |                                   |
 * +---------------------+----------------+-----------------------------------+
 * | fw_ready                                                                 |
 * +---------------------+----------------+-----------------------------------+
 * | static_uuid                                                              |
 * +---------------------+----------------+-----------------------------------+
 * | static_log                                                               |
 * +---------------------+----------------+-----------------------------------+
 * | fw_metadata                                                              |
 * +---------------------+----------------+-----------------------------------+
 *
 * +---------------------+----------------+-----------------------------------+
 * | SOF_DRAM_BASE       |                |  SOF_DRAM_SIZE                    |
 * +---------------------+----------------+-----------------------------------+
 * | HEAP_SYSTEM_BASE    | System Heap    |  HEAP_SYSTEM_SIZE                 |
 * +---------------------+----------------+-----------------------------------+
 * | HEAP_RUNTIME_BASE   | Runtime Heap   |  HEAP_RUNTIME_SIZE                |
 * +---------------------+----------------+-----------------------------------+
 * | HEAP_BUFFER_BASE    | Module Buffers |  HEAP_BUFFER_SIZE                 |
 * +---------------------+----------------+-----------------------------------+
 * | SOF_STACK_END       | Stack          |  SOF_STACK_SIZE                   |
 * +---------------------+----------------+-----------------------------------+
 * | SOF_STACK_BASE      |                |                                   |
 * +---------------------+----------------+-----------------------------------+
 * | MAILBOX_BASE        |                |  SOF_MAILBOX_SIZE                 |
 * +---------------------+----------------+-----------------------------------+
 */

#define SRAM_OUTBOX_BASE	MAILBOX_BASE
#define SRAM_OUTBOX_SIZE	0x1000
#define SRAM_OUTBOX_OFFSET	0

#define SRAM_INBOX_BASE		(SRAM_OUTBOX_BASE + SRAM_OUTBOX_SIZE)
#define SRAM_INBOX_SIZE		0x1000
#define SRAM_INBOX_OFFSET	SRAM_OUTBOX_SIZE

#define SRAM_DEBUG_BASE		(SRAM_INBOX_BASE + SRAM_INBOX_SIZE)
#define SRAM_DEBUG_SIZE		0x800
#define SRAM_DEBUG_OFFSET	(SRAM_INBOX_OFFSET + SRAM_INBOX_SIZE)

#define SRAM_EXCEPT_BASE	(SRAM_DEBUG_BASE + SRAM_DEBUG_SIZE)
#define SRAM_EXCEPT_SIZE	0x800
#define SRAM_EXCEPT_OFFSET	(SRAM_DEBUG_OFFSET + SRAM_DEBUG_SIZE)

#define SRAM_STREAM_BASE	(SRAM_EXCEPT_BASE + SRAM_EXCEPT_SIZE)
#define SRAM_STREAM_SIZE	0x1000
#define SRAM_STREAM_OFFSET	(SRAM_EXCEPT_OFFSET + SRAM_EXCEPT_SIZE)

#define SRAM_TRACE_BASE		(SRAM_STREAM_BASE + SRAM_STREAM_SIZE)
#define SRAM_TRACE_SIZE		0x1000
#define SRAM_TRACE_OFFSET	(SRAM_STREAM_OFFSET + SRAM_STREAM_SIZE)

/*4K + 4K +2K + 2K + 4K + 4K = 20KB*/
#define SOF_MAILBOX_SIZE                                        \
	(SRAM_INBOX_SIZE + SRAM_OUTBOX_SIZE + SRAM_DEBUG_SIZE + \
	 SRAM_EXCEPT_SIZE + SRAM_STREAM_SIZE + SRAM_TRACE_SIZE)

/* SOF image on DRAM */
#define SOF_DRAM_BASE	DRAM_BASE
#define SOF_DRAM_SIZE	0x100000

/* Heap section sizes for module pool */
#define HEAP_RT_COUNT8		0
#define HEAP_RT_COUNT16		48
#define HEAP_RT_COUNT32		48
#define HEAP_RT_COUNT64		32
#define HEAP_RT_COUNT128	32
#define HEAP_RT_COUNT256	32
#define HEAP_RT_COUNT512	32
#define HEAP_RT_COUNT1024	4
#define HEAP_RT_COUNT2048	2
#define HEAP_RT_COUNT4096	2

/* Heap section sizes for system runtime heap */
#define HEAP_SYS_RT_COUNT64	128
#define HEAP_SYS_RT_COUNT512	16
#define HEAP_SYS_RT_COUNT1024	8

/* Heap configuration */

#define HEAP_SYSTEM_BASE	(SOF_DRAM_BASE + SOF_DRAM_SIZE)
#define HEAP_SYSTEM_SIZE	0x6000

#define HEAP_SYS_RUNTIME_BASE	(HEAP_SYSTEM_BASE + HEAP_SYSTEM_SIZE)
/*24KB*/
#define HEAP_SYS_RUNTIME_SIZE                                    \
	(HEAP_SYS_RT_COUNT64 * 64 + HEAP_SYS_RT_COUNT512 * 512 + \
	 HEAP_SYS_RT_COUNT1024 * 1024)

#define HEAP_RUNTIME_BASE (HEAP_SYS_RUNTIME_BASE + HEAP_SYS_RUNTIME_SIZE)
/*48*(16 +32) + 32*(64 128+256) + 4*(512+1024) + 1*2048 = 24832 = 24.25KB*/
#define HEAP_RUNTIME_SIZE                                        \
	(HEAP_RT_COUNT8 * 8 + HEAP_RT_COUNT16 * 16 +             \
	 HEAP_RT_COUNT32 * 32 + HEAP_RT_COUNT64 * 64 +           \
	 HEAP_RT_COUNT128 * 128 + HEAP_RT_COUNT256 * 256 +       \
	 HEAP_RT_COUNT512 * 512 + HEAP_RT_COUNT1024 * 1024 +     \
	 HEAP_RT_COUNT2048 * 2048 + HEAP_RT_COUNT4096 * 4096)

#define HEAP_BUFFER_BASE (HEAP_RUNTIME_BASE + HEAP_RUNTIME_SIZE)
#define HEAP_BUFFER_SIZE                                           \
	(DRAM0_SIZE - SOF_DRAM_SIZE - HEAP_RUNTIME_SIZE - SOF_STACK_TOTAL_SIZE -   \
	 HEAP_SYS_RUNTIME_SIZE - HEAP_SYSTEM_SIZE)

#define HEAP_BUFFER_BLOCK_SIZE		0x100
#define HEAP_BUFFER_COUNT		(HEAP_BUFFER_SIZE / HEAP_BUFFER_BLOCK_SIZE)

#define PLATFORM_HEAP_SYSTEM		1 /* one per core */
#define PLATFORM_HEAP_SYSTEM_RUNTIME	1 /* one per core */
#define PLATFORM_HEAP_RUNTIME		1
#define PLATFORM_HEAP_BUFFER		1

/* Stack configuration */
#define SOF_STACK_SIZE			(CONFIG_SOF_STACK_SIZE)
#define SOF_STACK_TOTAL_SIZE		SOF_STACK_SIZE /*4KB*/
#define SOF_STACK_BASE			(DRAM_BASE + DRAM0_SIZE)
#define SOF_STACK_END			(SOF_STACK_BASE - SOF_STACK_TOTAL_SIZE)

/* Vector and literal sizes - not in core-isa.h */
#define SOF_MEM_VECT_LIT_SIZE		0x4
#define SOF_MEM_VECT_TEXT_SIZE		0x1c
#define SOF_MEM_VECT_SIZE		(SOF_MEM_VECT_TEXT_SIZE + SOF_MEM_VECT_LIT_SIZE)

#define SOF_MEM_RESET_TEXT_SIZE		0x2e0
#define SOF_MEM_RESET_LIT_SIZE		0x120
#define SOF_MEM_VECBASE_LIT_SIZE	0x178

#define SOF_MEM_RO_SIZE			0x8

#define HEAP_BUF_ALIGNMENT		DCACHE_LINE_SIZE

/** \brief EDF task's default stack size in bytes. */
#define PLATFORM_TASK_DEFAULT_STACK_SIZE	3072

#if !defined(__ASSEMBLER__) && !defined(LINKER)

struct sof;

/**
 * \brief Data shared between different cores.
 * Does nothing, since mt8186 doesn't support SMP.
 */
#define SHARED_DATA

void platform_init_memmap(struct sof *sof);

static inline void *platform_shared_get(void *ptr, int bytes)
{
	return ptr;
}

#define uncache_to_cache(address)	address
#define cache_to_uncache(address)	address
#define cache_to_uncache_init(address)	address
#define is_uncached(address)		0

/**
 * \brief Function for keeping shared data synchronized.
 * It's used after usage of data shared by different cores.
 * Such data is either statically marked with SHARED_DATA
 * or dynamically allocated with SOF_MEM_FLAG_SHARED flag.
 * Does nothing, since mt8186 doesn't support SMP.
 */

static inline void *platform_rfree_prepare(void *ptr)
{
	return ptr;
}

#endif

#define host_to_local(addr)	(addr)
#define local_to_host(addr)	(addr)

#endif /* __PLATFORM_LIB_MEMORY_H__ */

#else

#error "This file shouldn't be included from outside of sof/lib/memory.h"

#endif /* __SOF_LIB_MEMORY_H__ */
