/*
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/***** Includes *****/
/* Standard C Libraries */
#include <stdio.h>
int bands=0;
/* TI Drivers */
#include <ti/display/Display.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/pin/PINCC26XX.h>
static PIN_Handle ledPinHandle;
static PIN_Handle pinHandle;
/* Application specific Header files */
#include "menu.h"
#include "config.h"
#include "smartrf_settings/smartrf_settings_predefined.h"
#include <stdint.h>
#include <stdio.h>
/* For sleep() */
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/UART.h>

/* Example/Board Header files */
#include "Board.h"
/***** Defines *****/
#define ADCBUFFERSIZE    (10)
#define UARTBUFFERSIZE   ((20 * ADCBUFFERSIZE) + 24)

uint16_t sampleBufferOne[ADCBUFFERSIZE];
uint16_t sampleBufferTwo[ADCBUFFERSIZE];
uint32_t microVoltBuffer[ADCBUFFERSIZE];
int LED=1;
#if (defined __CC1350_LAUNCHXL_BOARD_H__) || (defined __CC1350_LAUNCHXL_433_BOARD_H__) || (defined __CC1310_LAUNCHXL_BOARD_H__) || defined (__CC1352R1_LAUNCHXL_BOARD_H__)
extern PIN_Handle buttonPinHandle;
#endif

/***** Variable declarations *****/
int ON = 1;
int cycle_stop;
uint32_t packet_track = 0;
int packet_val = 0;
int adc_stop=0;
static int togg =10;
/* Events used in the application */
typedef enum
{
    MenuEvent_Navigate = 2,
    MenuEvent_Select = 1,
    MenuEvent_AnyButtonPushed = MenuEvent_Navigate + MenuEvent_Select,
} MenuEvent;

/* Menu row indices */
typedef enum
{
    TitleRow = 0,
    TestCaseRow,
    bandRow,
    PacketCountRow,
    OperationModeRow,
    StartRow,
    NrOfMainMenuRows,
} MenuIndex;

/* String constants for different boards */
#if defined __CC1310DK_7XD_BOARD_H__
    static const char* const button0Text = "UP";
    static const char* const button1Text = "DOWN";
#elif defined __CC1350_LAUNCHXL_BOARD_H__
    static const char* const button0Text = "BTN-1";
    static const char* const button1Text = "BTN-2";
#elif defined __CC1350_LAUNCHXL_433_BOARD_H__
    static const char* const button0Text = "BTN-1";
    static const char* const button1Text = "BTN-2";
#elif defined __CC1350STK_BOARD_H__
    static const char* const button0Text = "LEFT";
    static const char* const button1Text = "RIGHT";
#else
    #error This board is not supported.
#endif

/* Convenience macros for printing on a vt100 terminal via UART */
#define vt100_print0(handle, row, col, text) \
    Display_printf(handle, 0, 0, "\x1b[%d;%df" text, row+1, col+1)

#define vt100_print1(handle, row, col, formatString, arg1) \
    Display_printf(handle, 0, 0, "\x1b[%d;%df" formatString, row+1, col+1, arg1)

#define vt100_print2(handle, row, col, formatString, arg1, arg2) \
    Display_printf(handle, 0, 0, "\x1b[%d;%df" formatString, row+1, col+1, arg1, arg2)

#define vt100_clear(handle) \
    Display_printf(handle, 0, 0, "\x1b[2J\x1b[H")

#define vt100_setCursorVisible(handle, visible) \
    Display_printf(handle, 0, 0, "\x1b[?25%c", ((visible) == true) ? 'h' : 'l')

/* Holds the configuration for the current test case */
static ApplicationConfig config =
{
    RfSetup_Fsk,
    OperationMode_Rx,
    10,
    0,
    0,
#if (defined __CC1310_LAUNCHXL_BOARD_H__)
    RangeExtender_Dis
#endif
};

static Display_Handle lcdDisplay;
static Display_Handle uartDisplay;
static volatile uint8_t eventFlag = 0;

/***** Prototypes *****/
void menu_runTask();

bool menu_isButtonPressed()
{
    return (eventFlag != 0);
}

/*
Menu task function.

This task contains the main application logic. It prints the menu on both,
LCD and UART and starts the RX and TX test cases.
The setup code is generated from the .cfg file.
*/
void adcBufCallback(ADCBuf_Handle handle, ADCBuf_Conversion *conversion,
    void *completedADCBuffer, uint32_t completedChannel)
{
    ADCBuf_adjustRawValues(handle, completedADCBuffer, ADCBUFFERSIZE,
                                completedChannel);
                            ADCBuf_convertAdjustedToMicroVolts(handle, completedChannel,
                                completedADCBuffer, microVoltBuffer, ADCBUFFERSIZE);

                            /*
                              48              3.2 15                All On
                              47.53847        3.2 14.85577188       4GH Off
                              47.03847        3.2 14.69952188       4GM Off
                              46.53847        3.2 14.54327188       4GL Off
                              46.03847        3.2 14.38702188       3GH Off
                              45.53847        3.2 14.23077188       3GM Off
                              45.03847        3.2 14.07452188       3GL Off
                              44.53847        3.2 13.91827188       2GH Off
                              44.03847        3.2 13.76202188       2GM Off
                              43.53847        3.2 13.60577188       2GL Off
                              43.03847        3.2 13.44952188       GSMGH Off
                              42.53847        3.2 13.29327188       GSMGM Off
                              42.03847        3.2 13.13702188       GSML Off
                              42              3.2 13.125            All Off


                             */
if(ON==0 && microVoltBuffer[1] > 3180000){
   togg+=10;


}else if(microVoltBuffer[1] > 3170000 && microVoltBuffer[1] < 3180000){

   // cycle_stop=0;
                                //      GPIO_toggle(Board_GPIO_LED0);
                                PIN_setOutputValue(pinHandle,PIN_ID(7),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),1);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);//make this last pin
                            }


                            else if(microVoltBuffer[1] < 3170000 && microVoltBuffer[1] > 3158000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                                                PIN_setOutputValue(pinHandle,PIN_ID(3),1);

                                                                PIN_setOutputValue(pinHandle,PIN_ID(19),1);


                                                                PIN_setOutputValue(pinHandle,PIN_ID(18),1);//fix

                                                                PIN_setOutputValue(pinHandle,PIN_ID(15),1);

                                                                PIN_setOutputValue(pinHandle,PIN_ID(27),1);


                                                                PIN_setOutputValue(pinHandle,PIN_ID(20),1);

                                                                PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                                                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }



                            else if(microVoltBuffer[1] < 3150000 && microVoltBuffer[1] > 3120000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),1);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 3120000 && microVoltBuffer[1] > 3090000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),1);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 3090000 && microVoltBuffer[1] > 3050000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 3058000 && microVoltBuffer[1] > 3025000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 3025000 && microVoltBuffer[1] > 2992000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 2992000 && microVoltBuffer[1] > 2959000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 2959000 && microVoltBuffer[1] > 2925000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 2925000 && microVoltBuffer[1] > 2892000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 2892000 && microVoltBuffer[1] > 2859000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 2859000 && microVoltBuffer[1] > 2826000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),1);
                            }


                            else if(microVoltBuffer[1] < 2826000 && microVoltBuffer[1] > 2792000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),0);
                            }


                            else if(microVoltBuffer[1] < 2792000){
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(3),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),0);
                            }


                          /*  else {
                                //      GPIO_toggle(Board_GPIO_LED0);
                                PIN_setOutputValue(pinHandle,PIN_ID(7),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(13),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(18),1);//fix

                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(25),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(26),0);


                                PIN_setOutputValue(pinHandle,PIN_ID(6),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(21),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(11),0);//make this last pin
                            }*/


/*                            if(microVoltBuffer[1] > 2000000){
                                          //      GPIO_toggle(Board_GPIO_LED0);
                                           PIN_setOutputValue(pinHandle,PIN_ID(7),1);
                                           PIN_setOutputValue(pinHandle,PIN_ID(3),1);
                                           PIN_setOutputValue(pinHandle,PIN_ID(19),1);
                                           PIN_setOutputValue(pinHandle,PIN_ID(18),1);//fix


                                           PIN_setOutputValue(pinHandle,PIN_ID(15),1);
                                           PIN_setOutputValue(pinHandle,PIN_ID(27),1);

                                           PIN_setOutputValue(pinHandle,PIN_ID(20),1);
                                           PIN_setOutputValue(pinHandle,PIN_ID(25),1);
                                           PIN_setOutputValue(pinHandle,PIN_ID(26),1);

                                           PIN_setOutputValue(pinHandle,PIN_ID(6),1);
                                           PIN_setOutputValue(pinHandle,PIN_ID(21),1);
                                           PIN_setOutputValue(pinHandle,PIN_ID(11),1);//make this last pin
                                       }
                            else if(microVoltBuffer[1] < 2000000){
                                PIN_setOutputValue(pinHandle,PIN_ID(17),0);
                                PIN_setOutputValue(pinHandle,PIN_ID(19),0);
                                PIN_setOutputValue(pinHandle,PIN_ID(18),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);
                                PIN_setOutputValue(pinHandle,PIN_ID(15),0);
                                PIN_setOutputValue(pinHandle,PIN_ID(27),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(20),0);
                                PIN_setOutputValue(pinHandle,PIN_ID(21),0);
                                PIN_setOutputValue(pinHandle,PIN_ID(26),0);

                                PIN_setOutputValue(pinHandle,PIN_ID(6),0);
                                PIN_setOutputValue(pinHandle,PIN_ID(23),0);
                                PIN_setOutputValue(pinHandle,PIN_ID(11),0);
                            }
*/
                        // Toggle LED
                      //  GPIO_toggle(Board_GPIO_LED1);
                           // Start a new conversion
                           ADCBuf_convert(handle, conversion, 1);


                    /*
                     * Callback function to use the UART in callback mode. It does nothing.
                     */}
void menu_init()
{


    config.frequencyTable = config_frequencyTable_Lut[config.rfSetup];

    /* Init displays */
    Display_Params params;
    Display_Params_init(&params);
    params.lineClearMode = DISPLAY_CLEAR_NONE;

    lcdDisplay = Display_open(Display_Type_LCD, &params);
    if(lcdDisplay == NULL)
    {
        while(1);
    }

    Display_clear(lcdDisplay);

    uartDisplay = Display_open(Display_Type_UART, &params);
    if(uartDisplay == NULL)
    {
        while(1);
    }
}

void menu_runTask()
{
    UART_Params uartParams;
          ADCBuf_Handle adcBuf;
          ADCBuf_Params adcBufParams;
          ADCBuf_Conversion continuousConversion;

          /* Call driver init functions */
          ADCBuf_init();

          /* Set up an ADCBuf peripheral in one shot */
                ADCBuf_Params_init(&adcBufParams);
                adcBufParams.callbackFxn = adcBufCallback;
                adcBufParams.recurrenceMode = ADCBuf_RECURRENCE_MODE_ONE_SHOT;
                adcBufParams.returnMode = ADCBuf_RETURN_MODE_CALLBACK;
                adcBufParams.samplingFrequency = 2;
                adcBuf = ADCBuf_open(Board_ADCBUF0, &adcBufParams);

                ADCBuf_Conversion singleConversion;

            /* Configure the conversion struct
            continuousConversion.arg = NULL;
            continuousConversion.adcChannel = 7;
            continuousConversion.sampleBuffer = sampleBufferOne;
            continuousConversion.sampleBufferTwo = sampleBufferTwo;
            continuousConversion.samplesRequestedCount = ADCBUFFERSIZE;*/

                /* Configure the conversion struct for one shot */
                singleConversion.arg = NULL;
                singleConversion.adcChannel = 6;
                singleConversion.sampleBuffer = sampleBufferOne;
                singleConversion.sampleBufferTwo = NULL;
                singleConversion.samplesRequestedCount = ADCBUFFERSIZE;

           /* if (adcBuf == NULL){
                // ADCBuf failed to open.
                while(1);
            }*/
                if (!adcBuf){
                        /* AdcBuf did not open correctly. */
                        while(1);
                    }



            /* Start converting for continous.
            if (ADCBuf_convert(adcBuf, &continuousConversion, 1) !=
                ADCBuf_STATUS_SUCCESS) {
                // Did not start conversion process correctly.
                while(1);
            }*/
                    /* Start converting. for one single conversion*/
                        if (ADCBuf_convert(adcBuf, &singleConversion, 1) !=
                            ADCBuf_STATUS_SUCCESS) {
                            /* Did not start conversion process correctly. */
                            while(1);
                        }
    uint8_t cursorRow = TestCaseRow;
    uint8_t packetIndex = 0;
    vt100_clear(uartDisplay);
    vt100_setCursorVisible(uartDisplay, false);

    /* Splash screen */
    Display_printf(lcdDisplay, 0, 0, "Selective");
    Display_printf(lcdDisplay, 1, 0, "Power Controller");
    Display_printf(lcdDisplay, 3, 0, "Select:   %s", button0Text);
    Display_printf(lcdDisplay, 4, 0, "Navigate: %s", button1Text);
    Display_printf(lcdDisplay, 6, 0, "Push a button");
    Display_printf(lcdDisplay, 7, 0, "to proceed...");



    bool previousHwiState = IntMasterDisable();
    // Tricky IntMasterDisable():
    //true  : Interrupts were already disabled when the function was called.
    //false : Interrupts were enabled and are now disabled.
    while(!eventFlag){
        IntMasterEnable();
        Power_idleFunc();
        IntMasterDisable();

    }
    if(!previousHwiState)
    {
        IntMasterEnable();
    }
    eventFlag = 0;
    Display_clear(lcdDisplay);
    vt100_clear(uartDisplay);

#if (defined __CC1350_LAUNCHXL_BOARD_H__) || (defined __CC1350_LAUNCHXL_433_BOARD_H__)
    /* If it's CC1350 launchpad, configure GPIO to enable power switch and set RF antenna switch to Sub 1GHz as default.
     * This is necessary as the default testcase is Sub 1GHz 2-GFSK. Otherwise if the first tescase to run is Sub 1GHz,
     * it will run without correct GPIO settings */
    /* Enable power to RF switch */
    PIN_setOutputValue(buttonPinHandle, Board_DIO30_SWPWR, 1);
    /* Switch RF switch to Sub 1GHz antenna */
    PIN_setOutputValue(buttonPinHandle, Board_DIO1_RFSW, 1);
#endif

    while(true)
    {
        /* Main Menu */
        Display_printf(lcdDisplay, 0, 0, "Main Menu");
        Display_printf(lcdDisplay, TestCaseRow, 0,      " Remove:%s", config_rfSetupLabels[config.rfSetup]);
        Display_printf(lcdDisplay, bandRow, 0,     " Band: %s", config_Band[config.band]);
        Display_printf(lcdDisplay, PacketCountRow, 0,   " Loading: %-5d", (config.packetCount=config_packetCountTable[packet_val]));


        if((config.rfSetup == 0) && (config.band == 0)){ //4G ALL
            packet_val = 9;
        }
        else if ((config.rfSetup == 0) && (config.band == 1)){//4G HIGH
            packet_val = 8;
        }else if ((config.rfSetup == 0) && (config.band == 2)){//4G MID
            packet_val = 7;
        }else if ((config.rfSetup == 0) && (config.band == 3)){//4G LOW
            packet_val = 6;
        }else if ((config.rfSetup == 1) && (config.band == 0)){//3G ALL
            packet_val = 5;
        }else if ((config.rfSetup == 1) && (config.band == 1)){//3G HIGH
            packet_val = 4;
        }else if ((config.rfSetup == 1) && (config.band == 2)){//3G MID
            packet_val = 3;
        }else if ((config.rfSetup == 2) && (config.band == 2)){//2G MID
            packet_val = 2;
        }else if ((config.rfSetup == 3) && (config.band == 0)){//RESET
            packet_val = 10;
        }else if ((config.rfSetup == 4) && (config.band == 0)){//PRESET
            packet_val = 11;
        }


        Display_printf(lcdDisplay, OperationModeRow, 0, " Mode: %s", config_opmodeLabels[config.operationMode]);

        Display_printf(lcdDisplay, StartRow, 0, " Start...");

  //      vt100_print0(uartDisplay, 0, 0, "Main Menu");
 //       vt100_print1(uartDisplay, TestCaseRow, 0,      " Test: %s", config_rfSetupLabels[config.rfSetup]);
  //      vt100_print1(uartDisplay, FrequencyRow, 0,     " Freq: %s", config.frequencyTable[config.frequency].label);
 //       vt100_print1(uartDisplay, PacketCountRow, 0,   " Pkts: %-5d", config.packetCount);
  //      vt100_print1(uartDisplay, OperationModeRow, 0, " Mode: %s", config_opmodeLabels[config.operationMode]);
  //      vt100_print0(uartDisplay, StartRow, 0, " Start...");

        /* Print the selector */
        Display_printf(lcdDisplay, cursorRow, 0, ">");
        vt100_print0(uartDisplay, cursorRow, 0, ">" "\x1b[1A"); // Overlay selector and cursor

        previousHwiState = IntMasterDisable();
        /* Navigation is done event based. Events are created from button interrupts */
        while(!eventFlag)
        {
            IntMasterEnable();
            Power_idleFunc();
            IntMasterDisable();
        }
        if(!previousHwiState)
        {
            IntMasterEnable();
        }
        if (eventFlag & MenuEvent_Navigate)
        {
            eventFlag &= !MenuEvent_Navigate;
            cursorRow++;
            if (cursorRow >= NrOfMainMenuRows)
            {
                cursorRow = TestCaseRow;
            }
        }
        if (eventFlag & MenuEvent_Select)
        {
            eventFlag &= !MenuEvent_Select;
            switch(cursorRow)
            {
                case TestCaseRow:

                    config.rfSetup = (RfSetup)((config.rfSetup + 1) % NrOfRfSetups);
                    config.frequencyTable = config_frequencyTable_Lut[config.rfSetup];
                    config.frequency = 0;

#if (defined __CC1350_LAUNCHXL_BOARD_H__) || (defined __CC1350_LAUNCHXL_433_BOARD_H__)
                    if(config.rfSetup == RfSetup_Ble)
                    {
                        // Enable power to RF switch
                        PIN_setOutputValue(buttonPinHandle, Board_DIO30_SWPWR, 1);

                        // Switch RF switch to 2.4 GHz antenna
                        PIN_setOutputValue(buttonPinHandle, Board_DIO1_RFSW, 0);
                    }
                    else
                    {
                        // Enable power to RF switch
                        PIN_setOutputValue(buttonPinHandle, Board_DIO30_SWPWR, 1);

                        // Switch RF switch to Sub 1GHz antenna
                        PIN_setOutputValue(buttonPinHandle, Board_DIO1_RFSW, 1);
                    }
#endif
                    break;



            case PacketCountRow:

                packetIndex = (packetIndex + 1) % config_NrOfPacketCounts;
                config.packetCount = config_packetCountTable[packetIndex];
                break;

            case OperationModeRow:

                config.operationMode = (OperationMode)((config.operationMode + 1) % NrOfOperationModes);
                break;

            case bandRow:

                config.band = (ConfigBand)((config.band +1) % NrOfConfigBand);
                break;


            case StartRow:

                if (config.operationMode == OperationMode_Rx)
                {
                    PIN_setOutputValue(pinHandle,PIN_ID(7),1);

                    PIN_setOutputValue(pinHandle,PIN_ID(3),1);

                    PIN_setOutputValue(pinHandle,PIN_ID(19),1);


                    PIN_setOutputValue(pinHandle,PIN_ID(18),1);//fix

                    PIN_setOutputValue(pinHandle,PIN_ID(15),1);

                    PIN_setOutputValue(pinHandle,PIN_ID(27),1);


                    PIN_setOutputValue(pinHandle,PIN_ID(20),1);

                    PIN_setOutputValue(pinHandle,PIN_ID(25),1);

                    PIN_setOutputValue(pinHandle,PIN_ID(26),1);


                    PIN_setOutputValue(pinHandle,PIN_ID(6),1);

                    PIN_setOutputValue(pinHandle,PIN_ID(21),1);

                    PIN_setOutputValue(pinHandle,PIN_ID(11),1);//make this last pin


                    /* Prepare RX display */
                    Display_clear(lcdDisplay);
                    Display_printf(lcdDisplay, 0, 0, "Receiving...");
                    Display_printf(lcdDisplay, 1, 0, "%s %s",
                            config_rfSetupLabels[config.rfSetup],
                            config.frequencyTable[config.frequency].label);
                    Display_printf(lcdDisplay, 2, 0, "Pkts ok   :%-5d", 0);
                    Display_printf(lcdDisplay, 3, 0, "RSSI [dBm]:n/a");
                    Display_printf(lcdDisplay, 4, 0, "PER  [%%] :n/a");
                    Display_printf(lcdDisplay, 6, 0, "Push a button");
                    Display_printf(lcdDisplay, 7, 0, "to abort.");


                    /* Run the test. We are not interersted in the result. */
                    rx_runRxTest(&config);

                    eventFlag = 0;
                }
                else
                {
                    /* Prepare TX display */
                    Display_clear(lcdDisplay);
                    Display_printf(lcdDisplay, 0, 0, "Sending command...");
                    Display_printf(lcdDisplay, 1, 0, "Remove: %s",
                                   config_rfSetupLabels[config.rfSetup]);
                    Display_printf(lcdDisplay, 3, 0, "Sending: %-5d", 0);


                    /* Run the test. */
                    TestResult result = tx_runTxTest(&config);
                    if (result == TestResult_Aborted)
                    {
                        Display_printf(lcdDisplay, 6, 0, "...aborted.");
                        vt100_print0(uartDisplay, 6, 0, "...aborted.");
                        eventFlag = 0;
                    }
                    else if (result == TestResult_Finished)
                    {
                        Display_printf(lcdDisplay, 6, 0, "...finished.");
                        vt100_print0(uartDisplay, 6, 0, "...finished.");
                    }
                    Display_printf(lcdDisplay, 7, 0, "Push a button...");
                    vt100_print0(uartDisplay, 7, 0, "Push a button...");

                    previousHwiState = IntMasterDisable();
                    while(!eventFlag)
                    {
                        IntMasterEnable();
                        Power_idleFunc();
                        IntMasterDisable();
                    }
                    if(!previousHwiState)
                    {
                        IntMasterEnable();
                    }
                    eventFlag = 0;
                }
                Display_clear(lcdDisplay);
                vt100_clear(uartDisplay);
                break;
            }
        }
    }
}

/*
Callback for button interrupts.

This function is supposed to be called asynchronously from within an interrupt
handler and signals a button press event to the application logic.
*/
void menu_notifyButtonPressed(Button button)
{
    if (button == Button_Navigate)
    {
        eventFlag |= MenuEvent_Navigate;
    }
    else
    {
        eventFlag |= MenuEvent_Select;
    }
}

/*
Updates the screen content during an ongoing receive.

Call this function from any other task to refresh the menu with
updated parameters.
*/
void menu_updateRxScreen(uint32_t packetsReceived, int8_t rssi)
{
   static int counter = 0;

   counter++;
    Display_printf(lcdDisplay, 2, 11, "%-5d", packetsReceived);
    Display_printf(lcdDisplay, 3, 11, "%-5d", counter);
    Display_printf(lcdDisplay, 4, 0, "PER  [%%] :");
    vt100_print1(uartDisplay, 2, 0, "Pkts ok   : %-5d", packetsReceived);
    vt100_print1(uartDisplay, 5, 0, "Pk trck  : %-5d", packet_track);

    vt100_print1(uartDisplay, 3, 0, "RSSI [dBm]: %-5i", rssi);
    vt100_print0(uartDisplay, 4, 0, "PER  [%%] :");


    if (packetsReceived > 0){
        ON=0;
        }



if (counter == 10){
    PIN_setOutputValue(pinHandle,PIN_ID(15),0);// 4GH
    PIN_setOutputValue(pinHandle,PIN_ID(27),0);// 4GH
    PIN_setOutputValue(pinHandle,PIN_ID(20),0);//4GM
    PIN_setOutputValue(pinHandle,PIN_ID(25),0);//4GM
    PIN_setOutputValue(pinHandle,PIN_ID(26),0);//4GL
    PIN_setOutputValue(pinHandle,PIN_ID(6),0); //4GL
    PIN_setOutputValue(pinHandle,PIN_ID(7),1);// 3GH
    PIN_setOutputValue(pinHandle,PIN_ID(3),1);// 3GH
    PIN_setOutputValue(pinHandle,PIN_ID(19),1); //3GM
    PIN_setOutputValue(pinHandle,PIN_ID(18),1); //3GM

}else if (counter == 9){
    PIN_setOutputValue(pinHandle,PIN_ID(15),0);// 4GH
    PIN_setOutputValue(pinHandle,PIN_ID(27),0);// 4GH


}else if (counter == 8){
    PIN_setOutputValue(pinHandle,PIN_ID(20),0);//4GM
    PIN_setOutputValue(pinHandle,PIN_ID(25),0);//4GM


}else if (counter == 7){
    PIN_setOutputValue(pinHandle,PIN_ID(26),0);//4GL
    PIN_setOutputValue(pinHandle,PIN_ID(6),0); //4GL
}else if (counter == 6){
    PIN_setOutputValue(pinHandle,PIN_ID(7),0);// 3GH
    PIN_setOutputValue(pinHandle,PIN_ID(3),0);// 3GH
    PIN_setOutputValue(pinHandle,PIN_ID(19),0); //3GM
    PIN_setOutputValue(pinHandle,PIN_ID(18),0); //3GM
}else if (counter == 5){
    PIN_setOutputValue(pinHandle,PIN_ID(7),0);// 3GH
    PIN_setOutputValue(pinHandle,PIN_ID(3),0);// 3GH
}else if (counter == 4){
    PIN_setOutputValue(pinHandle,PIN_ID(19),0); //3GM
    PIN_setOutputValue(pinHandle,PIN_ID(18),0); //3GM
    PIN_setOutputValue(pinHandle,PIN_ID(21),1);// 2GM
     PIN_setOutputValue(pinHandle,PIN_ID(11),1);// 2GM
}else if (counter == 3){
    PIN_setOutputValue(pinHandle,PIN_ID(21),0);// 2GM
    PIN_setOutputValue(pinHandle,PIN_ID(11),0);// 2GM
}else if (packetsReceived == 22){
    PIN_setOutputValue(pinHandle,PIN_ID(7),1);
     PIN_setOutputValue(pinHandle,PIN_ID(3),0);
     PIN_setOutputValue(pinHandle,PIN_ID(19),1);
     PIN_setOutputValue(pinHandle,PIN_ID(18),0);
     PIN_setOutputValue(pinHandle,PIN_ID(15),1);
     PIN_setOutputValue(pinHandle,PIN_ID(27),0);
     PIN_setOutputValue(pinHandle,PIN_ID(20),1);
     PIN_setOutputValue(pinHandle,PIN_ID(25),0);
     PIN_setOutputValue(pinHandle,PIN_ID(26),1);
     PIN_setOutputValue(pinHandle,PIN_ID(6),0);
     PIN_setOutputValue(pinHandle,PIN_ID(21),1);
     PIN_setOutputValue(pinHandle,PIN_ID(11),0);
//     packetsReceived = packetsReceived-packetsReceived;
     counter = counter-counter;


}

while (packetsReceived >65){
if(((togg/10)%2)==1){
    PIN_setOutputValue(pinHandle,PIN_ID(7),1);
     PIN_setOutputValue(pinHandle,PIN_ID(3),0);
     PIN_setOutputValue(pinHandle,PIN_ID(19),1);
     PIN_setOutputValue(pinHandle,PIN_ID(18),0);
     PIN_setOutputValue(pinHandle,PIN_ID(15),1);
     PIN_setOutputValue(pinHandle,PIN_ID(27),0);
     PIN_setOutputValue(pinHandle,PIN_ID(20),1);
     PIN_setOutputValue(pinHandle,PIN_ID(25),0);
     PIN_setOutputValue(pinHandle,PIN_ID(26),1);
     PIN_setOutputValue(pinHandle,PIN_ID(6),0);
     PIN_setOutputValue(pinHandle,PIN_ID(21),1);
     PIN_setOutputValue(pinHandle,PIN_ID(11),0);
  }else{
     PIN_setOutputValue(pinHandle,PIN_ID(7),0);
          PIN_setOutputValue(pinHandle,PIN_ID(3),1);
          PIN_setOutputValue(pinHandle,PIN_ID(19),0);
          PIN_setOutputValue(pinHandle,PIN_ID(18),1);
          PIN_setOutputValue(pinHandle,PIN_ID(15),0);
          PIN_setOutputValue(pinHandle,PIN_ID(27),1);
          PIN_setOutputValue(pinHandle,PIN_ID(20),0);
          PIN_setOutputValue(pinHandle,PIN_ID(25),1);
          PIN_setOutputValue(pinHandle,PIN_ID(26),0);
          PIN_setOutputValue(pinHandle,PIN_ID(6),1);
          PIN_setOutputValue(pinHandle,PIN_ID(21),0);
          PIN_setOutputValue(pinHandle,PIN_ID(11),1);}


}


 if (counter == 30){

     PIN_setOutputValue(pinHandle,PIN_ID(7),1);
     PIN_setOutputValue(pinHandle,PIN_ID(3),1);
     PIN_setOutputValue(pinHandle,PIN_ID(19),1);
     PIN_setOutputValue(pinHandle,PIN_ID(18),1);
     PIN_setOutputValue(pinHandle,PIN_ID(15),1);
     PIN_setOutputValue(pinHandle,PIN_ID(27),1);
     PIN_setOutputValue(pinHandle,PIN_ID(20),1);
     PIN_setOutputValue(pinHandle,PIN_ID(25),1);
     PIN_setOutputValue(pinHandle,PIN_ID(26),1);
     PIN_setOutputValue(pinHandle,PIN_ID(6),1);
     PIN_setOutputValue(pinHandle,PIN_ID(21),1);
     PIN_setOutputValue(pinHandle,PIN_ID(11),1);
     counter++;
     counter = counter-counter;

 }

   /* //4G/LTE config
    if(config.rfSetup == RfSetup_Custom && !OperationMode_Tx && config.band == 1)    //4G/LTE - High
            {
        PIN_setOutputValue(pinHandle,PIN_ID(7),0);
                            }

    if(config.rfSetup == RfSetup_Custom && !OperationMode_Tx && config.band == 2)    //4G/LTE - Mid
            {
        PIN_setOutputValue(pinHandle,PIN_ID(3),0);
                            }

    if(config.rfSetup == RfSetup_Custom && !OperationMode_Tx && config.band == 3)    //4G/LTE - Low
            {
        PIN_setOutputValue(pinHandle,PIN_ID(19),0);
                            }



    // 3G Band Config
    if(config.rfSetup == RfSetup_Fsk && !OperationMode_Tx && config.band == 1)       // 3G - High
            {
        PIN_setOutputValue(pinHandle,PIN_ID(18),0);
                            }

    if(config.rfSetup == RfSetup_Fsk && !OperationMode_Tx && config.band == 2)       // 3G - Mid
            {
        PIN_setOutputValue(pinHandle,PIN_ID(15),0);
                            }

    if(config.rfSetup == RfSetup_Fsk && !OperationMode_Tx && config.band == 3)       // 3G - Low
            {
        PIN_setOutputValue(pinHandle,PIN_ID(27),0);
                            }



    //2G Band Config
    if(config.rfSetup == RfSetup_Lrm && !OperationMode_Tx && config.band == 1)       // 2G - High
               {
        PIN_setOutputValue(pinHandle,PIN_ID(20),0);
                               }

    if(config.rfSetup == RfSetup_Lrm && !OperationMode_Tx && config.band == 2)       // 2G - Mid
               {
        PIN_setOutputValue(pinHandle,PIN_ID(25),0);
                               }

    if(config.rfSetup == RfSetup_Lrm && !OperationMode_Tx && config.band == 3)       // 2G - Low
               {
        PIN_setOutputValue(pinHandle,PIN_ID(26),0);
                               }


    //GSM Band Config
    if(config.rfSetup == RfSetup_Sl_lr && !OperationMode_Tx && config.band == 1)     //GSM - High
                {
        PIN_setOutputValue(pinHandle,PIN_ID(6),0);
                               }

    if(config.rfSetup == RfSetup_Sl_lr && !OperationMode_Tx && config.band == 2)     //GSM - Mid
                 {
        PIN_setOutputValue(pinHandle,PIN_ID(21),0);
                               }

    if(config.rfSetup == RfSetup_Sl_lr && !OperationMode_Tx && config.band == 3)     //GSM - Low
                 {
      PIN_setOutputValue(pinHandle,PIN_ID(11),0);
                               }


    //All Band Config
    if(config.rfSetup == RfSetup_Ook)
            {
             PIN_setOutputValue(pinHandle,PIN_ID(7),0);  //4G High - Off
             PIN_setOutputValue(pinHandle,PIN_ID(3),0);  //4G Mid - Off
             PIN_setOutputValue(pinHandle,PIN_ID(19),0); //4G Low - Off

             PIN_setOutputValue(pinHandle,PIN_ID(18),0); //3G High - Off
             PIN_setOutputValue(pinHandle,PIN_ID(15),0); //3G Mid - Off
             PIN_setOutputValue(pinHandle,PIN_ID(27),0); //3G Low - Off

             PIN_setOutputValue(pinHandle,PIN_ID(120),0); //2G High - Off
             PIN_setOutputValue(pinHandle,PIN_ID(25),0); //2G Mid - Off
             PIN_setOutputValue(pinHandle,PIN_ID(26),0); //2G Low - Off

             PIN_setOutputValue(pinHandle,PIN_ID(6),0); //GSM High - Off
             PIN_setOutputValue(pinHandle,PIN_ID(21),0); //GSM Mid - Off
            PIN_setOutputValue(pinHandle,PIN_ID(11),0); //GSM Low - Off
               }

    //Preset 1 Band Config
    if(config.rfSetup == RfSetup_Hsm)       //Preset 1
               {
        PIN_setOutputValue(pinHandle,PIN_ID(7),0);  //4G High - Off
        PIN_setOutputValue(pinHandle,PIN_ID(3),0);  //4G Mid - Off
        PIN_setOutputValue(pinHandle,PIN_ID(19),0); //4G Low - Off

        PIN_setOutputValue(pinHandle,PIN_ID(18),0); //3G High - Off
        PIN_setOutputValue(pinHandle,PIN_ID(15),0); //3G Mid - Off
        PIN_setOutputValue(pinHandle,PIN_ID(27),0); //3G Low - Off
               }

    //Preset 2 Band Config
    if(config.rfSetup == RfSetup_Ble)       //Preset 2
                   {
                    PIN_setOutputValue(pinHandle,PIN_ID(7),0);  //4G High - Off
                    PIN_setOutputValue(pinHandle,PIN_ID(3),0);  //4G Mid - Off
                    PIN_setOutputValue(pinHandle,PIN_ID(19),0); //4G Low - Off

                    PIN_setOutputValue(pinHandle,PIN_ID(18),0); //3G High - Off
                    PIN_setOutputValue(pinHandle,PIN_ID(15),0); //3G Mid - Off
                    PIN_setOutputValue(pinHandle,PIN_ID(27),0); //3G Low - Off

                    PIN_setOutputValue(pinHandle,PIN_ID(120),0); //2G High - Off
                    PIN_setOutputValue(pinHandle,PIN_ID(25),0); //2G Mid - Off
                    PIN_setOutputValue(pinHandle,PIN_ID(26),0); //2G Low - Off
                   }*/



    /* Convert float to string buffer */
    if (packetsReceived <= config.packetCount)
    {
        float per = (float)((config.packetCount - packetsReceived) * 100.0f / config.packetCount);
        int characteristic = (int)per;
        int mantissa = (int)((per - characteristic)*100);

        Display_printf(lcdDisplay, 4, 10, "%3d", characteristic);
        vt100_print1(uartDisplay, 4, 10, "%3d", characteristic);

        Display_printf(lcdDisplay, 4, 13, "%c", '.');
        vt100_print1(uartDisplay, 4, 13, "%c", '.');

        Display_printf(lcdDisplay, 4, 14, "%-2d", mantissa);
        vt100_print1(uartDisplay, 4, 14, "%-2d", mantissa);
    }
    else
    {
        char buffer[6] = "n/a  ";
        Display_printf(lcdDisplay, 4, 11, "%s", &buffer);
        vt100_print1(uartDisplay, 4, 0, "PER  [%%] : %s", &buffer);
    }
}

/*
Updates the screen content during an ongoing transmission.

Call this function from any other task to refresh the menu with
updated parameters.
 */
void menu_updateTxScreen(uint32_t packetsSent)
{
    Display_printf(lcdDisplay, 3, 11, "%-5d", packetsSent);
    vt100_print1(uartDisplay, 3, 11, "%-5d", packetsSent);
}
