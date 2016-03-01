## Command
test application:
`CommandApp_USBRelay.exe`

- usage
 - `CommandApp_USBRelay [serial_number]  [op_type]  [relay_index]`

- parameter
 - `[serial_number]` every device has a only one serial number. Use the application `GuiApp_English.exe` to get the serial number.
 - `[op_type]`  *`open`* --open the relay, *`close`* --close the relay
 - `[relay_index]`: *`01`* relay one, *`02`* relay two, ... *`255`* indicate the all relays.


### Example:
1. Open the first relay of the device serial number is ESWED

```
CommandApp_USBRelay ESWED open 01
```

2. Open  the all relay of the device serial number is ESWED

```
CommandApp_USBRelay ESWED open 255
```

3. Close the first relay of the device serial number is ESWED

```
CommandApp_USBRelay ESWED close 01
```

4. Close the all relay of the device serial number is ESWED

```
CommandApp_USBRelay ESWED close 255
```

If succeed, the command tools will return value **0**, else return **1**;
The caller can just the result of execute from the return value.


## GuiApp_English.exe is a test application with GUI.

- First, click the _**"Find Device "**_ button to find the devices that plugged into computer. 
- Second, select the device which you want to use from the  drop-down list . 
- Third, click the _**"open device "**_ button to open the device you selected, and then you can do operation. 