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
#include <customer/customer_diag.h>
#include <rtk/drv/rtl8231/rtl8231_drv.h>

#if defined(CONFIG_RTL8380)
#include <rtk/mac/rtl8380/rtl8380_rtk.h>
#endif

#if defined(CONFIG_RTL8390)
#include <rtk/mac/rtl8390/rtl8390_drv.h>
#include <rtk/mac/rtl8390/rtl8390_mdc_mdio.h>
#include <rtk/mac/rtl8390/rtl8390_rtk.h>
#endif

/*
 * Symbol Definition
 */
#define PORT_LOOPBACK_TEST_PKT_MAX_LEN (1500)

/*
 * Data Declaration
 */
uint8 pkt[] = { /* ARP: Who has 192.168.154.80? Tell 192.168.154.89 */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x52, 0x45, 0x41, 0x4c, 0x30, 0x59, 0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x52, 0x45, 0x41, 0x4c, 0x30, 0x59, 0xc0, 0xa8, 0x9a, 0x59,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x9a, 0x50, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20
    };

uint8 pkt_recv[PORT_LOOPBACK_TEST_PKT_MAX_LEN];

/*
 * Macro Definition
 */


/*
 * Function Declaration
 */
int32 is_ctrlc(void)
{
	if (tstc()) {
		switch (getc ()) {
		case 0x03:		/* ^C - Control C */
			return 1;
		default:
			break;
		}
	}
	return 0;
}

#if 0
int32 rtk_rstDeft_detect(void)
{
    uint32 delay1,delay2;
    int32   ret = 1;

    for(delay1=0;delay1<0xFFFF;delay1++)
    {
        for(delay2=0;delay2<0x5;delay2++)
        {
    		if (is_ctrlc())
            {
                printf("\n");
    			goto END;
    		}

            ret = rtk_rstDeftGpio_detect();
            if (1 == ret)
            {
                printf("Reset to factory default ....\n");
                ret = 0;
                goto END;
            }
        }
    }

END:
    return ret;
}

int32 rtk_dram_test(uint32 round, vu_long addr_start, vu_long addr_end)
{
    uint32 i;
	vu_long	*addr, *start, *end;

	vu_long	addr_len;
	vu_long	offset;
	vu_long	pattern;
	vu_long	temp;
	vu_long	anti_pattern;

    start = (ulong *)addr_start;
    end = (ulong *)addr_end;
    //printf ("Testing %08x ... %08x:\n", (uint)start, (uint)end);

    for(i=0; i<round; i++)
    {
        printf("\r[round:%d]",i);

    /*ctrl+c to break*/
       if (is_ctrlc())
       {
           printf("\n");
           return 1;
       }

        addr = start;

        addr_len = (ulong)end - (ulong)start;
        pattern = (vu_long) 0xaaaaaaaa;
        anti_pattern = (vu_long) 0x55555555;

      /*
         * Write the default pattern at each of the
         * power-of-two offsets.
         */

        for (offset = 0; offset < addr_len; offset += 4)
        {
            start[offset] = pattern;
        }

      /*
         * Check for address bits stuck high.
         */
        for (offset = 0; offset < addr_len; offset += 4)
        {
            temp = start[offset];
            if (temp != pattern)
            {
                printf ("\nFAILURE: Address bit stuck high @ 0x%.8lx:"
                    " expected 0x%.8lx, actual 0x%.8lx\n",
                    (ulong)&start[offset], pattern, temp);
                return 1;
            }
        }

        for (offset = 0; offset < addr_len; offset += 4)
        {
            start[offset] = anti_pattern;
        }

      /*
         * Check for address bits stuck high.
         */
        for (offset = 0; offset < addr_len; offset += 4)
        {
            temp = start[offset];
            if (temp != anti_pattern)
            {
                printf ("\nFAILURE: Address bit stuck high @ 0x%.8lx:"
                    " expected 0x%.8lx, actual 0x%.8lx\n",
                    (ulong)&start[offset], anti_pattern, temp);
                return 1;
            }
        }

        for (offset = 0; offset < addr_len; offset += 4)
        {
            start[offset] = offset;
        }

      /*
         * Check for address bits stuck high.
         */
        for (offset = 0; offset < addr_len; offset += 4)
        {
            temp = start[offset];
            if (temp != offset)
            {
                printf ("\nFAILURE: Address bit stuck high @ 0x%.8lx:"
                    " expected 0x%.8lx, actual 0x%.8lx\n",
                    (ulong)&start[offset], offset, temp);
                return 1;
            }
        }
    }

    return 0;
}







#define RTL8231_GPIO_ACCESS_DELAY (0xFFF)
#define RTL8231_GPIO_READ_TIMES (10)
#define RTL8231_GPIO_VALUE_THRD (3)

#define RTK_FC_DRAM_TEST_ROUND  (1000)
#define RTK_FC_DRAM_TEST_ADDR_START (0x81000000)
#define RTK_FC_DRAM_TEST_ADDR_END (0x81020000)  //128KB

#define RTK_FC_FLASH_TEST_ROUND  (200)  /*test about 1 hour*/
//#define	FC_FLASH_TEST_SIZE_PER_PATTREN	(0x20000) //128KB
#define	FC_FLASH_TEST_SIZE_PER_PATTREN	(0x10000) //64KB
#define RTK_FC_FLASH_TEST_ADDR_START (0xB4250000)
//#define RTK_FC_FLASH_TEST_ADDR_END (FLASH_TOP_ADDRESS)
#define RTK_FC_FLASH_TEST_ADDR_END (RTK_FC_FLASH_TEST_ADDR_START+(FC_FLASH_TEST_SIZE_PER_PATTREN)*1)

/*
 * Protection Flags for flash_protect():
 */
#define FLAG_PROTECT_SET	0x01
#define FLAG_PROTECT_CLEAR	0x02
#define	FLAG_PROTECT_INVALID	0x03


int32 rtk_flash_test(uint32 round, vu_long addr_start, vu_long addr_end)
{
    uint32 i;
    int32 ret;

    for(i=0;i<round;i++)
    {
        printf("\r[round:%d]",i);

     /*ctrl+c to break*/
        if (is_ctrlc())
        {
            printf("\n");
            return 1;
        }

    	/* protect off flash data. */
    	flash_protect( FLAG_PROTECT_CLEAR, addr_start,addr_end, addr2info(addr_start));
    	/* 1. Non Boot loader area, in case of unrecoverable operation. */
#if 0
    /*need not to excute fully flash test*/
    	ret = _flash_test(addr_start, FC_FLASH_TEST_SIZE_PER_PATTREN, (addr_end - addr_start));
        ret = flash_normal_patterns(addr_start, FC_FLASH_TEST_SIZE_PER_PATTREN, (addr_end - addr_start));
#else
        ret = flash_walking_of_1(addr_start, FC_FLASH_TEST_SIZE_PER_PATTREN, (addr_end - addr_start));
#endif

        if(ret != 0)
            return ret;
    }

    return 0;
}

int32 rtk_port_loopback_test(void)
{
    int ret = 0;
    uint32 i;
    uint32 val;
    uint32 count;
    uint32 port;
    uint32 port_start,port_end;
    uint32 rx_pkt_len=0;
    uint8 *recv_pkt = NULL;


    if (gSwitchModel == NULL)
        return 1;

    NetInit();

    port_start = gSwitchModel->port.list[0].mac_id;
    port_end = gSwitchModel->port.list[0].mac_id + gSwitchModel->port.count;
    for(port=port_start;port<port_end;port++)
    {
        printf("\r[port:%d] ",port);

        rtk_phy_selfLoop_on(port);

        for(i=0;i<0xF;i++)
        {
            udelay(3000000);

            val = MEM32_READ(0xbb00a188);

            if((val>>port)&&0x1 == 0x1)
            {
                break;
            }
        }

        if(i == 0x9)
        {
            printf("could not linkup!\n");
            return 1;
        }

        NetSendPacket(pkt,sizeof(pkt));

        for(count=0;count<0xF;count++)
        {
        /*ctrl+c to break*/
           if (is_ctrlc())
           {
               printf("\n");
               return 1;
           }

            udelay(3000000);

            rx_pkt_len = 0;
            rx_pkt_len = NetRecvPacket(&recv_pkt,PORT_LOOPBACK_TEST_PKT_MAX_LEN);

            if((rx_pkt_len != 0)&&(recv_pkt != NULL))
            {
                ret = OSAL_MEMCMP(pkt,recv_pkt,(rx_pkt_len-4));

                if(ret == 0)
                    break;
                else
                {
                    printf("loop back test failed!\n");
                    return 1;
                }
            }
        }

        if(rx_pkt_len == 0)
        {
            printf("\nloop back test failed!\n");
            return 1;
        }

        rtk_phy_selfLoop_off(port);
    }

    return 0;
}


void run_fc_test(void)
{
    uint32 htp_gpio_val_sum=0;
    int32 ret_val=0,ret_val_temp=0;

    rtk_rstDeftGpio_init();

    if (0 == rtk_htp_detect())
        return;

    ret_val = 0;
    if(htp_gpio_val_sum < RTL8231_GPIO_VALUE_THRD)
    {
        rtk_network_on();
        /*HTP switch status is '1', excute HTP function*/
        printf("============= Factory Test Begin.... =============\n");

        /**************************************************/
        /****************** RST_TO_DEFAULT ***************/
        /**************************************************/
        printf("--- Wait 10s to detect RST....[ctrl+c to skip] ---\n");
        ret_val |= rtk_rstDeft_detect();

        while (1)
        {
            /**************************************************/
            /***************** All port loopback test **************/
            /**************************************************/
            printf("------- All port loopback test.... -------\n");
            ret_val_temp = rtk_port_loopback_test();
            ret_val |= ret_val_temp;
            if (0 != ret_val_temp)
                break;
            printf(" -------------[%s]\n",(0 == ret_val_temp)?"Pass":"Failed");

            /**************************************************/
            /****************** System DRAM test ****************/
            /**************************************************/
            printf("------- System DRAM test.... -------\n");
            ret_val_temp = rtk_dram_test(RTK_FC_DRAM_TEST_ROUND,RTK_FC_DRAM_TEST_ADDR_START,RTK_FC_DRAM_TEST_ADDR_END);
            ret_val |= ret_val_temp;
            if (0 != ret_val_temp)
                break;
            printf(" -------------[%s]\n",(0 == ret_val_temp)?"Pass":"Failed");

            /**************************************************/
            /****************** System Flash test ****************/
            /**************************************************/
            printf("------- System Flash test.... -------\n");
            ret_val_temp = rtk_flash_test(RTK_FC_FLASH_TEST_ROUND,RTK_FC_FLASH_TEST_ADDR_START,RTK_FC_FLASH_TEST_ADDR_END);
            ret_val |= ret_val_temp;
            if (0 != ret_val_temp)
                break;
            printf(" -------------[%s]\n",(0 == ret_val_temp)?"Pass":"Failed");
        }

        if(ret_val)
        {/*if factory test failed, power off sys led to indicate it*/
            rtk_sys_led_off();
        }

        printf("========= Factory Test End [Status: %s ]=========\n\n",(0==ret_val)?"PASS":"FAILED");
        rtk_network_off();
    }
}

void rtl8231_pin_status_get(unsigned int pinNum, unsigned int *pinStatus)
{
    rtl8231_pin_direction_set(pinNum,rtl8231_PIN_DIR_IN);
    rtl8231_pin_data_get(pinNum,pinStatus);
}

void rtl8231_pin_status_set(unsigned int pinNum, unsigned int pinStatus)
{
    rtl8231_pin_direction_set(pinNum,rtl8231_PIN_DIR_OUT);
    rtl8231_pin_data_set(pinNum,pinStatus);
}
#endif
