#include "../../include/rtk_soc_common.h"

#define IP_BUSY (1)
#define IP_IDLE (0)
#define CHECKING_FAIL (-1)
#define CHECKING_PASS (1)
#define NO_SUCH_CASE (0)


//#define _cache_flush_all rlx5281_cache_flush_dcache

#define COUNTER_ADDR 		(0xA0800000)
#define A0_ADDR 		(COUNTER_ADDR+32)

#define COUNTER_MODE_ZERO 	(0x0) //cycles, ifetches, ld+stores, dcache miss
#define COUNTER_MODE_ONE 	(0x1) //cycles, ifetches, icache miss, dcache miss
#define COUNTER_MODE_TWO 	(0x2) //cycles, ifetches, icache miss, icache busy
#define COUNTER_MODE_THREE 	(0x3) //cycles, dmiss busy, ld+stores, dcache miss

#define COUNTER_MODE COUNTER_MODE_TWO

#define PHYSADDR(a)             ((unsigned int)(a) & 0x1fffffff)
/*4KB IMEM0*/
#define IMEM0_SIZE (0x1000)
/*4KB IMEM1*/
#define IMEM1_SIZE (0x1000)
/*4KB DMEM2*/
#define DMEM0_SIZE (0x1000)
/*4KB DMEM1*/
#define DMEM1_SIZE (0x1000)
/*32KB L2MEM*/
//#define L2MEM_SIZE (0x8000)
#define L2MEM_SIZE (0)

#define IMEM_SIZE (0x4000)
#define DMEM_SIZE (0x2000)

#define STACK_ADDR 		(0xA0600000)
#define TARGET_CPU_CASE_ADDR 	(0xA0400000)
#define TARGET_IMEM0_ADDR 	(0xA1A00000)
#define TARGET_IMEM1_ADDR 	(0xA1A80000)
#define TARGET_DMEM0_ADDR 	(0xA1300000)
#define TARGET_DMEM1_ADDR 	(0xA1380000)
#define TARGET_L2MEM_ADDR 	(0xA1400000)
#define TARGET_BACKUP_ADDR 	(0xA1500000)

//For rlx5181
#define TARGET_IMEM_ADDR 0xA0200000
#define TARGET_DMEM_ADDR 0xA0300000

#define CACHED_CPU_CASE_ADDR ((TARGET_CPU_CASE_ADDR) & 0xDFFFFFFF)
#define CACHED_TARGET_IMEM0_ADDR ((TARGET_IMEM0_ADDR) & 0xDFFFFFFF)
#define CACHED_TARGET_IMEM1_ADDR ((TARGET_IMEM1_ADDR) & 0xDFFFFFFF)
#define CACHED_TARGET_DMEM0_ADDR ((TARGET_DMEM0_ADDR) & 0xDFFFFFFF)
#define CACHED_TARGET_DMEM1_ADDR ((TARGET_DMEM1_ADDR) & 0xDFFFFFFF)
#define CACHED_TARGET_L2MEM_ADDR ((TARGET_L2MEM_ADDR) & 0xDFFFFFFF)
#define CACHED_TARGET_BACKUP_ADDR ((TARGET_BACKUP_ADDR) & 0xDFFFFFFF)

#define CACHED_TARGET_IMEM_ADDR ((TARGET_IMEM_ADDR) & 0xDFFFFFFF)
#define CACHED_TARGET_DMEM_ADDR ((TARGET_DMEM_ADDR) & 0xDFFFFFFF)
#define IMEM0_BASE_ADDR PHYSADDR(TARGET_IMEM0_ADDR)
#define IMEM0_TOP_ADDR PHYSADDR(TARGET_IMEM0_ADDR+IMEM0_SIZE-1)
#define IMEM1_BASE_ADDR PHYSADDR(TARGET_IMEM1_ADDR)
#define IMEM1_TOP_ADDR PHYSADDR(TARGET_IMEM1_ADDR+IMEM1_SIZE-1)
#define DMEM0_BASE_ADDR PHYSADDR(TARGET_DMEM0_ADDR)
#define DMEM0_TOP_ADDR PHYSADDR(TARGET_DMEM0_ADDR+DMEM0_SIZE-1)
#define DMEM1_BASE_ADDR PHYSADDR(TARGET_DMEM1_ADDR)
#define DMEM1_TOP_ADDR PHYSADDR(TARGET_DMEM1_ADDR+DMEM1_SIZE-1)
#define L2MEM_BASE_ADDR PHYSADDR(TARGET_L2MEM_ADDR)
#define L2MEM_TOP_ADDR PHYSADDR(TARGET_L2MEM_ADDR+L2MEM_SIZE-1)

//for rlx5181
#define IMEM_BASE_ADDR PHYSADDR(TARGET_IMEM_ADDR)
#define IMEM_TOP_ADDR PHYSADDR(TARGET_IMEM_ADDR+IMEM_SIZE-1)
#define DMEM_BASE_ADDR PHYSADDR(TARGET_DMEM_ADDR)
#define DMEM_TOP_ADDR PHYSADDR(TARGET_DMEM_ADDR+DMEM_SIZE-1)


