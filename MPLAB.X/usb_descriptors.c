/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

 USB Descriptors
 
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
 
/*********************************************************************
 * Descriptor specific type definitions are defined in:
 * usb_device.h
 *
 * Configuration options are defined in:
 * usb_config.h
 ********************************************************************/
#ifndef __USB_DESCRIPTORS_C
#define __USB_DESCRIPTORS_C

/** INCLUDES *******************************************************/
#include "usb.h"
#include "usb_device_msd.h"

/** CONSTANTS ******************************************************/

/* Device Descriptor */
const USB_DEVICE_DESCRIPTOR device_dsc=
{
    0x12,    // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,                // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // See interface for class codes
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_config.h
    0x04D8,                 // Vendor ID  "Microchip Technology Inc"
    0x0009,                 // MSD device
    0x0001,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x03,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
const uint8_t configDescriptor1[]={
    /* Configuration Descriptor */
    9,    // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                // CONFIGURATION descriptor type
    32, 0,                  // Total length of data for this cfg
    1,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    2,                      // Configuration string index
    _DEFAULT | _SELF,       // Attributes, see usb_device.h
    50,                     // Max power consumption (2X mA)

//---------------MSD Function 1 Descriptors------------------------
    /* Interface Descriptor */
    9,   // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
    MSD_INTF_ID,            // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this intf
    MSD_INTF,               // Class code
    MSD_INTF_SUBCLASS,      // Subclass code
    MSD_PROTOCOL, 		    // Protocol code
    0,                      // Interface string index
    
    /* Endpoint Descriptor */
    7,
    USB_DESCRIPTOR_ENDPOINT,
    _EP01_IN,_BULK,
    MSD_IN_EP_SIZE,0x00,
    0x01,
    
    /* Endpoint Descriptor */
    7,
    USB_DESCRIPTOR_ENDPOINT,
    _EP01_OUT,
    _BULK,
    MSD_OUT_EP_SIZE,0x00,
    0x01,    
};


//Language code(s) supported string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[1];}sd000={
    sizeof(sd000),
    USB_DESCRIPTOR_STRING,
    {0x0409} //0x0409 = Language ID code for US English
};
//Manufacturer string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[15];}sd001={
sizeof(sd001),USB_DESCRIPTOR_STRING,
{'L','o','r','r','a','i','n','b','o','w',' ','T','e','c','h'}
};

//Product string descriptor
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[5];}sd002={
sizeof(sd002),USB_DESCRIPTOR_STRING,
{'S','o','l','a','s'}
};

//Serial number string descriptor.  Note: This should be unique for each unit 
//built on the assembly line.  Plugging in two units simultaneously with the 
//same serial number into a single machine can cause problems.  Additionally, not 
//all hosts support all character values in the serial number string.  The MSD 
//Bulk Only Transport (BOT) specs v1.0 restrict the serial number to consist only
//of ASCII characters "0" through "9" and capital letters "A" through "F".
const struct{uint8_t bLength;uint8_t bDscType;uint16_t string[6];}sd003={
sizeof(sd003),USB_DESCRIPTOR_STRING,
{'1','2','3','4','5','6'}};


//Array of configuration descriptors
const uint8_t *const USB_CD_Ptr[]=
{
    (const uint8_t *const)&configDescriptor1
};

//Array of string descriptors
const uint8_t *const USB_SD_Ptr[]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002,
    (const uint8_t *const)&sd003
};

/** EOF usb_descriptors.c ***************************************************/

#endif
