# GameMaker OpenXInputExtension
An open-source extension to be used with GameMaker to allow for more than 4 XInput devices.

## Usage
Import the local package.

On game start, call gamepad_oxi_init().

During gameplay call gamepad_button_check_oxi(deviceIndex, buttonIndex).
- deviceIndex: 0-7 (the current release it setup to support 8 controllers, but this can be extended by rebuilding the OpenXInput with a larger number set.)
- buttonIndex: Currently supports: gp_face1, gp_face2, gp_face3, gp_face4, gp_shoulderl, gp_shoulderr, gp_shoulderlb, gp_shoulderrb, gp_padu, gp_padd, gp_padl, gp_padr, gp_start, gp_select

On game end, call gamepad_oxi_quit().

## More Details
This extension relys on the open-source project OpenXInput which can be found [here](https://github.com/Nemirtingas/OpenXinput/tree/OpenXinput1_4).

There is a build of that project's dll (OpenXinput1_4.dll, originally named Xinput1_4.dll) within the .yymaps file found within this project. This was done to help those who do not wish to rebuild the dll themselves. However, I do encourage you to download and build [OpenXInput](https://github.com/Nemirtingas/OpenXinput/tree/OpenXinput1_4) if you are familiar with C++.














