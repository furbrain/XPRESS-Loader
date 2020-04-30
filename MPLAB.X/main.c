/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

 MSD Loader and CDC Interface for the MPLAB XPRESS Evaluation Board
   
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 
*******************************************************************************/

/** INCLUDES *******************************************************/
#include <pic16f1455.h>

#include "system.h"     // find release MAJOR/MINOR and DATE in here!!!
#include "system_config.h"

#include "usb.h"
#include "usb_device_msd.h"

#include "app_device_msd.h"
#include "direct.h"
#include "tmr1.h"
#include "tmr2.h"
#include "pwm2.h"

/********************************************************************
 * Function:        void main(void)
 *******************************************************************/
inline void goto_app(void) {
    PWM2CONbits.PWM2EN = 0;
    LATCbits.LATC3 = 0;    
    asm ("movlp 0x16"); 
    asm ("goto 0x600");    
}

inline void throb(void) {
    static uint8_t duty = PWM2_MAX_VALUE;
    static bool throb_up = false;
    if (TMR1_HasOverflowOccured()) {
        PWM2_Initialize();
        if (throb_up) {
            duty++;
            if (duty >= PWM2_MAX_VALUE) throb_up = false;
        } else {
            duty--;
            if (duty <= 6) throb_up = true;                
        }
        PWM2_LoadDutyValue(duty);
        PIR1bits.TMR1IF = 0;
        TMR1_Reload();
    }
}

#define charged() (PORTAbits.RA5)

void run_usb(void) {
    USBDeviceInit();
    USBDeviceAttach();
    TMR1_Initialize();
    TMR1_StartTimer();
    TMR2_Initialize();
    TMR2_StartTimer();
    PWM2_Initialize();
    while(1)
    {
        SYSTEM_Tasks();

        #if defined(USB_POLLING)
            USBDeviceTasks();
        #endif
            if (charged()) {
                PWM2_Off();
                LATCbits.LATC3 = 1;
                
            } else {
                throb();
            }
        
        /* If the USB device isn't configured yet, we can't really do anything
         * else since we don't have a host to talk to.  So jump back to the
         * top of the while loop. */
        if( USBGetDeviceState() < CONFIGURED_STATE )
        {   /* USB connection not available or not yet complete */
            // implement nMCLR button 
            /* Jump back to the top of the while loop. */
            continue;
        }

        /* If we are currently suspended, then we need to see if we need to
         * issue a remote wakeup.  In either case, we shouldn't process any
         * keyboard commands since we aren't currently communicating to the host
         * thus just continue back to the start of the while loop. */
        if( USBIsDeviceSuspended() == true )
        {
            /* Jump back to the top of the while loop. */
            continue;
        }
        //Application specific tasks
        APP_DeviceMSDTasks();
    }//end while    
}

bool isUSBPower(void) {
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO == 1);
    return (ADRES < 476);
}


MAIN_RETURN main(void)
{
    SYSTEM_Initialize();
    LATAbits.LATA5 = 0;
    __delay_ms(1);
    if (isUSBPower()) {
        run_usb();
    } else {
        goto_app();
    } 
}//end main


/*******************************************************************
 * Function:        bool USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, uint16_t size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  uint16_t size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch((int)event)
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;

        case EVENT_SOF:
            break;

        case EVENT_SUSPEND:
            break;

        case EVENT_RESUME:
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the demo
             * code. */
            APP_DeviceMSDInitialize();

            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The MSD driver
             * needs to check to see if the request was for it. */
            USBCheckMSDRequest();

            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT
            //      endpoints).

            //Check if the host recently did a clear endpoint halt on the MSD OUT endpoint.
            //In this case, we want to re-arm the MSD OUT endpoint, so we are prepared
            //to receive the next CBW that the host might want to send.
            //Note: If however the STALL was due to a CBW not valid condition,
            //then we are required to have a persistent STALL, where it cannot
            //be cleared (until MSD reset recovery takes place).  See MSD BOT
            //specs v1.0, section 6.6.1.
            if(MSDWasLastCBWValid() == false)
            {
                //Need to re-stall the endpoints, for persistent STALL behavior.
                USBStallEndpoint(MSD_DATA_IN_EP, IN_TO_HOST);
                USBStallEndpoint(MSD_DATA_OUT_EP, OUT_FROM_HOST);
            }
            else
            {
                //Check if the host cleared halt on the bulk out endpoint.  In this
                //case, we should re-arm the endpoint, so we can receive the next CBW.
                if((USB_HANDLE)pdata == USBGetNextHandle(MSD_DATA_OUT_EP, OUT_FROM_HOST))
                {
                    USBMSDOutHandle = USBRxOnePacket(MSD_DATA_OUT_EP, (uint8_t*)&msd_cbw, MSD_OUT_EP_SIZE);
                }
            }
            break;

        default:
            break;
    }
    return true;
}

/*******************************************************************************
 End of File
*/

