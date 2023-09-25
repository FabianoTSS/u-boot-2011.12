#include <preloader.h>
#include <mips-generic.h>
#include <configs/autoconf.h>

 .section preloader.uc, "ax", @progbits
 .align 4
 .set noreorder
 .global soc_init
 .ent soc_init
soc_init:
#include "preloader/platform/current/lplr_sram.H"

/* */
 la t0,0xBB00A010
 li t1,0xFFFFFFFF
 sw t1,0(t0)
 nop
 sw t1,4(t0)
 nop
 sw t1,8(t0)
 nop

 /*PORT A*/
 la t0,0xB8003514
 lw t1,0(t0)
 and t2,t1,0xfffcffff
 sw t2,0(t0)
 nop
 /*system led*/
 la t0,0xBB00A000
 lw t1,0(t0)
 and t2,t1,0xffff01ff
 sw t2,0(t0)
 nop

 /*PORT A GPIO pin*/
 la t0,0xB8003500
 lw t1,0(t0)
 and t2,t1,0xfeffffff
 sw t2,0(t0)
 nop

 /*PORT A output pin*/
 la t0,0xB8003508
 lw t1,0(t0)
 or t2,t1,0x01000000
 sw t2,0(t0)
 nop

 /* PORT A*/
 la t0,0xB800350C
 lw t1,0(t0)
 and t2,t1,0xFEFFFFFF
 sw t2,0(t0)
 nop
 #if defined(CONFIG_OS_8G_60W) || defined(CONFIG_OS_8G_150W) || defined(CONIFG_TL_SG2210P) || defined(CONIFG_TL_SG2008)
 /* 
 
 GPIO10-14 JTAG 
 REG32(0xBB001000) &= ~(0x3 << 2);
 REG32(0xBB001000) |= (0x2 << 2);
 */

 la t0,0xBB001000
 lw t1,0(t0)
 and t2,t1,0xFFFFFFF3
 sw t2,0(t0)
 nop 
 
 la t0,0xBB001000
 lw t1,0(t0)
 or t2,t1,0x00000008
 sw t2,0(t0)
 nop


 /* PORT B[6] GPIO pin */
 la t0,0xB8003500
 lw t1,0(t0)
 and t2,t1,0xffbfffff
 sw t2,0(t0)
 nop
 
 /* PORT B[6] output pin */
 la t0,0xB8003508
 lw t1,0(t0)
 or t2,t1,0x00400000
 sw t2,0(t0)
 nop
 
 /* PORT B[6]*/
 la t0,0xB800350C
 lw t1,0(t0)
 or t2,t1,0x00400000
 sw t2,0(t0)
 nop
 
 #elif defined(CONFIG_OS_24G_250W)|| defined(CONFIG_TL_SG3428MP)
 /* 
 
 A[5:4] GPIO ڸ ã Ĭ ״ GPIO
 REG32(0xBB000058) |= 0x3
 REG32(0xBB000038) |= 0x2
 */
 
 la t0,0xBB000058
 lw t1,0(t0)
 or t2,t1,0x00000003
 sw t2,0(t0)
 nop 
 
 la t0,0xBB000038
 lw t1,0(t0)
 or t2,t1,0x00000002
 sw t2,0(t0)
 nop 

 /* PORT A[4] GPIO pin */
 la t0,0xB8003500
 lw t1,0(t0)
 and t2,t1,0xefffffff
 sw t2,0(t0)
 nop

 /* PORT A[4] output pin */
 la t0,0xB8003508
 lw t1,0(t0)
 or t2,t1,0x10000000
 sw t2,0(t0)
 nop

 /* PORT A[4] */
 la t0,0xB800350C
 lw t1,0(t0)
 or t2,t1,0x10000000
 sw t2,0(t0)
 nop
 #elif defined(CONFIG_TL_SG2428) || defined(CONFIG_TL_SG3428)
 /* 
 
 A[5:4] GPIO ڸ ã Ĭ ״ GPIO
 REG32(0xBB000058) |= 0x3
 REG32(0xBB000038) |= 0x2
 */
 
 la t0,0xBB000058
 lw t1,0(t0)
 or t2,t1,0x00000003
 sw t2,0(t0)
 nop 
 
 la t0,0xBB000038
 lw t1,0(t0)
 or t2,t1,0x00000002
 sw t2,0(t0)
 nop 

 /* PORT A[3] GPIO pin */
 la t0,0xB8003500
 lw t1,0(t0)
 and t2,t1,0xf7ffffff
 sw t2,0(t0)
 nop

 /* PORT A[3] output pin */
 la t0,0xB8003508
 lw t1,0(t0)
 or t2,t1,0x08000000
 sw t2,0(t0)
 nop

 /* PORT A[3] */
 la t0,0xB800350C
 lw t1,0(t0)
 or t2,t1,0x08000000
 sw t2,0(t0)
 nop
 #elif defined(CONFIG_RTK_MAPLE)
 /* 
 GPIO10-14 JTAG 
 REG32(0xBB001000) &= ~(0x3 << 2);
 REG32(0xBB001000) |= (0x2 << 2);
 */

 la t0,0xBB001000
 lw t1,0(t0)
 and t2,t1,0xFFFFFFF3
 sw t2,0(t0)
 nop

 la t0,0xBB001000
 lw t1,0(t0)
 or t2,t1,0x00000008
 sw t2,0(t0)
 nop
 
 /* 
 A[5:4] GPIO
 REG32(0xBB000058) |= 0x3
 REG32(0xBB000038) |= 0x2
 */
 la t0,0xBB000058
 lw t1,0(t0)
 or t2,t1,0x00000003
 sw t2,0(t0)
 nop 
 
 la t0,0xBB000038
 lw t1,0(t0)
 or t2,t1,0x00000002
 sw t2,0(t0)
 nop 

 /* л ͵Ŀ ʹ ź */
 
 /* Ƚ PORT A[3] A[4] B[6] Щ gpio֮ ͵ » ޷ */
 la t0,0xB800350C
 lw t1,0(t0)
 or t2,t1,0x18400000
 sw t2,0(t0)
 nop

 /* PORT A[3] A[4] B[6] GPIO pin */
 la t0,0xB8003500
 lw t1,0(t0)
 and t2,t1,0xe7bfffff
 sw t2,0(t0)
 nop

 /* PORT A[3] A[4] B[6] output pin */
 la t0,0xB8003508
 lw t1,0(t0)
 or t2,t1,0x18400000
 sw t2,0(t0)
 nop

 /* PORT A[3] A[4] B[6] */
 la t0,0xB800350C
 lw t1,0(t0)
 or t2,t1,0x18400000
 sw t2,0(t0)
 nop
 #endif 
 
/* memcpy(SRAM_BASE, preloader_uclma, preloader_sz) */
 la t0, preloader_sram_start
 li t1, 0x20000000
 or t0, t0, t1

 la t1, UCSRAM_BASE

 la t2, preloader_sram_size
 addu t2, t0, t2
1:
 lw t3, 0(t0)
 addi t0, t0, 4
 sw t3, 0(t1)
 bne t0, t2, 1b
 addi t1, t1, 4
 nop

 REBASED_CALL(cpu_init, UCSRAM_BASE)

 la t0, soc_init_in_cache
 jr t0
 nop
 .end soc_init

GFUNC(soc_init_in_cache)
 la sp, c_sp_pos
 lw sp, 0(sp)

 la t9, c_start
 jal t9
 nop

 li sp, CONFIG_SYS_TEXT_BASE - 8
 la t9, board_init_f
 j t9
 nop
END(soc_init_in_cache)

GFUNC(soc_ramtest_init)
 subu sp, 0x18
 sw $31,0x14($29) 
 sw $30,0x10($29) 
 addu $30,$29,$0

 la sp, c_sp_pos
 lw sp, 0(sp)

 subu sp, 0x18
 sw $30,0x14($29) 
 
 la t9, plr_dram_test
 jal t9
 nop
 
 lw $30,0x14($29)
 
 addu $29,$30,$0 
 lw $31,0x14($29) ; 
 lw $30,0x10($29) ; 
 addiu $29,$29,0x18 
 jr $31 
 nop
END(soc_ramtest_init)

