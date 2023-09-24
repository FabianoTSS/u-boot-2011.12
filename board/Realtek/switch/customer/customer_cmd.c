/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : RTL83xx SOC and SWCORE commands for U-Boot.
 *
 * Feature : 
 *
 */


/*  
 * Include Files 
 */
#include <rtk_osal.h>
#include <config.h>
#include <common.h>
#include <command.h>
#include <customer/customer_diag.h>

/* 
 * Symbol Definition 
 */

/* 
 * Data Declaration 
 */

/*
 * Macro Definition
 */
#ifndef REG32
#define REG32(reg)   (*(volatile unsigned int *)((unsigned int)reg))
#endif
#ifndef PORTMASK_COUNT
#define PORTMASK_COUNT(port_count)  ((0x1 << (port_count)) - 1)
#endif


/* 
 * Function Declaration 
 */

#if 0
/* Function Name:
 *      do_customer_cmd
 * Description:
 *      Main function of customer commands.
 * Input:
 *      cmdtp, flag, argc, argv handled by the parser
 * Output:
 *      None
 * Return:
 *      [FIXME]
 * Note:
 *      None
 */
int do_customer_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    unsigned int pinNum;
    unsigned int pinStatus;

    if (argc < 2)
    {
        goto usage;
    }

    /*HTP test*/
    if (0 == strcmp(argv[1], "fcTest"))
    {
        run_fc_test();
        return 0;
    }

    /*get external 8231 pin status*/
    if (strcmp(argv[1], "pinGet") == 0)
    {
        if (argc < 3)
        {
            printf("Usage: cst pinGet <pinNum> \n pinNum: 0~36\n\n");
            return 1;
        }
        else
        {
            pinNum = simple_strtoul(argv[2], NULL, 10);
            
            if ((pinNum < 0) || (pinNum > 36))
            {
                printf("The pinNum (%d) is invalid.\n", pinNum);
                return 1;
            }
            else
            {
                rtl8231_pin_status_get(pinNum,&pinStatus);
                printf("pin%d:\t%d\n\n",pinNum,pinStatus);
                return 0;
            }
        }
    }

    /*set external 8231 pin status*/
    if (strcmp(argv[1], "pinSet") == 0)
    {
        if (argc < 4)
        {
            printf("Usage: cst pinSet <pinNum> <status> \n pinNum: 0~36, status: 0/1\n\n");
            return 1;
        }
        else
        {
            pinNum = simple_strtoul(argv[2], NULL, 10);
            pinStatus = simple_strtoul(argv[3], NULL, 10);
            
            if ((pinNum < 0) || (pinNum > 36))
            {
                printf("The pinNum (%d) is invalid.\n", pinNum);
                return 1;
            }
            else if((pinStatus < 0) || (pinStatus > 1))
            {
                printf("The status (%d) is invalid.\n", pinStatus);
                return 1;
            }
            else
            {
                rtl8231_pin_status_set(pinNum,pinStatus);
                printf("pin%d:\t%d\n\n",pinNum,pinStatus);
                return 0;
            }
        }
    }

    

usage:
    printf("Usage:\n%s\n", cmdtp->usage);
    return 1;
} /* end of do_customer_cmd */


U_BOOT_CMD(
    cst, 7, 1, do_customer_cmd,
    "cst     - customer commands\n",
    "object action\n"
    "        - SOC commands.\n"
    "cst fcTest\n"
    "        - Excute factory test\n"
    "cst pinGet [pinNum]\n"
    "        - get external 8231 pin status\n"
    "cst pinSet [pinNum] [status]\n"
    "        - set external 8231 pin status\n"
);
#endif

