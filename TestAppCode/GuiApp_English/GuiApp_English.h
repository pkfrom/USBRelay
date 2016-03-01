#pragma once

#include "resource.h"

#include "GuiApp_English.h"

struct usb_relay_device_info;


class CUSBRelayWnd : public CXEventMsg
{
public:
	CUSBRelayWnd():m_pDeviceList(NULL), m_hCurDevice(0), m_bOpened(FALSE), m_hCureDeviceIndex(0)
	{

	}
	BOOL Create();

	//finde device button click event
	BOOL OnFindDeviceBtnClick(HELE hEle,HELE hEleEvent);
	//open device button click event
	BOOL OnOpenDeviceBtnClick(HELE hEle,HELE hEleEvent);
	//close device button click event
	BOOL OnCloseDeviceBtnClick(HELE hEle,HELE hEleEvent);

	//open one relay button click event 
	BOOL OnOpenRelayBtnClick(HELE hEle, HELE hEleEvent);
	//close one relay button click event 
	BOOL OnCloseRelayBtnClick(HELE hEle, HELE hEleEvent);
	//open all relay button click event 
	BOOL OnOpenAllRelayBtnClick(HELE hEle, HELE hEleEvent);
	//close all relay button click event 
	BOOL OnCloseAllRelayBtnClick(HELE hEle, HELE hEleEvent);



public:
	HWINDOW m_hWindow; //windows handle

	HELE    m_hFindDeviceBtn; //查找设备句柄
	HELE	m_hDeviceListBox; //设备列表框

	HELE    m_hOpenDeviceBtn;
	HELE	m_hOpenStatus;
	BOOL    m_bOpened;
	HELE	m_hCloseDeviceBtn;

	HELE    m_hRelayText[8];
	HELE	m_hRelayOpenBtn[8];
	HELE	m_hRelayCloseBtn[8];
	HELE	m_hRelayStatusLabel[8];

	HELE	m_hOpenAllRelayBtn;
	HELE	m_hCloseAllRelayBtn;

	/*****************************/
	struct usb_relay_device_info *m_pDeviceList;
	int m_hCurDevice;
	int m_hCureDeviceIndex;

	std::map<std::wstring, struct usb_relay_device_info*> m_mapIndexToDevice;
};