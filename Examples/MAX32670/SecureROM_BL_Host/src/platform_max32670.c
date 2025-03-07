/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/*******************************      INCLUDES    ****************************/
#include <stdio.h>
#include <stdint.h>

#include "terminal.h"
#include "platform.h"

#include "uart.h"
#include "gpio.h"
#include "mxc_delay.h"

/*******************************      DEFINES     ****************************/
#define TARGET_COM_PORT MXC_UART1

/******************************* Type Definitions ****************************/

/*******************************    Variables   ****************************/

/******************************* Static Functions ****************************/

/******************************* Public Functions ****************************/
/*
 * 	UART
 */
int plt_uart_init(void)
{
    int ret = 0;

    ret = MXC_UART_Init(TARGET_COM_PORT, 115200, MXC_UART_APB_CLK);

    return ret;
}

int plt_uart_write(const unsigned char *src, unsigned int len, unsigned int to)
{
    (void)to;

    int ret = 0;
    mxc_uart_req_t req;

    req.uart = TARGET_COM_PORT;
    req.txData = (unsigned char *)src;
    req.txLen = len;
    req.rxData = NULL;
    req.rxLen = 0;
    req.txCnt = 0;
    req.rxCnt = 0;
    req.callback = NULL;

    /*
	 * MXC_UART_Write(..) function does not check fifo status
	 * so that MXC_UART_Transaction is used instead of MXC_UART_Write
	 */
    ret = MXC_UART_Transaction(&req);

    return ret;
}

int plt_uart_read(unsigned char *dst, unsigned int len, unsigned int to)
{
    int ret = 0;
    int val;

    to *= 1000; //
    while (len) {
        val = MXC_UART_ReadCharacterRaw(TARGET_COM_PORT);
        if (val >= 0) {
            *dst++ = (unsigned char)val;
            --len;
        } else {
            if (to == 0) {
                ret = -1; // Indicates FAIL
                break;
            }
            --to;
        }
    }

    return ret;
}

/*
 *  GPIO
 */
static const mxc_gpio_cfg_t bl_pins[] = {
    { MXC_GPIO0, MXC_GPIO_PIN_30, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH },
    { MXC_GPIO0, MXC_GPIO_PIN_27, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH },
};

int plt_gpio_init(void)
{
    int i;
    for (i = 0; i < sizeof(bl_pins) / sizeof(bl_pins[0]); i++) {
        MXC_GPIO_Config(&bl_pins[i]);
    }

    plt_gpio_set(0, 1); // set RST pin state to 1
    return 0;
}

void plt_gpio_set(unsigned int idx, int state)
{
    if (state) {
        MXC_GPIO_OutSet(bl_pins[idx].port, bl_pins[idx].mask);
    } else {
        MXC_GPIO_OutClr(bl_pins[idx].port, bl_pins[idx].mask);
    }
}

int plt_gpio_get(unsigned int idx)
{
    return !MXC_GPIO_InGet(bl_pins[idx].port, bl_pins[idx].mask);
}

/*
 *	Delay
 */
void plt_delay_ms(unsigned int ms)
{
    MXC_Delay(ms * 1000UL);
}
