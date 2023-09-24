/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : Related definition of the Switch initalize for U-Boot.
 *
 * Feature : Switch initalize
 *
 */


/*
 * Include Files
 */
#include <common.h>
#include <command.h>
#include <asm/addrspace.h>
#include <asm/mipsregs.h>
#include <asm/io.h>
#include <watchdog.h>
#include <rtk_osal.h>
#include <rtk_reg.h>
#include <rtk_switch.h>
#include <rtk_debug.h>
#include <rtk/drv/gpio/ext_gpio.h>
#include <rtk/phy/rtl8214f.h>
#include <rtk/mac/mac_init.h>
#if defined(CONFIG_RTL8380)
#include <rtk/mac/rtl8380/rtl8380_swcore_reg.h>
#endif
#if defined(CONFIG_RTL8390)
#include <rtk/mac/rtl8390/rtl8390_swcore_reg.h>
#endif
#if defined(CONFIG_CUSTOMER_BOARD)
#include <customer/mac_init.h>
#endif

#include <rtk/rtk_probe.h>
#include <probe.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
rtk_switch_model_t *gSwitchModel = NULL;
const rtk_mac_drv_t *gMacDrv = NULL;
rtk_gpio_drv_t *gIntGpio = NULL;
rtk_extGpio_drv_t *gExtGpio = NULL;

#define PHY_PAGE_0                              (0)
#define PHY_IDENTIFIER_2_REG                    (3)
#define    PHY_8218D_IDENTIFIER (0xc983)

typedef int32 (*hal_get_chip_id_f)(uint32 *pChip_index);

#if defined(CONFIG_RTL8380)
static int32 _bsp_drv_swcore_cid8380_get(uint32 *pChip_index);
#endif
#if defined(CONFIG_RTL8390)
static int32 _bsp_drv_swcore_cid8390_get(uint32 *pChip_index);
#endif

static hal_get_chip_id_f func[] =
{
#if defined(CONFIG_RTL8380)
    _bsp_drv_swcore_cid8380_get,
#endif
#if defined(CONFIG_RTL8390)
    _bsp_drv_swcore_cid8390_get,
#endif
};


/*
 * Macro Definition
 */

/*
 * Function Definition
 */
#if defined(CONFIG_MISC_INIT_R)
#if defined(CONFIG_RTL8380)
int32 _bsp_drv_swcore_cid8380_get(uint32 *pChip_index)
{
    uint32 temp;
    uint32 reg_val;
    uint32 sub_version;

    temp = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_MODEL_NAME_INFO_ADDR);
    if ((((temp>>16)&0xFFFF) != 0x8330) && (((temp>>16)&0xFFFF) != 0x8332) &&
        (((temp>>16)&0xFFFF) != 0x8380) && (((temp>>16)&0xFFFF) != 0x8382))
        return 1;

    temp = (temp >> 16) & 0xffff;

    switch (temp)
    {
        case 0x8330:
            *pChip_index = RTK_CHIP_RTL8330M;
            return 0;
        case 0x8332:
            *pChip_index = RTK_CHIP_RTL8332M;
            return 0;
        case 0x8380:
            reg_val = MEM32_READ(SWCORE_BASE_ADDR | (0x58));
            MEM32_WRITE(SWCORE_BASE_ADDR | (0x58), 0x3);
            sub_version = MEM32_READ(SWCORE_BASE_ADDR | (0x1024));
            sub_version = sub_version & (1UL<<23);
            if(sub_version == 0)
            {
                *pChip_index = RTK_CHIP_RTL8381M;
            }
            else
            {
                *pChip_index = RTK_CHIP_RTL8380M;
            }

#if 0
            printf("reg_val=0x%8x\n",reg_val);
            printf("sub_version=0x%8x\n",sub_version);
#endif

            MEM32_WRITE(SWCORE_BASE_ADDR | (0x58), reg_val);
            return 0;
        case 0x8382:
            *pChip_index = RTK_CHIP_RTL8382M;
            return 0;

        default:
            return 1;
    }
}
#endif

#if defined(CONFIG_RTL8390)
int32 _bsp_drv_swcore_cid8390_get(uint32 *pChip_index)
{
    uint32 model_info = 0;

    model_info = MEM32_READ(SWCORE_BASE_ADDR | RTL8390_MODEL_NAME_INFO_ADDR);
    model_info &= 0xFFFFFFC0;

    switch (model_info)
    {
        case RTL8351M_CHIP_ID:
            *pChip_index = RTK_CHIP_RTL8351M;
            return 0;
        case RTL8352M_CHIP_ID:
            *pChip_index = RTK_CHIP_RTL8352M;
            return 0;
        case RTL8353M_CHIP_ID:
            *pChip_index = RTK_CHIP_RTL8353M;
            return 0;
        case RTL8391M_CHIP_ID:
            *pChip_index = RTK_CHIP_RTL8391M;
            return 0;
        case RTL8392M_CHIP_ID:
            *pChip_index = RTK_CHIP_RTL8392M;
            return 0;
        case RTL8393M_CHIP_ID:
            *pChip_index = RTK_CHIP_RTL8393M;
            return 0;
        case RTL8396M_CHIP_ID:
            *pChip_index = RTK_CHIP_RTL8396M;
            return 0;

        default:
            return 1;
    }
}
#endif

void swcore_probe(uint32 *pChip_index)
{
    uint32  i;
    hal_get_chip_id_f f;

    if (pChip_index == NULL)
    {
        OSAL_PRINTF("pChip_index is NULL!\n");
        return;
    }

    for (i = 0; i < (sizeof(func)/sizeof(hal_get_chip_id_f)); i++)
    {
        f = (hal_get_chip_id_f) func[i];
        if (0 == ((f)(pChip_index)))
            return;
    }

    printf("*pChip_index=%8x\n",*pChip_index);

    OSAL_PRINTF("Proper chid ID is not found!\n");
    return;
}

void board_phy_adapt(void)
{
    unsigned int phyid;
    unsigned int phy0_macid;
    unsigned int val;
    unsigned int portIdx;

    for (portIdx = 0; portIdx < gSwitchModel->port.count; portIdx++)
    {
        phyid = gSwitchModel->port.list[portIdx].phy;
        phy0_macid = gSwitchModel->phy.list[gSwitchModel->port.list[portIdx].phy_idx].mac_id;
		

		if(RTK_CHIP_RTL8218B == gSwitchModel->phy.list[gSwitchModel->port.list[portIdx].phy_idx].chip)
		{
			val = 0;
			gMacDrv->drv_miim_read(phy0_macid+0, PHY_PAGE_0, PHY_IDENTIFIER_2_REG, (unsigned int *)&val);
			
			if(val == PHY_8218D_IDENTIFIER)
			{
				gSwitchModel->phy.list[gSwitchModel->port.list[portIdx].phy_idx].chip = RTK_CHIP_RTL8218D;
			}
		}
    }
	
	return;
}

void rtl8380_serdes_patch(void)
{
    unsigned int portIdx;

    for (portIdx = 0; portIdx < gSwitchModel->port.count; portIdx++)
    {	
		if(RTK_CHIP_RTL8218D == gSwitchModel->phy.list[gSwitchModel->port.list[portIdx].phy_idx].chip)
		{
			MEM32_WRITE(0xbb00f464, 0x6C03);
			MEM32_WRITE(0xbb00f564, 0x6C03);
			MEM32_WRITE(0xbb00f664, 0x6C03);
			MEM32_WRITE(0xbb00f764, 0x6C03);			
			return;
		}
    }
	
	return;
}

void rtl8390_serdes_patch(void)
{
    unsigned int portIdx;

    for (portIdx = 0; portIdx < gSwitchModel->port.count; portIdx++)
    {	
		if(RTK_CHIP_RTL8218D == gSwitchModel->phy.list[gSwitchModel->port.list[portIdx].phy_idx].chip)
		{
			MEM32_WRITE(0xbb00afb0, 0x57720306);
			MEM32_WRITE(0xbb00b730, 0x57720306);
			MEM32_WRITE(0xbb00b7b0, 0x57720306);
	
			MEM32_WRITE(0xbb000000, 0x0);
			MEM32_WRITE(0xbb00b350, 0x33ec080);
			MEM32_WRITE(0xbb00b3d0, 0x33ec080);
			MEM32_WRITE(0xbb000000, 0xc00);			
			return;
		}
    }
	
	return;
}

int rtk_serdes_patch(int chip_index)
{
    switch (chip_index)
    {
#if defined(CONFIG_RTL8328)
        case RTK_CHIP_RTL8328M:
        case RTK_CHIP_RTL8328S:
        case RTK_CHIP_RTL8328L:
            break;
#endif /* end of #if defined(CONFIG_RTL8328) */
#if defined(CONFIG_RTL8390)
        case RTK_CHIP_RTL8391M:
        case RTK_CHIP_RTL8392M:
        case RTK_CHIP_RTL8393M:
        case RTK_CHIP_RTL8396M:
        case RTK_CHIP_RTL8351M:
        case RTK_CHIP_RTL8352M:
        case RTK_CHIP_RTL8353M:
			rtl8390_serdes_patch();
            break;
#endif /* end of #if defined(CONFIG_RTL8390) */
#if defined(CONFIG_RTL8380)
        case RTK_CHIP_RTL8382M:
        case RTK_CHIP_RTL8381M:
        case RTK_CHIP_RTL8380M:
        case RTK_CHIP_RTL8332M:
        case RTK_CHIP_RTL8330M:
			rtl8380_serdes_patch();
            break;
#endif /* end of #if defined(CONFIG_RTL8380) */
        default:
            break;
    }

    return 0;
}







/* Function Name:
 *      misc_init_r
 * Description:
 *      Initialize of misc objects.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0 - Success
 * Note:
 *      None
 */
int misc_init_r(void)
{
    uint32 chip_index;

    /* board probe */
    DBG_PRINT(1, "### Board Probe ###\n");

    swcore_probe(&chip_index);

    /* int-gpio chip driver init*/
    intGpio_drv_init(chip_index, &gIntGpio);

    /* ext-gpio chip driver init*/
    extGpio_drv_init(chip_index, &gExtGpio);

    board_probe((rtk_switch_model_t **)&gSwitchModel);


#ifdef __TEST_FT2__
        printf("{B}\n");
        printf("{VL01}\n");
#endif

    if (gSwitchModel == NULL)
    {
        printf("Fatal: gSwitchModel is NULL\n");
        return 0;
    }

    /* mac driver init */
    DBG_PRINT(1, "### Mac Driver Init ###\n");
    mac_drv_init(gSwitchModel->chip, (rtk_mac_drv_t **)&gMacDrv);

    if (gMacDrv == NULL)
    {
        printf("Fatal: gMacDrv is NULL\n");
        return 0;
    }
	board_phy_adapt();

    /* chip config */
    DBG_PRINT(1, "### Chip Config ###\n");
    chip_config(gSwitchModel);
	
	rtk_serdes_patch(gSwitchModel->chip);

    return 0;
} /* end of misc_init_r */
#endif

