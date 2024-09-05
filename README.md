# GameMaker OpenXInputExtension
An open-source extension to be used with GameMaker to allow for up to 8 XInput devices. This project contains replacement functions for the majority of stock gamepad functions found within gamemaker. Each replacement function is designed to work exactly like the stock GameMaker gamepad functions. (The current release is setup to support 8 controllers, but this can be extended by rebuilding the OpenXInput with a larger number set and then replacing the OpenXInput1_4.dll within this extension with it.)

Important Note: When using this extension, make sure to run your game with the YYC output option selected. VM output will not use the extension's dll properly. (VM will only allow 4 XInput devices. YYC will allow 8 XInput devices.)

## Usage
Import the local package to your project.

On game start, call `gamepad_oxi_init()`.

At the start of each step (Begin Step), call `gamepad_oxi_update()`.

On game end, call `gamepad_oxi_quit()`.

Button Functions: 
- `gamepad_button_check_oxi(deviceIndex, buttonIndex)`
- `gamepad_button_check_pressed_oxi(deviceIndex, buttonIndex)`
- `gamepad_button_check_released_oxi(deviceIndex, buttonIndex)`
- `gamepad_button_value_oxi(deviceIndex, buttonIndex)`

Axis Functions: 
- `gamepad_get_axis_deadzone_oxi(deviceIndex)`
- `gamepad_set_axis_deadzone_oxi(deviceIndex, deadzone)`
- `gamepad_axis_value_oxi(deviceIndex, axisIndex)`
   
Other Functions:
- `gamepad_get_device_count_oxi()`
- `gamepad_is_connected_oxi(deviceIndex)`
- `gamepad_set_vibration_oxi(deviceIndex, leftMotor, rightMotor)`

Parameters for the above functions:
- `deviceIndex`: `0`-`7`
- `axisIndex`: `gp_axislh`, `gp_axislv`, `gp_axisrh`, `gp_axisrv`
- `buttonIndex`: `gp_face1`, `gp_face2`, `gp_face3`, `gp_face4`, `gp_shoulderl`, `gp_shoulderr`, `gp_shoulderlb`, `gp_shoulderrb`, `gp_padu`, `gp_padd`, `gp_padl`, `gp_padr`, `gp_start`, `gp_select`, `gp_stickl`, `gp_stickr`
- `leftMotor`: `0.0`-`1.0`
- `rightMotor`: `0.0`-`1.0`

## Example GameMaker Project
Within the folder OpenXInputExtensionExample, you will find a GameMaker project that uses every function found within this extension. 
When ran, you will see the following:
![Example](https://github.com/andrew171717/OpenXInputExtension/assets/25375491/a532b104-965e-4d05-b421-d8f6a587caea)


## Building Source Instructions
- Step 1: Download the OpenXInput1_4 code from [here](https://github.com/Nemirtingas/OpenXinput/tree/OpenXinput1_4)
- Step 2: Download OpenXInputExtension's code [here](https://github.com/andrew171717/OpenXInputExtension/archive/refs/heads/main.zip).
- Step 3: Move the folder `GMOpenXInputExtension` from OpenXInputExtension into OpenXInput1_4's working directory.
- Step 4: Replace the file `CMakeLists.txt` in OpenXInput1_4's working directory with the version found in OpenXInputExtension.
  - Your build environment should look something like this ![image](https://github.com/andrew171717/OpenXInputExtension/assets/25375491/fd118b47-c2e5-4125-adef-1a0a14f08cd0)
- Step 5: Open the folder `OpenXinput-OpenXinput1_4` in Visual Studio (I used v2022) and wait for Visual Studio to build out the CMake file (you must have CMake support setup in Visual Studio).
- Step 6: You should be able to click build all in Visual Studio to compile the dll.

## More Details
This extension relys on the open-source project OpenXInput which can be found [here](https://github.com/Nemirtingas/OpenXinput/tree/OpenXinput1_4).

There is a build of OpenXinput's dll (OpenXinput1_4.dll, originally named Xinput1_4.dll) within the .yymaps file found within the release of this project. This was done to help those who do not wish to rebuild the dll themselves. However, I do encourage you to download and build [OpenXInput](https://github.com/Nemirtingas/OpenXinput/tree/OpenXinput1_4) if you are familiar with C++ and CMake.














