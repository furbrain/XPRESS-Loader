 /**
   PWM2 Generated Driver File
 
   @Company
     Microchip Technology Inc.
 
   @File Name
     pwm2.c
 
   @Summary
     This is the generated driver implementation file for the PWM2 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs
 
   @Description
     This source file provides implementations for driver APIs for PWM2.
     Generation Information :
         Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
         Device            :  PIC16F1455
         Driver Version    :  2.01
     The generated drivers are tested against the following:
         Compiler          :  XC8 2.05 and above or later
         MPLAB             :  MPLAB X 5.20
 */ 

 /*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/
 
 /**
   Section: Included Files
 */

 #include <xc.h>
 #include "pwm2.h"

 /**
   Section: PWM Module APIs
 */

 void PWM2_Initialize(void)
 {
    // Set the PWM to the options selected in the PIC10 / PIC12 / PIC16 / PIC18 MCUs.
    // PWM2POL active_hi; PWM2OE enabled; PWM2EN enabled; 
    PWM2CON = 0xC0;   

 }

 void PWM2_LoadDutyValue(uint8_t dutyValue)
 {
     // Writing to 8 MSBs of PWM duty cycle in PWMDCH register
     // This ignores the bottom two bits of PWM2DCH
     PWM2DCH = dutyValue;
     
 }
 
 void PWM2_Off(void) {
     // turn PWM off
     PWM2CON = 0x00;
 }
 /**
  End of File
 */
