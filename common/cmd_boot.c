/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <net.h>

#ifdef CONFIG_INTERRUPT_SUPPORT
#include <asm/mipsregs.h>
#include <interrupt.h>
#endif /* end of CONFIG_INTERRUPT_SUPPORT */

#ifdef CONFIG_INTERRUPT_SUPPORT
extern void __attribute__((nomips16)) __attribute__((weak)) write_ebase(ulong addr);
extern unsigned int __attribute__((nomips16)) __attribute__((weak)) read_ebase(void);
#endif /*CONFIG_INTERRUPT_SUPPORT*/



#ifdef CONFIG_CMD_GO

/* Allow ports to override the default behavior */
__attribute__((weak))
unsigned long do_go_exec (ulong (*entry)(int, char * const []), int argc, char * const argv[])
{
	return entry (argc, argv);
}

int do_go (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong	addr, rc;
	int     rcode = 0;

#ifdef CONFIG_INTERRUPT_SUPPORT
		unsigned int ebase = MIPS_DEFAULT_EBASE;
		unsigned int ebase_org = MIPS_DEFAULT_EBASE;
		ebase_org = read_c0_ebase();
#endif /*CONFIG_INTERRUPT_SUPPORT*/


	if (argc < 2)
		return cmd_usage(cmdtp);

	addr = simple_strtoul(argv[1], NULL, 16);

	printf ("## Starting application at 0x%08lX ...\n", addr);

#ifdef CONFIG_INTERRUPT_SUPPORT
		{ // for INTERRUPT
			common_disable_interrupt();
			write_c0_ebase(ebase);
		}
#endif /*CONFIG_INTERRUPT_SUPPORT*/


	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
	rc = do_go_exec ((void *)addr, argc - 1, argv + 1);
	if (rc != 0) rcode = 1;

	printf ("## Application terminated, rc = 0x%lX\n", rc);
#ifdef CONFIG_INTERRUPT_SUPPORT
	{ // for INTERRUPT
		write_c0_ebase(ebase_org);
		common_enable_interrupt();
	}
#endif /*CONFIG_INTERRUPT_SUPPORT*/
	return rcode;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	go, CONFIG_SYS_MAXARGS, 1,	do_go,
	"start application at address 'addr'",
	"addr [arg ...]\n    - start application at address 'addr'\n"
	"      passing 'arg' as arguments"
);

#endif


extern void menu_shell(void);

int do_menu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    menu_shell();
    return 0;
}


U_BOOT_CMD(
	menu, 1, 0,	do_menu,
	"update the image from PC",
	""
);


U_BOOT_CMD(
	reset, 1, 0,	do_reset,
	"Perform RESET of the CPU",
	""
);
