#include "hal.h"

#include "usb.h"

SerialUSBDriver SDU1;

#define USBD1_DATA_REQUEST_EP           1
#define USBD1_DATA_AVAILABLE_EP         1
#define USBD1_INTERRUPT_REQUEST_EP      2

static const uint8_t usbDeviceDescriptorData[18] = {
    USB_DESC_DEVICE (
        0x0110,        /* bcdUSB (1.1).                    */
        0x02,          /* bDeviceClass (CDC).              */
        0x00,          /* bDeviceSubClass.                 */
        0x00,          /* bDeviceProtocol.                 */
        0x40,          /* bMaxPacketSize.                  */
        0x1209,        /* idVendor (ST).                   */
        0x0134,        /* idProduct.                       */
        0x0200,        /* bcdDevice.                       */
        1,             /* iManufacturer.                   */
        2,             /* iProduct.                        */
        3,             /* iSerialNumber.                   */
        1              /* bNumConfigurations.              */
    )
};

static const USBDescriptor usbDeviceDescriptor = {
    sizeof usbDeviceDescriptorData,
    usbDeviceDescriptorData
};

static const uint8_t usbConfigurationDescriptorData[67] = {
    /* Configuration Descriptor.*/
    USB_DESC_CONFIGURATION (
        67,                               /* wTotalLength.                    */
        0x02,                             /* bNumInterfaces.                  */
        0x01,                             /* bConfigurationValue.             */
        0,                                /* iConfiguration.                  */
        0xC0,                             /* bmAttributes (self powered).     */
        50                                /* bMaxPower (100mA).               */
    ),
    /* Interface Descriptor.*/
    USB_DESC_INTERFACE (
        0x00,                             /* bInterfaceNumber.                */
        0x00,                             /* bAlternateSetting.               */
        0x01,                             /* bNumEndpoints.                   */
        0x02,                             /* bInterfaceClass (Communications
                                             Interface Class, CDC section
                                             4.2).                            */
        0x02,                             /* bInterfaceSubClass (Abstract
                                             Control Model, CDC section 4.3). */
        0x01,                             /* bInterfaceProtocol (AT commands,
                                             CDC section 4.4).                */
        0                                 /* iInterface.                      */
    ),
    /* Header Functional Descriptor (CDC section 5.2.3).*/
    USB_DESC_BYTE         (5),            /* bLength.                         */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x00),         /* bDescriptorSubtype (Header
                                             Functional Descriptor.           */
    USB_DESC_BCD          (0x0110),       /* bcdCDC.                          */
    /* Call Management Functional Descriptor. */
    USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x01),         /* bDescriptorSubtype (Call Management
                                             Functional Descriptor).          */
    USB_DESC_BYTE         (0x00),         /* bmCapabilities (D0+D1).          */
    USB_DESC_BYTE         (0x01),         /* bDataInterface.                  */
    /* ACM Functional Descriptor.*/
    USB_DESC_BYTE         (4),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x02),         /* bDescriptorSubtype (Abstract
                                             Control Management Descriptor).  */
    USB_DESC_BYTE         (0x02),         /* bmCapabilities.                  */
    /* Union Functional Descriptor.*/
    USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
    USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
    USB_DESC_BYTE         (0x06),         /* bDescriptorSubtype (Union
                                             Functional Descriptor).          */
    USB_DESC_BYTE         (0x00),         /* bMasterInterface (Communication
                                             Class Interface).                */
    USB_DESC_BYTE         (0x01),         /* bSlaveInterface0 (Data Class
                                             Interface).                      */
    /* Endpoint 2 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD1_INTERRUPT_REQUEST_EP|0x80,
                           0x03,          /* bmAttributes (Interrupt).        */
                           0x0008,        /* wMaxPacketSize.                  */
                           0xFF),         /* bInterval.                       */
    /* Interface Descriptor.*/
    USB_DESC_INTERFACE    (0x01,          /* bInterfaceNumber.                */
                           0x00,          /* bAlternateSetting.               */
                           0x02,          /* bNumEndpoints.                   */
                           0x0A,          /* bInterfaceClass (Data Class
                                           Interface, CDC section 4.5).       */
                           0x00,          /* bInterfaceSubClass (CDC section
                                           4.6).                              */
                           0x00,          /* bInterfaceProtocol (CDC section
                                           4.7).                              */
                           0x00),         /* iInterface.                      */
    /* Endpoint 3 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD1_DATA_AVAILABLE_EP,       /* bEndpointAddress.*/
                           0x02,          /* bmAttributes (Bulk).             */
                           0x0040,        /* wMaxPacketSize.                  */
                           0x00),         /* bInterval.                       */
    /* Endpoint 1 Descriptor.*/
    USB_DESC_ENDPOINT     (USBD1_DATA_REQUEST_EP|0x80,    /* bEndpointAddress.*/
                           0x02,          /* bmAttributes (Bulk).             */
                           0x0040,        /* wMaxPacketSize.                  */
                           0x00)          /* bInterval.                       */
};

static const USBDescriptor usbConfigurationDescriptor = {
    sizeof usbConfigurationDescriptorData,
    usbConfigurationDescriptorData
};

static const uint8_t usbString0[] = {
    USB_DESC_BYTE(4),                     /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

static const uint8_t usbString1[] = {
    USB_DESC_BYTE(22),                    /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    'O', 0, 'p', 0, 'e', 0, 'n', 0, 'L', 0, 'o', 0, 'W', 0, 'P', 0,
    'A', 0, 'N', 0,
};

static const uint8_t usbString2[] = {
    USB_DESC_BYTE(16),                    /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    'S', 0, 'n', 0, 'i', 0, 'f', 0, 'f', 0, 'e', 0, 'r', 0
};

static const uint8_t usbString3[] = {
    USB_DESC_BYTE(8),                     /* bLength.                         */
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
    '0' + CH_KERNEL_MAJOR, 0,
    '0' + CH_KERNEL_MINOR, 0,
    '0' + CH_KERNEL_PATCH, 0
};

static const USBDescriptor usbStrings[] = {
    {sizeof usbString0, usbString0},
    {sizeof usbString1, usbString1},
    {sizeof usbString2, usbString2},
    {sizeof usbString3, usbString3}
};

static const USBDescriptor *usbGetDescriptor(USBDriver *usbp,
                                             uint8_t dtype,
                                             uint8_t dindex,
                                             uint16_t lang) 
{
    (void)usbp;
    (void)lang;
    switch (dtype) 
    {
    case USB_DESCRIPTOR_DEVICE:
        return &usbDeviceDescriptor;
    case USB_DESCRIPTOR_CONFIGURATION:
        return &usbConfigurationDescriptor;
    case USB_DESCRIPTOR_STRING:
        if (dindex < 4)
            return &usbStrings[dindex];
    }
    return NULL;
}

static USBInEndpointState  usbEP1InState;
static USBOutEndpointState usbEP1OutState;

static const USBEndpointConfig usbEP1Config = {
    USB_EP_MODE_TYPE_BULK,
    NULL,
    sduDataTransmitted,
    sduDataReceived,
    0x0040,
    0x0040,
    &usbEP1InState,
    &usbEP1OutState,
    1,
    NULL
};

static USBInEndpointState usbEP2InState;

static const USBEndpointConfig usbEP2Config = {
    USB_EP_MODE_TYPE_INTR,
    NULL,
    sduInterruptTransmitted,
    NULL,
    0x0010,
    0x0000,
    &usbEP2InState,
    NULL,
    1,
    NULL
};

static void usbEvent(USBDriver *usbp, usbevent_t event)
{
    extern SerialUSBDriver SDU1;

    usbUserEvent(usbp, event);

    switch (event)
    {
    case USB_EVENT_ADDRESS:
        return;

    case USB_EVENT_CONFIGURED:
        chSysLockFromISR();
        usbInitEndpointI(usbp, USBD1_DATA_REQUEST_EP, &usbEP1Config);
        usbInitEndpointI(usbp, USBD1_INTERRUPT_REQUEST_EP, &usbEP2Config);
        sduConfigureHookI(&SDU1);
        chSysUnlockFromISR();
        return;

    case USB_EVENT_RESET:
    case USB_EVENT_UNCONFIGURED:
    case USB_EVENT_SUSPEND:
        chSysLockFromISR();
        sduSuspendHookI(&SDU1);
        chSysUnlockFromISR();
        return;

    case USB_EVENT_WAKEUP:
        chSysLockFromISR();
        sduWakeupHookI(&SDU1);
        chSysUnlockFromISR();
        return;

    case USB_EVENT_STALLED:
        return;
    }
    return;
}

static void usbSOF(USBDriver *usbp) 
{
    (void)usbp;

    osalSysLockFromISR();
    sduSOFHookI(&SDU1);
    osalSysUnlockFromISR();
}

const USBConfig usbConfig = {
    usbEvent,
    usbGetDescriptor,
    sduRequestsHook,
    usbSOF
};

const SerialUSBConfig serialUSBConfig = {
    &USBD1,
    USBD1_DATA_REQUEST_EP,
    USBD1_DATA_AVAILABLE_EP,
    USBD1_INTERRUPT_REQUEST_EP
};
