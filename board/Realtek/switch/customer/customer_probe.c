/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : Related definition of the board probe for U-Boot.
 *
 * Feature : board probe function
 *
 */


/*
 * Include Files
 */
#include <config.h>
#include <rtk_osal.h>
#include <rtk_switch.h>
#include <init.h>
#include <customer/loader/customer_board.c>

/*
 * Symbol Definition
 */
#define ENV_BOARD_MODEL     "boardmodel"

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

/* Function Name:
 *      customer_board_probe
 * Description:
 *      Probe the customer board
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0 - Success
 * Note:
 *      None
 */
int customer_board_probe(rtk_switch_model_t **pSwitchModel)
{
    char    *board_model;

    board_model = getenv(ENV_BOARD_MODEL);

    {   /* Not match customer board */
        (*pSwitchModel) = NULL;
    }

    return 0;
} /* end of customer_board_probe */
