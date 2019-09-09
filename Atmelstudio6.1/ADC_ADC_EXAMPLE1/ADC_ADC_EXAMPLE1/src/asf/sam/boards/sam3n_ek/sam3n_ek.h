/**
 * \file
 *
 * \brief SAM3N-EK Board Definition.
 *
 * Copyright (c) 2011 - 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef _SAM3N_EK_H_
#define _SAM3N_EK_H_

#include "compiler.h"
#include "system_sam3n.h"
#include "exceptions.h"

/*----------------------------------------------------------------------------*/
/**
 *  \page sam3n_ek_opfreq "SAM3N-EK - Operating frequencies"
 *  This page lists several definition related to the board operating frequency
 *
 *  \section Definitions
 *  - \ref BOARD_FREQ_*
 *  - \ref BOARD_MCK
 */

/** Board oscillator settings */
#define BOARD_FREQ_SLCK_XTAL		(32768U)
#define BOARD_FREQ_SLCK_BYPASS		(32768U)
#define BOARD_FREQ_MAINCK_XTAL		(12000000U)
#define BOARD_FREQ_MAINCK_BYPASS	(12000000U)

/** Master clock frequency */
#define BOARD_MCK					CHIP_FREQ_CPU_MAX

/*----------------------------------------------------------------------------*/
/**
 * \page sam3n_ek_board_info "SAM3N-EK - Board informations"
 * This page lists several definition related to the board description.
 *
 * \section Definitions
 * - \ref BOARD_NAME
 */

/** Name of the board */
#define BOARD_NAME "SAM3N-EK"
/** Board definition */
#define sam3nek
/** Family definition (already defined) */
#define sam3n
/** Core definition */
#define cortexm3

/*----------------------------------------------------------------------------*/
/**
 * \page sam3n_ek_piodef "SAM3N-EK - PIO definitions"
 * This pages lists all the pio definitions. The constants
 * are named using the following convention: PIN_* for a constant which defines
 * a single Pin instance (but may include several PIOs sharing the same
 * controller), and PINS_* for a list of Pin instances.
 *
 * Uart0
 * - \ref PINS_UART0
 *
 * Leds
 * - \ref PIN_POWER_LED
 * - \ref PIN_LED_BLUE
 * - \ref PIN_LED_AMBER
 * - \ref PIN_LED_GREEN
 * - \ref PINS_LEDS
 *
 * Spi
 * - \ref PIN_SPI_MISO
 * - \ref PIN_SPI_MOSI
 * - \ref PIN_SPI_SPCK
 * - \ref PINS_SPI
 * - \ref PIN_SPI_NPCS0_SDCARD
 * - \ref PIN_SPI_NPCS1_ZIGBEE
 * - \ref PIN_SPI_NPCS2_LCD
 * - \ref PIN_SPI_NPCS3_AT25
 *
 * Push buttons
 * - \ref PIN_PB_USER1
 * - \ref PIN_PB_USER2
 * - \ref PINS_PB_USER
 *
 * Lcd
 * - \ref PIN_LCD_RS
 * - \ref PIN_LCD_RSTN
 * - \ref PIN_LCD_BACKLIGTH
 * - \ref PINS_LCD
 *
 * Serial flash
 * - \ref PINS_AT25
 *
 * Sdcard
 * - \ref PINS_SDCARD
 *
 * Zigbee
 * - \ref PINS_ZIGBEE
 *
 * PWM for Buzzer
 * - \ref PIN_PWM_BUZZER
 * - \ref CHANNEL_PWM_BUZZER
 *
 * PWM for LEDs
 * - \ref PIN_PWM_LED0
 * - \ref PIN_PWM_LED1
 * - \ref PIN_PWM_LED2
 * - \ref CHANNEL_PWM_LED0
 * - \ref CHANNEL_PWM_LED1
 * - \ref CHANNEL_PWM_LED2
 *
 * QTouch Slider
 * - \ref SLIDER_IOMASK_SNS
 * - \ref SLIDER_IOMASK_SNSK
 * - \ref PINS_SLIDER_SNS
 * - \ref PINS_SLIDER_SNSK
 *
 * QTouch Keys
 * - \ref KEY_IOMASK_SNS
 * - \ref KEY_IOMASK_SNSK
 * - \ref PINS_KEY_SNS
 * - \ref PINS_KEY_SNSK
 *
 * TWI
 * - \ref PINS_TWI0
 * - \ref PINS_TWI1
 *
 * PCK
 * - \ref PIN_PCK0
 * - \ref PIN_PCK1
 *
 * USART
 * - \ref PIN_USART0_RXD
 * - \ref PIN_USART0_TXD
 * - \ref PIN_USART0_RTS
 * - \ref PIN_USART0_CTS
 * - \ref PIN_USART0_SCK
 * - \ref PIN_USART1_RXD
 * - \ref PIN_USART1_TXD
 * - \ref PIN_USART1_RTS
 * - \ref PIN_USART1_CTS
 * - \ref PIN_USART1_SCK
 *
 * ISO7816
 * - \ref PIN_ISO7816_RSTMC
 * - \ref PINS_ISO7816
 * - \ref BOARD_ISO7816_BASE_USART
 * - \ref BOARD_ISO7816_ID_USART
 */

/* ===============  LED =============== */
/** POWER_LED pin definition (RED). */
#define PIN_POWER_LED   {PIO_PA0, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/** LED BLUE pin definition. */
#define PIN_LED_BLUE    {PIO_PA23, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/** LED AMBER pin definition. */
#define PIN_LED_AMBER   {PIO_PA25, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/** LED GREEN pin definition. */
#define PIN_LED_GREEN   {PIO_PB14, PIOB, ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT}

/** List of all LEDs definitions. */
#define PINS_LEDS   PIN_LED_BLUE, PIN_LED_GREEN, PIN_LED_AMBER, PIN_POWER_LED

#define LED0_GPIO 		(PIO_PA23_IDX)
#define LED0_FLAGS (PIO_OUTPUT_1 | PIO_DEFAULT)

#define PIN_LED_0_MASK PIO_PA23
#define PIN_LED_0_PIO PIOA
#define PIN_LED_0_ID ID_PIOA
#define PIN_LED_0_TYPE PIO_OUTPUT_1
#define PIN_LED_0_ATTR PIO_DEFAULT

#define LED1_GPIO 		(PIO_PB14_IDX)
#define LED1_FLAGS (PIO_OUTPUT_1 | PIO_DEFAULT)

#define PIN_LED_1_MASK PIO_PB14
#define PIN_LED_1_PIO PIOB
#define PIN_LED_1_ID ID_PIOB
#define PIN_LED_1_TYPE PIO_OUTPUT_1
#define PIN_LED_1_ATTR PIO_DEFAULT

#define PINS_ADC_TRIG  PIO_PA8_IDX
#define PINS_ADC_TRIG_FLAG  (PIO_PERIPH_B | PIO_DEFAULT)

/* =============== UART0 =============== */
/** UART0 pin RX */
#define PIN_RX_UART0    {PIO_PA9A_URXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** UART0 pin TX */
#define PIN_TX_UART0    {PIO_PA10A_UTXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** UART0 pin definitions */
#define PINS_UART0 PIN_RX_UART0, PIN_TX_UART0

#define PINS_UART		(PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART_FLAGS	(PIO_PERIPH_A | PIO_DEFAULT)

#define PINS_UART_MASK     (PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART_PIO      PIOA
#define PINS_UART_ID       ID_PIOA
#define PINS_UART_TYPE     PIO_PERIPH_A
#define PINS_UART_ATTR     PIO_DEFAULT

/* =============== SPI =============== */
/** SPI MISO pin definition. */
#define PIN_SPI_MISO    {PIO_PA12A_MISO, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI MOSI pin definition. */
#define PIN_SPI_MOSI    {PIO_PA13A_MOSI, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI SPCK pin definition. */
#define PIN_SPI_SPCK    {PIO_PA14A_SPCK, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI chip select pin definition for Micro SD connector. */
#define PIN_SPI_NPCS0_SDCARD  {PIO_PA11A_NPCS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI chip select pin definition for Zigbee extension board. */
#define PIN_SPI_NPCS1_ZIGBEE  {PIO_PA31A_NPCS1, PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/** SPI chip select pin definition for LCD. */
#define PIN_SPI_NPCS2_LCD     {PIO_PA30B_NPCS2, PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/** SPI chip select pin definition for Serial Flash. */
#define PIN_SPI_NPCS3_AT25    {PIO_PA22B_NPCS3, PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/** List of SPI pin definitions (MISO, MOSI & SPCK). */
#define PINS_SPI        PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SPCK

/** SPI MISO pin definition. */
#define SPI_MISO_GPIO 		      (PIO_PA12_IDX)
#define SPI_MISO_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO 		      (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO 		      (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS       (PIO_PERIPH_A | PIO_DEFAULT)

/** SPI chip select 0 pin definition. (Only one configuration is possible) */
#define SPI_NPCS0_GPIO            (PIO_PA11_IDX)
#define SPI_NPCS0_FLAGS           (PIO_PERIPH_A | PIO_DEFAULT)
/** SPI chip select 1 pin definition. (multiple configurations are possible) */
#define SPI_NPCS1_PA9_GPIO 		  (PIO_PA9_IDX)
#define SPI_NPCS1_PA9_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS1_PA31_GPIO 	  (PIO_PA31_IDX)
#define SPI_NPCS1_PA31_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS1_PB14_GPIO 	  (PIO_PB14_IDX)
#define SPI_NPCS1_PB14_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS1_PC4_GPIO 		  (PIO_PC4_IDX)
#define SPI_NPCS1_PC4_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
/** SPI chip select 2 pin definition. (multiple configurations are possible) */
#define SPI_NPCS2_PA10_GPIO 	  (PIO_PA10_IDX)
#define SPI_NPCS2_PA10_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS2_PA30_GPIO 	  (PIO_PA30_IDX)
#define SPI_NPCS2_PA30_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS2_PB2_GPIO 		  (PIO_PB2_IDX)
#define SPI_NPCS2_PB2_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS2_PC7_GPIO 		  (PIO_PC7_IDX)
#define SPI_NPCS2_PC7_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
/** SPI chip select 3 pin definition. (multiple configurations are possible) */
#define SPI_NPCS3_PA3_GPIO 		  (PIO_PA3_IDX)
#define SPI_NPCS3_PA3_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS3_PA5_GPIO 		  (PIO_PA5_IDX)
#define SPI_NPCS3_PA5_FLAGS       (PIO_PERIPH_B | PIO_DEFAULT)
#define SPI_NPCS3_PA22_GPIO 	  (PIO_PA22_IDX)
#define SPI_NPCS3_PA22_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)

/* =============== Push Buttons =============== */
/** Push button #0 definition. Attributes = pull-up + debounce + interrupt on rising edge. */
#define PIN_PB_USER1    {PIO_PA15, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE}
/** Push button #1 definition. Attributes = pull-up + debounce + interrupt on falling edge. */
#define PIN_PB_USER2    {PIO_PA16, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE}
/** List of all push button definitions. */
#define PINS_PB_USER    PIN_PB_USER1, PIN_PB_USER2

#define GPIO_PUSH_BUTTON_1			(PIO_PA15_IDX)
#define GPIO_PUSH_BUTTON_1_FLAGS	(PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)

#define PIN_PUSHBUTTON_1_MASK PIO_PA15
#define PIN_PUSHBUTTON_1_PIO PIOA
#define PIN_PUSHBUTTON_1_ID ID_PIOA
#define PIN_PUSHBUTTON_1_TYPE PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

#define GPIO_PUSH_BUTTON_2			(PIO_PA16_IDX)
#define GPIO_PUSH_BUTTON_2_FLAGS	(PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)

#define PIN_PUSHBUTTON_2_MASK PIO_PA16
#define PIN_PUSHBUTTON_2_PIO PIOA
#define PIN_PUSHBUTTON_2_ID ID_PIOA
#define PIN_PUSHBUTTON_2_TYPE PIO_INPUT
#define PIN_PUSHBUTTON_2_ATTR PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE

/* =============== LCD =============== */
/** LCD Register Select pin definition. */
#define PIN_LCD_RS_GPIO   PIO_PA28_IDX
#define PIN_LCD_RS_FLAGS    PIO_OUTPUT_1 | PIO_DEFAULT
#define PIN_LCD_RS  {PIO_PA28, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LCD_RS_PIO     PIOA
#define PIN_LCD_RS_MASK  PIO_PA28
#define PIN_LCD_RS_ID      ID_PIOA
#define PIN_LCD_RS_TYPE   PIO_OUTPUT_1
#define PIN_LCD_RS_ATTR   PIO_DEFAULT
/** LCD Reset pin definition: active low */
#define PIN_LCD_RSTN_GPIO   PIO_PA29_IDX
#define PIN_LCD_RSTN_FLAGS    PIO_OUTPUT_1 | PIO_DEFAULT
#define PIN_LCD_RSTN {PIO_PA29, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define PIN_LCD_RSTN_PIO     PIOA
#define PIN_LCD_RSTN_MASK  PIO_PA29
#define PIN_LCD_RSTN_ID      ID_PIOA
#define PIN_LCD_RSTN_TYPE   PIO_OUTPUT_1
#define PIN_LCD_RSTN_ATTR   PIO_DEFAULT
/** LCD backlight definition */
#define PIN_LCD_BACKLIGHT {PIO_PC13, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
#define BOARD_BACKLIGHT          PIO_PC13_IDX
#define BOARD_BACKLIGHT_FLAG       PIO_OUTPUT_1 | PIO_DEFAULT
#define PIN_LCD_BACKLIGHT_MASK PIO_PC13
#define PIN_LCD_BACKLIGHT_PIO PIOC
#define PIN_LCD_BACKLIGHT_ID ID_PIOC
#define PIN_LCD_BACKLIGHT_TYPE PIO_OUTPUT_1
#define PIN_LCD_BACKLIGHT_ATTR PIO_DEFAULT
/** List of all LCD pin definitions. */
#define PINS_LCD    PIN_SPI_SPCK, PIN_SPI_MOSI, PIN_SPI_NPCS2_LCD, PIN_LCD_RS, PIN_LCD_RSTN

/* =============== DATAFLASH =============== */
/** List of all Serial flash pin definitions. */
#define PINS_AT25         PINS_SPI, PIN_SPI_NPCS3_AT25

/* =============== SDCARD =============== */
/** List of all SDCARD pin definitions. */
#define PINS_SDCARD       PINS_SPI, PIN_SPI_NPCS0_SDCARD

/* =============== ZIGBEE =============== */
/** Zigbee Reset pin definition: active low */
#define PIN_ZIGBEE_RSTN {PIO_PC23, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/** Zigbee Irq1 definition */
#define PIN_ZIGBEE_IRQ1 {PIO_PC25, PIOC, ID_PIOC, PIO_INPUT, PIO_PULLUP | PIO_IT_RISE_EDGE}
/** List of all SDCARD pin definitions. */
#define PINS_ZIGBEE       PINS_SPI, PIN_SPI_NPCS1_ZIGBEE, PIN_ZIGBEE_RSTN, PIN_ZIGBEE_IRQ1

#define PIN_TC0_TIOA0		(PIO_PA0_IDX)
#define PIN_TC0_TIOA0_FLAGS	(PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_TC0_TIOA1		(PIO_PA15_IDX)
#define PIN_TC0_TIOA1_FLAGS	(PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_TC0_TIOA1_PIO     PIOA
#define PIN_TC0_TIOA1_MASK  PIO_PA15
#define PIN_TC0_TIOA1_ID      ID_PIOA
#define PIN_TC0_TIOA1_TYPE   PIO_PERIPH_B
#define PIN_TC0_TIOA1_ATTR   PIO_DEFAULT

#define PIN_TC0_TIOA2		(PIO_PA26_IDX)
#define PIN_TC0_TIOA2_FLAGS	(PIO_INPUT | PIO_DEFAULT)

#define PIN_TC0_TIOA2_PIO     PIOA
#define PIN_TC0_TIOA2_MASK  PIO_PA26
#define PIN_TC0_TIOA2_ID      ID_PIOA
#define PIN_TC0_TIOA2_TYPE   PIO_INPUT
#define PIN_TC0_TIOA2_ATTR   PIO_DEFAULT

/* =============== PWM for BUZZER =============== */
/** PWM pins definition for buzzer */
#define PIN_PWM_BUZZER {PIO_PA1A_PWM1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** PWM channel for BUZZER */
#define CHANNEL_PWM_BUZZER 1

/* =============== PWM for LEDs =============== */
/** PWM pins definition for LED0 */
#define PIN_PWM_LED0 {PIO_PA23B_PWM0, PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/** PWM pins definition for LED1 */
#define PIN_PWM_LED1 {PIO_PB14B_PWM3, PIOB, ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT}
/** PWM pins definition for LED2 */
#define PIN_PWM_LED2 {PIO_PA25B_PWM2, PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/** PWM channel for LED0 */
#define CHANNEL_PWM_LED0 0
/** PWM channel for LED1 */
#define CHANNEL_PWM_LED1 3
/** PWM channel for LED2 */
#define CHANNEL_PWM_LED2 2

/** PWM LED0 pin definitions. */
#define PIN_PWM_LED0_GPIO   PIO_PA23_IDX
#define PIN_PWM_LED0_FLAGS  (PIO_PERIPH_B | PIO_DEFAULT)

/** PWM LED1 pin definitions. */
#define PIN_PWM_LED1_GPIO   PIO_PB14_IDX
#define PIN_PWM_LED1_FLAGS  (PIO_PERIPH_B | PIO_DEFAULT)

/* =============== QTOUCH =============== */
/** List of all Slider pin definitions. */
#define SLIDER_IOMASK_SNS   (PIO_PC0 | PIO_PC2 | PIO_PC4)
#define SLIDER_IOMASK_SNSK  (PIO_PC1 | PIO_PC3 | PIO_PC5)
#define PINS_SLIDER_SNS     {SLIDER_IOMASK_SNS,  PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}
#define PINS_SLIDER_SNSK    {SLIDER_IOMASK_SNSK, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}

/** List of all Keys pin definitions. */
#define KEY_IOMASK_SNS   (PIO_PC8 | PIO_PC10 )
#define KEY_IOMASK_SNSK  (PIO_PC9 | PIO_PC11 )
#define PINS_KEY_SNS     {KEY_IOMASK_SNS,  PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}
#define PINS_KEY_SNSK    {KEY_IOMASK_SNSK, PIOC, ID_PIOC, PIO_INPUT, PIO_DEFAULT}

/** PIOS for QTouch */
#define PINS_QTOUCH     PINS_SLIDER_SNS, PINS_SLIDER_SNSK, PINS_KEY_SNS, PINS_KEY_SNSK

/* =============== TWI =============== */
/** TWI0 peripheral */
#define PIN_TWI_TWD0    {PIO_PA3A_TWD0,  PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_TWI_TWCK0   {PIO_PA4A_TWCK0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/** TWI1 peripheral */
#define PIN_TWI_TWD1    {PIO_PB4A_TWD1,  PIOB, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_TWI_TWCK1   {PIO_PB5A_TWCK1, PIOB, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 pin definitions */
#define PINS_TWI0       PIN_TWI_TWD0, PIN_TWI_TWCK0
/** TWI1 pin definitions */
#define PINS_TWI1       PIN_TWI_TWD1, PIN_TWI_TWCK1

/* =============== PCK =============== */
/** PCK0 pin definition. */
#define PIN_PCK0		(PIO_PA6_IDX)
#define PIN_PCK0_FLAGS	(PIO_PERIPH_B | PIO_DEFAULT)

#define PIN_PCK_0_MASK PIO_PA6
#define PIN_PCK_0_PIO PIOA
#define PIN_PCK_0_ID ID_PIOA
#define PIN_PCK_0_TYPE PIO_PERIPH_B
#define PIN_PCK_0_ATTR PIO_DEFAULT
/** PCK1 pin definition. */
#define PIN_PCK1        {PIO_PA17B_PCK1,PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}

/* =============== USART =============== */
/** RXD0 pin definition. */
#define PIN_USART0_RXD  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** TXD0 pin definition. */
#define PIN_USART0_TXD  {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** RTS0 pin definition. */
#define PIN_USART0_RTS  {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** CTS0 pin definition. */
#define PIN_USART0_CTS  {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART0 pin ENABLE */
#define PIN_USART0_EN   {PIO_PA2B_SCK0, PIOA, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_USART0_EN_IDX         (PIO_PA2_IDX)
#define PIN_USART0_EN_FLAGS       (PIO_OUTPUT_0 | PIO_DEFAULT)
/** SCK0 pin definition. */
#define PIN_USART0_SCK  {PIO_PA2B_SCK0, PIOA, ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
#define PIN_USART0_SCK_IDX        (PIO_PA2_IDX)
#define PIN_USART0_SCK_FLAGS      (PIO_PERIPH_B | PIO_DEFAULT)

/** RXD1 pin definition. */
#define PIN_USART1_RXD  {PIO_PA21A_RXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_RXD_IDX        (PIO_PA21_IDX)
#define PIN_USART1_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** TXD1 pin definition. */
#define PIN_USART1_TXD  {PIO_PA22A_TXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_TXD_IDX        (PIO_PA22_IDX)
#define PIN_USART1_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** RTS1 pin definition. */
#define PIN_USART1_RTS  {PIO_PA24A_RTS1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_RTS_IDX        (PIO_PA24_IDX)
#define PIN_USART1_RTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** CTS1 pin definition. */
#define PIN_USART1_CTS  {PIO_PA25A_CTS1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_CTS_IDX        (PIO_PA25_IDX)
#define PIN_USART1_CTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin ENABLE */
#define PIN_USART1_EN   {PIO_PA23A_SCK1, PIOA, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
#define PIN_USART1_EN_IDX         (PIO_PA23_IDX)
#define PIN_USART1_EN_FLAGS       (PIO_OUTPUT_0 | PIO_DEFAULT)
/** SCK1 pin definition. */
#define PIN_USART1_SCK  {PIO_PA23A_SCK1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART1_SCK_IDX        (PIO_PA23_IDX)
#define PIN_USART1_SCK_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)

/* =============== ISO7816 =============== */
/** PIN used for reset the smartcard */
#define PIN_ISO7816_RSTMC       {1 << 17, PIOA, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/** Pins used for connect the smartcard */
#define PINS_ISO7816            PIN_USART0_TXD, PIN_USART0_SCK, PIN_ISO7816_RSTMC
/** Base address of USART for the smartcard */
#define BOARD_ISO7816_BASE_USART    USART0
/** Peripheral ID of USART for the smartcard */
#define BOARD_ISO7816_ID_USART      ID_USART0


/*----------------------------------------------------------------------------*/
/**
 * \page sam3n_ek_mem "SAM3N-EK - Memories"
 * This page lists definitions related to internal & external on-board memories.
 *
 * \section SRAM
 * - \ref AT91C_IRAM
 * - \ref AT91C_IRAM_SIZE
 *
 * \section Flash
 * - \ref AT91C_IFLASH_SIZE
 * - \ref AT91C_IFLASH_PAGE_SIZE
 * - \ref AT91C_IFLASH_NB_OF_PAGES
 * - \ref AT91C_IFLASH_LOCK_REGION_SIZE
 * - \ref AT91C_IFLASH_NB_OF_LOCK_BITS
 */
/* Add new memories here. */


/*----------------------------------------------------------------------------*/
/**
 *  \page sam3n_ek_lcd "SAM3N-EK - LCD parameters"
 *  This page lists several definition related to the lcd parameters
 *
 *  \section Definitions
 *  - \ref BOARD_LCD_SPCK
 *  - \ref BOARD_LCD_NPCS
 *  - \ref BOARD_LCD_DLYBS
 *  - \ref BOARD_LCD_DLYBCT
 *  - \ref BOARD_LCD_WIDTH
 *  - \ref BOARD_LCD_HEIGHT
 */
/** LCD Spi Clock frequency */
#define BOARD_LCD_SPCK             (BOARD_MCK)
/** LCD Spi Chip Select */
#define BOARD_LCD_NPCS              2
/** LCD Spi Delay Before SPCK: >=10ns */
#define BOARD_LCD_DLYBS             1
/** LCD Spi DLYBCT: Delay Between Consecutive Transfers */
#define BOARD_LCD_DLYBCT            0
/** Display width in pixels. */
#define BOARD_LCD_WIDTH             176
/** Display height in pixels. */
#define BOARD_LCD_HEIGHT            220


/*----------------------------------------------------------------------------*/
/**
 *  \page sam3n_ek_sd "SAM3N-EK - SD(SPI) parameters"
 *  This page lists several definition related to the SD(SPI) parameters
 *
 *  \section Definitions
 *  - \ref BOARD_SD_SPCK
 *  - \ref BOARD_SD_PIN_CD
 *  - \ref BOARD_SD_SPI_BASE
 *  - \ref BOARD_SD_SPI_ID
 *  - \ref BOARD_SD_SPI_PINS
 *  - \ref BOARD_SD_SPI_IRQn
 *  - \ref BOARD_SD_NPCS
 */
/** SD Spi Clock frequency */
#define BOARD_SD_SPCK               10000000ul
/** SD Card Detect */
#define BOARD_SD_PIN_CD             {PIO_PA19, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP}
/** SD Spi Base */
#define BOARD_SD_SPI_BASE           SPI
/** SD Spi ID */
#define BOARD_SD_SPI_ID             ID_SPI
/** SD Spi Pins (and init other used NPCS to avoid conflict) */
#define BOARD_SD_SPI_PINS           PINS_SDCARD ,\
                                    PIN_SPI_NPCS1_ZIGBEE, \
                                    PIN_SPI_NPCS2_LCD, \
                                    PIN_SPI_NPCS3_AT25
/** SD SPI IRQn */
#define BOARD_SD_SPI_IRQn           SPI_IRQn
/** SD Spi Chip Select */
#define BOARD_SD_NPCS               0

/*----------------------------------------------------------------------------*/
/**
 *  \page sam3n_ek_sf "SAM3N-EK - Serial flash(SPI) parameters"
 *  This page lists several definition related to the serial flash(SPI) parameters.
 */
/** AT25 Spi Clock frequency */
#define BOARD_SF_SPCK               10000000ul
/** AT25 Spi Chip Select */
#define BOARD_AT25_NPCS             3

/*----------------------------------------------------------------------------*/
#define CONSOLE_UART				UART0
#define CONSOLE_UART_ID            ID_UART0

/* RE pin. */
#define PIN_RE_IDX                 PIN_USART0_CTS_IDX
#define PIN_RE_FLAGS               (PIO_OUTPUT_0 | PIO_DEFAULT)

/* IRDA SD pin. */
#define PIN_IRDA_SD_IDX            PIN_USART0_CTS_IDX
#define PIN_IRDA_SD_FLAGS          (PIO_OUTPUT_1 | PIO_DEFAULT)

/* TXD pin configuration. */
#define PIN_USART_TXD_IDX          PIN_USART0_TXD_IDX
#define PIN_USART_TXD_FLAGS        (PIO_PERIPH_A | PIO_DEFAULT)
#define PIN_USART_TXD_IO_FLAGS     (PIO_OUTPUT_0 | PIO_DEFAULT) 

#endif  // _SAM3N_EK_H_
