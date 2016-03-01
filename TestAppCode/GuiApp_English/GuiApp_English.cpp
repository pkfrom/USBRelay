// GuiApp.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GuiApp_English.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine, int nCmdShow)
{
	XInitXCGUI(); //initial

	CUSBRelayWnd  MyWnd;
	if(MyWnd.Create())
	{
		XRunXCGUI(); //run gui
	}
	return 0;
}

//create main window
BOOL CUSBRelayWnd::Create()
{
	m_hWindow = XWnd_CreateWindow(0, 0, 290, 430, L"USB Relay Manager"); //main window
	if(m_hWindow)
	{
		XWnd_EnableDragBorder(m_hWindow, FALSE);
		XWnd_EnableMaxButton(m_hWindow, FALSE);

		//1. create find device button
		m_hFindDeviceBtn = XBtn_Create(10, 10, 90, 25, L"Find device", m_hWindow); 
		//register the button click event
		XCGUI_RegEleEvent(m_hFindDeviceBtn, XE_BNCLICK, &CUSBRelayWnd::OnFindDeviceBtnClick); 

		//2. create the device list combobox
		m_hDeviceListBox = XComboBox_Create(110, 10, 150, 25, m_hWindow);

		//3. create open device button
		m_hOpenDeviceBtn = XBtn_Create(10, 45, 90, 25, L"open device", m_hWindow);
		//register the button click event
		XCGUI_RegEleEvent(m_hOpenDeviceBtn, XE_BNCLICK, &CUSBRelayWnd::OnOpenDeviceBtnClick);

		//4. create open status label
		m_hOpenStatus = XEle_Create(110, 45, 50, 25, m_hWindow);
		XEle_SetBkColor(m_hOpenStatus, RGB(255, 0, 0));

		//5. create close device button
		m_hOpenDeviceBtn = XBtn_Create(170, 45, 90, 25, L"close device", m_hWindow);
		//disable before open a device

		//register the button click event
		XCGUI_RegEleEvent(m_hOpenDeviceBtn, XE_BNCLICK, &CUSBRelayWnd::OnCloseDeviceBtnClick);

		int pos_text_x = 10;
		int pos_open_btn_x = 105;
		int pos_status_label_x = 170;
		int pos_close_btn_x = 200;

		int pos_y = 80;

		//6. create the buttons and lables for all relay
		for (int i = 0; i < 8; i++)
		{
			wchar_t relayName[20] = {0};
			wsprintf(relayName, L"Realy %d", i+1);

			m_hRelayText[i] = XEdit_Create(pos_text_x, pos_y, 90, 25, m_hWindow);
			XEdit_SetText(m_hRelayText[i], relayName);

			m_hRelayOpenBtn[i] = XBtn_Create(pos_open_btn_x, pos_y, 60, 25, L"open", m_hWindow);
			XCGUI_RegEleEvent(m_hRelayOpenBtn[i], XE_BNCLICK, &CUSBRelayWnd::OnOpenRelayBtnClick);

			m_hRelayStatusLabel[i] = XEle_Create(pos_status_label_x, pos_y, 25, 25, m_hWindow);
			XEle_SetBkColor(m_hRelayStatusLabel[i], RGB(255, 0, 0));

			m_hRelayCloseBtn[i] = XBtn_Create(pos_close_btn_x, pos_y, 60, 25, L"close", m_hWindow);
			XCGUI_RegEleEvent(m_hRelayCloseBtn[i], XE_BNCLICK, &CUSBRelayWnd::OnCloseRelayBtnClick);

			pos_y += 30;
		}

		//7. create open-all relay button
		m_hOpenAllRelayBtn  = XBtn_Create(10, pos_y+10, 120, 50, L"Open All", m_hWindow);
		XCGUI_RegEleEvent(m_hOpenAllRelayBtn, XE_BNCLICK, &CUSBRelayWnd::OnOpenAllRelayBtnClick);

		//8. create close-all relay button
		m_hCloseAllRelayBtn = XBtn_Create(140, pos_y+10, 120, 50, L"Close All", m_hWindow);
		XCGUI_RegEleEvent(m_hCloseAllRelayBtn, XE_BNCLICK, &CUSBRelayWnd::OnCloseAllRelayBtnClick);

		//initial usb_relay_device lib
		usb_relay_init();

		XWnd_ShowWindow(m_hWindow, SW_SHOW); //显示窗口
		return true;
	}
	return false;
}

BOOL CUSBRelayWnd::OnFindDeviceBtnClick( HELE hEle,HELE hEleEvent )
{
	if (m_bOpened)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"Close Current Device First",  L"Attention", XMB_OK);
		return TRUE;
	}

	if (m_pDeviceList)
	{
		usb_relay_device_free_enumerate(m_pDeviceList);
		m_pDeviceList = NULL;
	}

	m_pDeviceList = usb_relay_device_enumerate();
	if (m_pDeviceList)
	{
		struct usb_relay_device_info *cur_device = m_pDeviceList;
		while (cur_device)
		{
			XComboBox_AddString( m_hDeviceListBox, (wchar_t*)(Ansi2WChar((char*)(cur_device->serial_number)).c_str()) );
			m_mapIndexToDevice[Ansi2WChar((char*)(cur_device->serial_number))] = cur_device;

			cur_device = cur_device->next;
		}
	}

	return TRUE;
}

BOOL CUSBRelayWnd::OnOpenDeviceBtnClick( HELE hEle,HELE hEleEvent )
{
	if (m_bOpened)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"Close Current Device First",  L"Attention", XMB_OK);
		return TRUE;
	}

	int index = XComboBox_GetSelectItem(m_hDeviceListBox);
	if (index > -1)
	{
		wchar_t *serial = XComboBox_GetItemText(m_hDeviceListBox, index);
		std::map<wstring ,struct usb_relay_device_info*>::const_iterator fd = m_mapIndexToDevice.find(wstring(serial));
		if (fd != m_mapIndexToDevice.end())
		{
			//1. open usb relay device
			m_hCurDevice = usb_relay_device_open(fd->second);
			if (m_hCurDevice)
			{
				//2. Get status
				unsigned int status = 0;
				if (0 == usb_relay_device_get_status(m_hCurDevice, &status))
				{
					for (int i = 0; i < fd->second->type; i++)
					{
						if ((1<<i) & status)
						{
							XEle_SetBkColor(m_hRelayStatusLabel[i], RGB(0, 255, 0));
							XEle_RedrawEle(m_hRelayStatusLabel[i], TRUE);
						}
					}
					XEle_SetBkColor(m_hOpenStatus, RGB(0, 255, 0));
					XEle_RedrawEle(m_hOpenStatus, TRUE);
					m_bOpened = TRUE;
					m_hCureDeviceIndex = fd->second->type;
				}
				else
				{
					usb_relay_device_close(m_hCurDevice);
					m_hCurDevice = 0;
					XMessageBox(XEle_GetHWindow(hEle), L"Open Device Error!!", L"Error", XMB_OK);
				}

			}
			else
			{
				XMessageBox(XEle_GetHWindow(hEle), L"Open Device Error!!", L"Error", XMB_OK);
			}
		}
		else
		{
			XMessageBox(XEle_GetHWindow(hEle), L"The Device is not valid", L"Error", XMB_OK);
		}
	}
	else
	{
		XMessageBox(XEle_GetHWindow(hEle), L"Select the device first",  L"Error", XMB_OK);
	}

	return TRUE;
}

BOOL CUSBRelayWnd::OnCloseDeviceBtnClick( HELE hEle,HELE hEleEvent )
{
	if (!m_bOpened)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"Open Device First",  L"Error", XMB_OK);
		return TRUE;
	}

	usb_relay_device_close(m_hCurDevice);
	m_hCurDevice = 0;
	m_bOpened = FALSE;
	XEle_SetBkColor(m_hOpenStatus, RGB(255, 0, 0));
	XEle_RedrawEle(m_hOpenStatus, TRUE);

	for (int i = 0; i < m_hCureDeviceIndex; i++)
	{
		XEle_SetBkColor(m_hRelayStatusLabel[i], RGB(255, 0, 0));
		XEle_RedrawEle(m_hRelayStatusLabel[i], TRUE);
	}

	return TRUE;
}

BOOL CUSBRelayWnd::OnOpenRelayBtnClick( HELE hEle, HELE hEleEvent )
{
	if (!m_bOpened)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"Open Device First",  L"Error", XMB_OK);
		return TRUE;
	}
	for (int i = 0; i < 8; i++)
	{
		if (hEle == m_hRelayOpenBtn[i])
		{
			int ret = usb_relay_device_open_one_relay_channel(m_hCurDevice, i+1);
			if (ret == 0)
			{
				XEle_SetBkColor(m_hRelayStatusLabel[i], RGB(0, 255, 0));
				XEle_RedrawEle(m_hRelayStatusLabel[i],TRUE);
			}
		}
	}
	return TRUE;
}

BOOL CUSBRelayWnd::OnCloseRelayBtnClick( HELE hEle, HELE hEleEvent )
{
	if (!m_bOpened)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"Open Device First",  L"Error", XMB_OK);
		return TRUE;
	}
	for (int i = 0; i < 8; i++)
	{
		if (hEle == m_hRelayCloseBtn[i])
		{
			int ret =  usb_relay_device_close_one_relay_channel(m_hCurDevice, i+1);
			if (ret == 0)
			{
				XEle_SetBkColor(m_hRelayStatusLabel[i], RGB(255, 0, 0));
				XEle_RedrawEle(m_hRelayStatusLabel[i],TRUE);
			}
		}
	}
	return TRUE;
}

BOOL CUSBRelayWnd::OnOpenAllRelayBtnClick( HELE hEle, HELE hEleEvent )
{
	if (!m_bOpened)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"Open Device First",  L"Error", XMB_OK);
		return TRUE;
	}
	int ret = usb_relay_device_open_all_relay_channel(m_hCurDevice);
	if (ret == 0)
	{
		for(int i = 0; i < m_hCureDeviceIndex; i++)
		{
			XEle_SetBkColor(m_hRelayStatusLabel[i], RGB(0, 255, 0));
			XEle_RedrawEle(m_hRelayStatusLabel[i], TRUE);
		}
	}
	return TRUE;
}

BOOL CUSBRelayWnd::OnCloseAllRelayBtnClick( HELE hEle, HELE hEleEvent )
{
	if (!m_bOpened)
	{
		XMessageBox(XEle_GetHWindow(hEle), L"Open Device First",  L"Error", XMB_OK);
		return TRUE;
	}
	int ret = usb_relay_device_close_all_relay_channel(m_hCurDevice);
	if (ret == 0)
	{
		for(int i = 0; i < m_hCureDeviceIndex; i++)
		{
			XEle_SetBkColor(m_hRelayStatusLabel[i], RGB(255, 0, 0));
			XEle_RedrawEle(m_hRelayStatusLabel[i], TRUE);
		}
	}
	return TRUE;
}