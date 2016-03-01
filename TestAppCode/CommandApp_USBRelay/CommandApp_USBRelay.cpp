// CommandApp_USBRelay.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>

using namespace std;

/*
使用方法说明：
CommandApp_USBRelay [设备序列号]  [操作类型]  [继电器开关]

设备序列号：每个设备有一个唯一的序列号
操作类型：  open--打开继电器， close-- 关闭继电器
继电器开关：01表示第一个继电器，02表示第二个继电器，... 255表示所有继电器动作

例如：
1. 打开序列号afEd5设备的第一路继电器
CommandApp_USBRelay  afEd5 open 01
2. 打开序列号afEd5设备的所有继电器
CommandApp_USBRelay  afEd5 open 255
3. 关闭序列号afEd5设备的第一路继电器
CommandApp_USBRelay  afEd5 close 01
4. 关闭序列号afEd5设备的所有继电器
CommandApp_USBRelay  afEd5 close 255

命令行工具执行完命令后，如果没有错误，则返回0， 否则返回 1
调用者可以根据该值判断执行是否正确
*/

#pragma comment(lib, "setupapi.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	//argv[1] is the serial number
	if (argc != 4)
	{
		return 1;
	}

	unsigned int relay_index = atoi(argv[3]);
	
	usb_relay_init();

	/*
	struct usb_relay_device_info *device_info = usb_relay_device_enumerate();

	struct usb_relay_device_info *current_info =  device_info;
	struct usb_relay_device_info *find_info = NULL;

	 while (current_info != NULL) 
	{
		if (0 == strcmp(argv[1], (char*)(current_info->serial_number)))
		{
			find_info = current_info;
			break;
		}
		current_info = current_info->next;
	}

	current_info = NULL;

	if (find_info == NULL)
	{
		return 1;
	}

	*/
	//int hd = usb_relay_device_open(find_info);
	int hd = usb_relay_device_open_with_serial_number(argv[1], strlen(argv[1]));

	if (hd == 0)
	{
		return 1;
	}

	int ret = 0;

	if (strcmp(argv[2], "open") == 0)
	{
		if ((relay_index > 0) && (relay_index < 255) )
		{
			ret = usb_relay_device_open_one_relay_channel(hd, relay_index);
		}
		else if (relay_index == 255)
		{
			ret = usb_relay_device_open_all_relay_channel(hd);
		}
		else
		{
			ret =  1;
		}
	}
	else if (strcmp(argv[2], "close") == 0)
	{
		if ((relay_index > 0) && (relay_index < 255) )
		{
			ret = usb_relay_device_close_one_relay_channel(hd, relay_index);
		}
		else if (relay_index == 255)
		{
			ret = usb_relay_device_close_all_relay_channel(hd);
		}
		else
		{
			ret = 1;
		}
	}
	else
	{
		ret = 1;
	}


	//usb_relay_device_free_enumerate(device_info);
	//device_info = NULL;
	usb_relay_exit();

	return ret;
}

