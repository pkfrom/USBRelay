One.command test application: CommandApp_USBRelay.exe £º

usage£ºCommandApp_USBRelay [serial number]  [op_type]  [relay_index]

[parameter]
serial number£ºervery device has a only one serila number. Use the application GuiApp_English.exe to get the serial number.
op_type£º  open--open the relay, close-- close the relay
relay_index: 01 -- relay one£¬02 -- relay two£¬... 255 indicate the all relays.

Example:
1. Open the first relay of the device serial number is afEd5
CommandApp_USBRelay  afEd5 open 01

2. Open  the all relay of the device serial number is afEd5
CommandApp_USBRelay  afEd5 open 255

3. Close the first relay of the device serial number is afEd5
CommandApp_USBRelay  afEd5 close 01

4. Close the all relay of the device serial number is afEd5
CommandApp_USBRelay  afEd5 close 255

If succeed, the command tools will return value 0, else return 1;
The caller can jude the result of execute from the return value.

TWO. GuiApp_English.exe is a test application with GUI.
First, click the "Find Device" button to find the devices that pluged into computer. 
Second, select the device which you want to use from the  drop-down list . 
Third, click the "open device" button to open the device you selected, and then you can do operation. 