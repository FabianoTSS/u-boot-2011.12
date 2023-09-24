/*
 * Copyright (C) 2010 Realtek Semiconductor Corp. 
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated, 
 * modified or distributed under the authorized license from Realtek. 
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER 
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED. 
 *
 * $Revision: 10455 $
 * $Date: 2010-06-25 18:27:53 +0800 (Fri, 25 Jun 2010) $
 *
 * Purpose : POE PD69100
 *
 * Feature : POE PD69100
 *
 */

#ifndef __MICROSEMI_PD69100_H__
#define __MICROSEMI_PD69100_H__

/*
 * Include Files
 */

/*
 * Symbol Definition
 */

/*
 * Data Type Declaration
 */
typedef struct poe_pd69100_msg_s
{
    unsigned char   key;
    unsigned char   echo;
    unsigned char   payload[11];
    unsigned char   checksum[2];
} poe_pd69100_msg_t;

typedef enum poe_pd69100_cmd_s
{
    POE_PD69100_CMD_COMMAND = 0,
    POE_PD69100_CMD_RESET_SET,
    POE_PD69100_CMD_TEMP_MATRIX_SET,
    POE_PD69100_CMD_GLOBAL_MATRIX_SET,
    POE_PD69100_CMD_SYSTEM_MASK_SET,
    POE_PD69100_CMD_INDIVIDUAL_MASK_SET,
    POE_PD69100_CMD_PM_METHOD_SET,
    POE_PD69100_CMD_L2_PD_DATA_SET,
    POE_PD69100_CMD_TOTAL_POWER_SET,
    POE_PD69100_CMD_PORT_ADMIN_SET,
    POE_PD69100_CMD_PORT_PRIORITY_SET,
    POE_PD69100_CMD_PORT_POWER_LIMIT_SET,
    POE_PD69100_CMD_FW_DOWNLOAD_SET,

    POE_PD69100_CMD_PROGRAM,
    POE_PD69100_CMD_RESTORE_FACTORY_DEFAULT_SET,

    POE_PD69100_CMD_REQUEST,
    POE_PD69100_CMD_FW_VERSION_GET,
    POE_PD69100_CMD_SYSTEM_STATUS_GET,
    POE_PD69100_CMD_SYSTEM_MASK_GET,
    POE_PD69100_CMD_PM_METHOD_GET,
    POE_PD69100_CMD_TOTAL_POWER_GET,
    POE_PD69100_CMD_PORT_PRIORITY_GET,
    POE_PD69100_CMD_PORT_STATUS_GET,
    POE_PD69100_CMD_PORT_POWER_LIMIT_GET,
    POE_PD69100_CMD_PORT_POWER_MEASUREMENT_GET,
} poe_pd69100_cmd_t;

typedef poe_pd69100_msg_t sys_poe_msg_t;
/*
 * Macro Definition
 */ 

/*
 * Function Declaration
 */

/* Function Name:
 *      poe_pd69100_cmd_set
 * Description:
 *      Set the pd69100 message
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 * Note:
 *      None
 */
extern int poe_pd69100_cmd_set(poe_pd69100_cmd_t cmd, ...);

/* Function Name:
 *      poe_pd69100_cmd_get
 * Description:
 *      get the pd69100 message
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 * Note:
 *      None
 */
extern int poe_pd69100_cmd_get(poe_pd69100_cmd_t cmd, poe_pd69100_msg_t *pMsg, ...);

/* Function Name:
 *      poe_pd69100_fw_download
 * Description:
 *      PD69100 FW download
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 * Note:
 *      None
 */
extern int poe_pd69100_fw_download(void);

/* Function Name:
 *      poe_pd69100_init
 * Description:
 *      initial poe pd69100
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 * Note:
 *      None
 */
extern int poe_pd69100_init(void);

#endif /* __POE_PD69100_H__ */
