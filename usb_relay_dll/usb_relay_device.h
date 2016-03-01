#ifndef USB_RELAY_DEVICE_H__
#define USB_RELAY_DEVICE_H__

#pragma comment(lib, "setupapi.lib")

#ifdef __cplusplus
extern "C" {
#endif

	//#pragma comment(lib, "setupapi.lib")

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API 
#endif

	enum usb_relay_device_type
	{
		USB_RELAY_DEVICE_ONE_CHANNEL = 1,
		USB_RELAY_DEVICE_TWO_CHANNEL = 2,
		USB_RELAY_DEVICE_FOUR_CHANNEL = 4,
		USB_RELAY_DEVICE_EIGHT_CHANNEL = 8	
	};

	/*usb relay board info structure*/
	struct usb_relay_device_info
	{
		unsigned char *serial_number;
		char *device_path;
		usb_relay_device_type type;
		usb_relay_device_info* next;
	};

	/*init the USB Relay Libary
	@returns: This function returns 0 on success and -1 on error.
	*/
	int EXPORT_API usb_relay_init(void);
	
	/*Finalize the USB Relay Libary.
	This function frees all of the static data associated with
	USB Relay Libary. It should be called at the end of execution to avoid
	memory leaks.
	@returns:This function returns 0 on success and -1 on error.
	*/
	int EXPORT_API usb_relay_exit(void);


	/*Enumerate the USB Relay Devices.*/
	struct usb_relay_device_info EXPORT_API * usb_relay_device_enumerate(void);


	/*Free an enumeration Linked List*/
	void EXPORT_API usb_relay_device_free_enumerate(struct usb_relay_device_info*);
	
	/*open device that serial number is serial_number*/
	/*@return: This funcation returns a valid handle to the device on success or NULL on failure.*/
	/*e.g: usb_relay_device_open_with_serial_number("abcde", 5")*/
	int EXPORT_API usb_relay_device_open_with_serial_number(const char *serial_number, unsigned len);

	/*open a usb relay device
	@return: This funcation returns a valid handle to the device on success or NULL on failure.
	*/
	int EXPORT_API  usb_relay_device_open(struct usb_relay_device_info* device_info);

	/*close a usb relay device*/
	void EXPORT_API usb_relay_device_close(int hHandle);

	/*open a relay channel on the USB-Relay-Device
	@paramter: index -- which channel your want to open
	hHandle -- which usb relay device your want to operate
	@returns: 0 -- success; 1 -- error; 2 -- index is outnumber the number of the usb relay device
	*/
	int EXPORT_API usb_relay_device_open_one_relay_channel(int hHandle, int index);

	/*open all relay channel on the USB-Relay-Device
	@paramter: hHandle -- which usb relay device your want to operate
	@returns: 0 -- success; 1 -- error
	*/
	int EXPORT_API usb_relay_device_open_all_relay_channel(int hHandle);

	/*close a relay channel on the USB-Relay-Device
	@paramter: index -- which channel your want to close
	hHandle -- which usb relay device your want to operate
	@returns: 0 -- success; 1 -- error; 2 -- index is outnumber the number of the usb relay device
	*/
	int EXPORT_API usb_relay_device_close_one_relay_channel(int hHandle, int index);

	/*close all relay channel on the USB-Relay-Device
	@paramter: hHandle -- which usb relay device your want to operate
	@returns: 0 -- success; 1 -- error
	*/
	int EXPORT_API usb_relay_device_close_all_relay_channel(int hHandle);

	/*
	status bit: High --> Low 0000 0000 0000 0000 0000 0000 0000 0000, one bit indicate a relay status.
	the lowest bit 0 indicate relay one status, 1 -- means open status, 0 -- means closed status.
	bit 0/1/2/3/4/5/6/7/8 indicate relay 1/2/3/4/5/6/7/8 status
	@returns: 0 -- success; 1 -- error
	*/
	int EXPORT_API usb_relay_device_get_status(int hHandle, unsigned int *status);

#ifndef EXPORT_DLL
	int EXPORT_API usb_relay_device_set_serial(int hHandle, char serial[5]);
#endif

#ifdef __cplusplus
}
#endif 

#endif //end of ifdef __cplusplus