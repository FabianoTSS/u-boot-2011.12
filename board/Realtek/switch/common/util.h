/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : Related definition of switch util for U-Boot.
 *
 * Feature : switch util function
 *
 */

#ifndef __UTIL_H__
#define __UTIL_H__

/*
 * Include Files
 */
#include <config.h>

/*
 * Symbol Definition
 */
#define MEDIATYPE_COPPER        (0)
#define MEDIATYPE_FIBER         (1)
#define MEDIATYPE_COPPER_AUTO   (2)
#define MEDIATYPE_FIBER_AUTO    (3)

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */
typedef enum rtk_port_crossOver_mode_e
{
    PORT_CROSSOVER_MODE_AUTO = 0,
    PORT_CROSSOVER_MODE_MDI,
    PORT_CROSSOVER_MODE_MDIX,
    PORT_CROSSOVER_MODE_END
} rtk_port_crossOver_mode_t;

/*
 * Function Declaration
 */

/* Function Name:
 *      rtk_portIdxFromMacId
 * Description:
 *      Get PHY index from MAC port id
 * Input:
 *      macId   - MAC port id
 * Output:
 *      portIdx - port index relates MAC port id
 * Return:
 *      None
 * Note:
 *      None
 */
int rtk_portIdxFromMacId(int macId, int *portIdx);

int rtk_phyBaseMacIdFromMacId(int macId, int *baseMacId);

void rtk_phyPatchBit_set(int port, int page, int reg, unsigned char endBit,
    unsigned char startBit, unsigned int inVal);

#if (defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218FB))
extern void rtl8214fc_media_set(int portid, int media);
extern void rtl8214fc_media_get(int portid, int *media);
extern void rtl8214fc_fiber_watchdog(int port);
#endif

extern int rtk_port_linkDownPowerSavingEnable_set(int portid, int enable);
extern int rtk_port_greenEnable_set(int portid, int enable);
extern int rtk_port_phyCrossOverMode_set(int portid, rtk_port_crossOver_mode_t mode);

#endif  /* __UTIL_H__ */
