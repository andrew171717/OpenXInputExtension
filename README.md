# GameMaker OpenXInputExtension
An open-source extension to be used with GameMaker to allow for more than 4 XInput devices. The current version of this project is limited on what it can do, but I plan to flesh it out over the next coming weeks. I have tested this extension with 6 XInput controllers, but it should work with up to 8.

## Usage
Import the local package.

On game start, call `gamepad_oxi_init()`.

During gameplay call `gamepad_button_check_oxi(deviceIndex, buttonIndex)`.
- `deviceIndex`: `0-7` (the current release it setup to support 8 controllers, but this can be extended by rebuilding the [OpenXInput](https://github.com/Nemirtingas/OpenXinput/tree/OpenXinput1_4) dll with a larger number set.)
- `buttonIndex`: Current button supports: `gp_face1`, `gp_face2`, `gp_face3`, `gp_face4`, `gp_shoulderl`, `gp_shoulderr`, `gp_shoulderlb`, `gp_shoulderrb`, `gp_padu`, `gp_padd`, `gp_padl`, `gp_padr`, `gp_start`, `gp_select`

On game end, call `gamepad_oxi_quit()`.

## Building Instructions
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














