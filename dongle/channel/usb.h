#ifndef SNIFFER_USB_H
#define SNIFFER_USB_H

extern const USBConfig usbConfig;
extern const SerialUSBConfig serialUSBConfig;
extern SerialUSBDriver SDU1;

void usbUserEvent(USBDriver *usbp, usbevent_t event);

#endif  /* SNIFFER_USB_H */
