/*
 * Copyright(c) Realtek Semiconductor Corporation, 2012
 * All rights reserved.
 *
 * Purpose : RTL8380 SOC commands for U-Boot.
 *
 * Feature :
 *
 */


/*
 * Include Files
 */
#include <common.h>
#include <command.h>
#include <rtk_reg.h>
#include <rtk_osal.h>
#include <rtk/drv/rtl8231/rtl8231_drv.h>

#include <rtk/drv/swled/swctrl_led_main.h>
#if defined(CONFIG_RTL8380)
#include <rtk/mac/rtl8380/rtl8380_rtk.h>
#include <rtk/mac/rtl8380/rtl8380_mdc_mdio.h>
#include <rtk/mac/rtl8380/rtl8380_swcore_reg.h>
#endif

#if defined(CONFIG_RTL8390)
#include <rtk/mac/rtl8390/rtl8390_drv.h>
#include <rtk/mac/rtl8390/rtl8390_mdc_mdio.h>
#include <rtk/mac/rtl8390/rtl8390_rtk.h>
#include <rtk/mac/rtl8390/rtl8390_swcore_reg.h>
extern const rtk_switch_model_t *gSwitchModel;
#endif

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
/*
 * Macro Definition
 */


/*
 * Function Declaration
 */

static void swled_init(void);
static void swled_uninit(void);

unsigned int val_led_glb_ctrl;


#if defined(CONFIG_RTL8390)
#define MAX_PORT 52
#define LEDON(index) (0x07 << (3*index))
#define LED_MASK(index) (0x07 << (3*index))
#define LEDFIBON(index) (0x07 << (3*(index+3)))
#define LEDFIB_MASK(index) (0x07 << (3*(index+3)))
int max_portled;

static int32 swled_isfiber(int port,int index)
{
	int ret_val=0;
	int reg_index;
	unsigned int fib_pmask,led_combo,portmask;
	
	if (port > 32)
	{
		reg_index=1;
		portmask = 1<<(port-32);
	}
	else
	{
		reg_index=0;
		portmask = 1 << port;
	}
	fib_pmask = gSwitchModel->led.fib_pmask[reg_index];
	led_combo = gSwitchModel->led.led_combo[reg_index];
	if ((portmask & fib_pmask) && ((portmask & led_combo)== 0))
		return 1;
	return ret_val;
}

static void swled_init()
{
	int port;
	unsigned int value;
	if (gSwitchModel != 0)
	max_portled = gSwitchModel->led.num;
	else
		max_portled = 3;

	val_led_glb_ctrl = MEM32_READ(SWCORE_BASE_ADDR | RTL8390_LED_GLB_CTRL_ADDR);
	
	for (port=0;port<MAX_PORT;port++)
	{
		value = MEM32_READ(SWCORE_BASE_ADDR | RTL8390_LED_SW_P_EN_CTRL_ADDR(port));
		value |= RTL8390_LED_SW_P_EN_CTRL_SW_CTRL_LED_EN_MASK(port);
		MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_LED_SW_P_EN_CTRL_ADDR(port), value);
	}
	return;
	
}

static void swled_uninit()
{
	int port;
	unsigned int value;
	for (port=0;port<MAX_PORT;port++)
	{
		value = MEM32_READ(SWCORE_BASE_ADDR | RTL8390_LED_SW_P_EN_CTRL_ADDR(port));
		value &= ~RTL8390_LED_SW_P_EN_CTRL_SW_CTRL_LED_EN_MASK(port);
		MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_LED_SW_P_EN_CTRL_ADDR(port), value);
	}
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_LED_GLB_CTRL_ADDR, val_led_glb_ctrl);
	
	#ifdef CONFIG_CUSTOMER_BOARD
	customer_led_config_init(gSwitchModel);
	#endif
	return;
	
}

static unsigned int portindex_ledon(int port,int index)
{
	if (swled_isfiber(port,index) == 1)
				return LEDFIBON(index);
		  else 
		  	return LEDON(index);
		}

static void swled_out(unsigned int *bit)
{
	int port;
	
	for (port=0;port<MAX_PORT;port++)
	{
		MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_LED_SW_P_CTRL_ADDR(port),bit[port]);
	}
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_LED_SW_CTRL_ADDR,1);
	__asm__ volatile ("sync");
	udelay(300000);
	return;
}

void swportled_set(int port,int index,unsigned int val)
{
	unsigned int value;

	if (gSwitchModel == 0)
	{
		printf("\n led test: Cant get board model\n");
		return;
	}
	value = MEM32_READ(SWCORE_BASE_ADDR | RTL8390_LED_SW_P_EN_CTRL_ADDR(port));
	value &= ~RTL8390_LED_SW_P_EN_CTRL_SW_CTRL_LED_EN_MASK(port);
	if (index == 0)
	{
		value |= 0x01<<RTL8390_LED_SW_P_EN_CTRL_SW_CTRL_LED_EN_OFFSET(port);
	}
	else if (index == 1)
	{
		value |= 0x02<<RTL8390_LED_SW_P_EN_CTRL_SW_CTRL_LED_EN_OFFSET(port);
	}
	else if (index == 2)
	{
		value |= 0x04<<RTL8390_LED_SW_P_EN_CTRL_SW_CTRL_LED_EN_OFFSET(port);
	}
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_LED_SW_P_EN_CTRL_ADDR(port), value);

	value = MEM32_READ(SWCORE_BASE_ADDR | RTL8390_LED_SW_P_CTRL_ADDR(port));
	value &= ~LED_MASK(index);
	value |= (val << (3*index));
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_LED_SW_P_CTRL_ADDR(port),value);
	
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_LED_SW_CTRL_ADDR,1);
	__asm__ volatile ("sync");
	return;
}
#endif
#if defined(CONFIG_RTL8380)
#define MAX_PORT 28
#define LEDON(index) (0x05 << (3*index))
#define LED_MASK(index) (0x07 << (3*index))
int max_portled;
unsigned int old_sw_p,val_led_mode_sel,val_led_p_en_ctrl;
static void swled_init()
{
	unsigned int value;
	
	max_portled = gSwitchModel->led.num;
	val_led_glb_ctrl = value = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_LED_GLB_CTRL_ADDR);

	val_led_mode_sel = value = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_LED_MODE_SEL_ADDR);
	
	val_led_p_en_ctrl = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_LED_P_EN_CTRL_ADDR);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_P_EN_CTRL_ADDR, 0xFFFFFFF);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_SW_CTRL_ADDR, 0xFFFFFFF);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED0_SW_P_EN_CTRL_ADDR, 0xFFFFFFF);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED1_SW_P_EN_CTRL_ADDR, 0xFFFFFFF);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED2_SW_P_EN_CTRL_ADDR, 0);
	return;
	
}

static void swled_uninit()
{
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_P_EN_CTRL_ADDR, val_led_p_en_ctrl);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_SW_CTRL_ADDR, 0);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED0_SW_P_EN_CTRL_ADDR, 0);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED1_SW_P_EN_CTRL_ADDR, 0);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED2_SW_P_EN_CTRL_ADDR, 0);

	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_GLB_CTRL_ADDR, val_led_glb_ctrl);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_MODE_SEL_ADDR, val_led_mode_sel);
	#ifdef CONFIG_CUSTOMER_BOARD
	customer_led_config_init(gSwitchModel);
	#endif
	return;
}

static unsigned int portindex_ledon(int port,int index)
{
 	return LEDON(index);
}

static void swled_out(unsigned int *bit)
{
	int port;
	
	for (port=0;port<MAX_PORT;port++)
	{
		MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_SW_P_CTRL_ADDR(port),bit[port]);
		__asm__ volatile ("sync");
	}
	udelay(300000);
	return;
}

void swportled_set(int port,int index,unsigned int val)
{
	unsigned int value;
	
	if (gSwitchModel == 0)
	{
		printf("\n led test: Cant get board model\n");
		return;
	}
	value = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_LED_P_EN_CTRL_ADDR);
	value |= RTL8380_LED_P_EN_CTRL_LED_P_EN_27_0_MASK(port);
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_P_EN_CTRL_ADDR,value);
	
	if (index == 0)
	{
		value = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_LED0_SW_P_EN_CTRL_ADDR);
		value |= RTL8380_LED0_SW_P_EN_CTRL_SW_CTRL_LED0_EN_27_0_MASK(port);
		MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED0_SW_P_EN_CTRL_ADDR,value);
	}
	else if (index == 1)
	{
		value = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_LED1_SW_P_EN_CTRL_ADDR);
		value |= RTL8380_LED1_SW_P_EN_CTRL_SW_CTRL_LED1_EN_27_0_MASK(port);
		MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED1_SW_P_EN_CTRL_ADDR,value);
	}
	else if (index == 2)
	{
		value = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_LED2_SW_P_EN_CTRL_ADDR);
		value |= RTL8380_LED2_SW_P_EN_CTRL_SW_CTRL_LED2_EN_27_0_MASK(port);
		MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED2_SW_P_EN_CTRL_ADDR, value);
	}
	value = MEM32_READ(SWCORE_BASE_ADDR | RTL8380_LED_SW_P_CTRL_ADDR(port));
	value &= ~LED_MASK(index);
	value |= (val << (3*index));
	MEM32_WRITE(SWCORE_BASE_ADDR| RTL8380_LED_SW_P_CTRL_ADDR(port),value);
	__asm__ volatile ("sync");
	return;
}
#endif

static unsigned int portledallon(int port)
{
	int i;
	unsigned int value;
	
	value = 0;
	for (i=0;i<max_portled;i++)
	{
		value |= portindex_ledon(port,i);
	}
	return value;
}

static unsigned int portledallon_index(int port, int index)
{
	unsigned int value;
	
	value = 0;
	if (index < max_portled)
	{
		value |= portindex_ledon(port,index);
	}
	return value;
}

void swledtest_on(int port,int index)
{
	int portnum;
	unsigned int bit[MAX_PORT];
	
	if (gSwitchModel == 0)
	{
		printf("\n led test: Cant get board model\n");
		return;
	}
	swled_init();

	printf("\n port %d led %d on \n",port,index);
	for (portnum=0;portnum<MAX_PORT;portnum++)
	{
		bit[portnum]=0;
	}
	bit[port]=portindex_ledon(port,index);
	swled_out(bit);
	
	printf("\n press any to end.\n");
	getc();
	
	bit[port]=0;
	swled_out(bit);
	
	swled_uninit();

	return;
}
void run_ledtest(void)
{
	int portnum,index;
	unsigned int bit[MAX_PORT];
	unsigned int port_index;
	
	if (gSwitchModel == 0)
	{
		printf("\n led test: Cant get board model\n");
		return;
	}
	swled_init();

	printf("\n all led on \n");	
	for (portnum=0;portnum<MAX_PORT;portnum++)
	{
		bit[portnum]=portledallon(portnum);
	}
	swled_out(bit);

	udelay(200000);
	printf("\n all led off \n");
	for (portnum=0;portnum<MAX_PORT;portnum++)
	{
		bit[portnum]=0;
	}
	swled_out(bit);
	
	printf("\n walking one \n");

	for (port_index= 1;port_index <= MAX_PORT;port_index++)
	{
		for (index=0;index<max_portled;index++)
		{
			for (portnum=0;portnum<MAX_PORT;portnum++)
			{
				bit[portnum]=0;
			}
			bit[port_index-1]=portindex_ledon(port_index-1,index);
			swled_out(bit);
		}
	}

	printf("\n all led off \n");
	for (portnum=0;portnum<MAX_PORT;portnum++)
	{
		bit[portnum]=0;
	}
	swled_out(bit);
	
	swled_uninit();
	return;
}

void ledtest_groupmode(int req_round)
{
	int portnum, round_time, led_index;
	unsigned int bit[MAX_PORT];
	
	if (gSwitchModel == 0)
	{
		printf("\n ledtest_groupmode: Cant get board model\n");
		return;
	}
	swled_init();
	
	for(round_time = 0; round_time < req_round; round_time++)
	{
		printf("\n ledtest_groupmode: ROUND(%d)\n",round_time);
		for(led_index = 0; led_index < max_portled; led_index++)
		{
			rtk_sys_led_on();
			printf("LED index(%d) all on \n", led_index);	
			for (portnum=0;portnum < gSwitchModel->port.count; portnum++)
			{
				bit[portnum]=portledallon_index(portnum, led_index);
			}
			swled_out(bit);

			OSAL_MDELAY(500);
			printf("LED all off \n");
			rtk_sys_led_off();
			for (portnum=0;portnum < gSwitchModel->port.count; portnum++)
			{
				bit[portnum]=0;
			}
			swled_out(bit);
			
			OSAL_MDELAY(500);

		    if (is_ctrlc())
    		{
        		printf("\n");
	        	goto led_test_end;
	    	}
	    }
	}
	
led_test_end:	
	swled_uninit();
	return;
}

