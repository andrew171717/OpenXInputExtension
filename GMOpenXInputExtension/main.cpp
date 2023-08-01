/* Copyright (C) Drewworks
 * This file is part of the GameMaker OpenXinputExtension project.
 *
 * GameMaker OpenXinputExtension project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GameMaker OpenXinputExtension project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXinputExtension project.  If not, see <https://www.gnu.org/licenses/>
 */

#include <Windows.h>
#include <OpenXinput.h>

#include <thread>
#include <iostream>

struct XInputDevice_t
{
    bool connected;
    DWORD deviceIndex;
    XINPUT_CAPABILITIES_EX capabilities;
    XINPUT_BATTERY_INFORMATION battery;
    OPENXINPUT_STATE_FULL oldState;
    OPENXINPUT_STATE_FULL state;
    WORD vendorId;
    WORD productId;
    WORD inputId;
    double deadzone;
};

static DWORD XinputMaxControllerCount = XUSER_MAX_COUNT;
static OpenXInputGetDeviceUSBIds_t* pOpenXInputGetDeviceUSBIds = nullptr;
static OpenXInputGetStateFull_t* pOpenXInputGetStateFull = nullptr;

void OnDeviceInfoChange(XInputDevice_t& device)
{
    memcpy(&device.oldState, &device.state, sizeof(device.state));
}

void OnDeviceConnect(XInputDevice_t& device)
{
    device.connected = true;
    memset(&device.battery, 0, sizeof(device.battery));
    memset(&device.capabilities, 0, sizeof(device.capabilities));
    memset(&device.oldState, 0, sizeof(device.oldState));
    memset(&device.state, 0, sizeof(device.state));

    OpenXInputGetCapabilitiesEx(1, device.deviceIndex, XINPUT_FLAG_GAMEPAD, &device.capabilities);

    if (pOpenXInputGetDeviceUSBIds != nullptr)
    {
        pOpenXInputGetDeviceUSBIds(device.deviceIndex, &device.vendorId, &device.productId, &device.inputId);
    }
    else
    {
        device.vendorId = device.capabilities.VendorId;
        device.productId = device.capabilities.ProductId;
        device.inputId = 0xffff;
    }

    OnDeviceInfoChange(device);
}

void OnDeviceDisconnect(XInputDevice_t& device)
{
    device.connected = false;
    memset(&device, 0, sizeof(device));
    
    OnDeviceInfoChange(device);
}

bool DeviceStateChanged(XInputDevice_t& device)
{
    return memcmp(&device.oldState.XinputState.Gamepad, &device.state.XinputState.Gamepad, sizeof(device.state.XinputState.Gamepad)) != 0 ||
        memcmp(&device.oldState.GamepadExtras, &device.state.GamepadExtras, sizeof(device.state.GamepadExtras)) != 0;
}

HMODULE LoadDllByName(const std::wstring& dllName) {
    // Buffer to store the full path of the executable
    wchar_t exeFullPath[MAX_PATH];
    DWORD pathLength = GetModuleFileNameW(NULL, exeFullPath, MAX_PATH);

    if (pathLength == 0) {
        return nullptr;
    }

    // Find the last backslash in the full path to extract the directory
    wchar_t* lastBackslash = wcsrchr(exeFullPath, L'\\');
    if (!lastBackslash) {
        return nullptr;
    }

    // Replace the filename in the path with the DLL name
    *(lastBackslash + 1) = L'\0'; // Null-terminate after the last backslash
    std::wstring dllPath = exeFullPath;
    dllPath += dllName;

    // Load the DLL
    HMODULE hModule = LoadLibraryW(dllPath.c_str());
    if (!hModule) {
        DWORD error = GetLastError();
    }

    return hModule;
}

#pragma region Gamemaker_Extension_Code
#define GAMEPAD_DEADZONE_TRIGGER 150
#define GAMEPAD_STICK_MAX 32768
enum GameMakerGamepadButtonIDs
{
    gp_face1 = 32769, // A
    gp_face2 = 32770, // B
    gp_face3 = 32771, // Y
    gp_face4 = 32772, // X
    gp_shoulderl = 32773, // Left Bumper
    gp_shoulderr = 32774, // Right Bumper
    gp_shoulderlb = 32775, // Left Trigger
    gp_shoulderrb = 32776, // Right Trigger
    gp_select = 32777, // Select (left menu button)
    gp_start = 32778, // Start (right menu button)
    gp_stickl = 32779, // Left Thumb Stick
    gp_stickr = 32780, // Right Thumb Stick
    gp_padu = 32781, // Up
    gp_padd = 32782, // Down
    gp_padl = 32783, // Left
    gp_padr = 32784, // Right
    gp_axislh = 32785, // Analog Stick Left Horizontal
    gp_axislv = 32786, // Analog Stick Left Vertical
    gp_axisrh = 32787, // Analog Stick Right Horizontal
    gp_axisrv = 32788  // Analog Stick Right Vertical
};

#define fn_export extern "C" __declspec (dllexport)
std::unique_ptr<XInputDevice_t[]> devices;
HMODULE hXinput;

fn_export double gamepad_oxi_init()
{
    hXinput = LoadDllByName(L"OpenXinput1_4.dll");
    if (hXinput != NULL)
    {
        OpenXInputGetMaxControllerCount_t* pfnOpenXInputGetMaxControllerCount_t = (OpenXInputGetMaxControllerCount_t*)GetProcAddress(hXinput, "OpenXInputGetMaxControllerCount");
        if (pfnOpenXInputGetMaxControllerCount_t != nullptr)
        {// We are using OpenXinput, check for the max controller count.
            XinputMaxControllerCount = pfnOpenXInputGetMaxControllerCount_t();
        }

        pOpenXInputGetDeviceUSBIds = (OpenXInputGetDeviceUSBIds_t*)GetProcAddress(hXinput, "OpenXInputGetDeviceUSBIds");
        pOpenXInputGetStateFull = (OpenXInputGetStateFull_t*)GetProcAddress(hXinput, "OpenXInputGetStateFull");
    }

    devices = std::make_unique<XInputDevice_t[]>(XinputMaxControllerCount);
    memset(devices.get(), 0, sizeof(XInputDevice_t) * XinputMaxControllerCount);
    
    for (int i = 0; i < XinputMaxControllerCount; ++i)
    {
        devices[i].deviceIndex = i;
    }

    return 0;
}

fn_export double gamepad_oxi_update()
{
    for (int i = 0; i < XinputMaxControllerCount; i++)
    {
        XInputDevice_t& controller = devices[i];
        if (controller.connected)
        {
            OnDeviceInfoChange(controller);
        }

        if ((pOpenXInputGetStateFull == nullptr ? OpenXInputGetStateEx(i, &controller.state.XinputState) : pOpenXInputGetStateFull(i, &controller.state)) == ERROR_SUCCESS)
        {
            if (!controller.connected)
            {
                OnDeviceConnect(controller);
            }
        }
        else
        {
            OnDeviceDisconnect(controller);
        }
    }
    return 0;
}

fn_export double gamepad_oxi_quit() 
{
    FreeLibrary(hXinput);
    return 0;
}

fn_export double gamepad_button_check_oxi(double device, double button)
{
    try 
    {
        GameMakerGamepadButtonIDs gp_button = (GameMakerGamepadButtonIDs)(int)button;
        XInputDevice_t& controller = devices[device];
        if (controller.connected) 
        {
            switch (gp_button)
            {
                case gp_face1: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0);
                case gp_face2: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0);
                case gp_face3: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0);
                case gp_face4: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0);
                case gp_shoulderl: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
                case gp_shoulderr: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
                case gp_shoulderlb: return ((controller.state.XinputState.Gamepad.bLeftTrigger) > GAMEPAD_DEADZONE_TRIGGER);
                case gp_shoulderrb: return ((controller.state.XinputState.Gamepad.bRightTrigger) > GAMEPAD_DEADZONE_TRIGGER);
                case gp_padu: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
                case gp_padd: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
                case gp_padl: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
                case gp_padr: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
                case gp_start: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0);
                case gp_select: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0);
                case gp_stickl: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0);
                case gp_stickr: return ((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0);
                default: return 0;
            }
        }
    }
    catch (...) 
    {

    }
    return 0;
}

fn_export double gamepad_button_check_pressed_oxi(double device, double button)
{
    try
    {
        GameMakerGamepadButtonIDs gp_button = (GameMakerGamepadButtonIDs)(int)button;
        XInputDevice_t& controller = devices[device];
        if (controller.connected) 
        {
            switch (gp_button)
            {
                case gp_face1: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0));
                case gp_face2: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0));
                case gp_face3: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0));
                case gp_face4: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0));
                case gp_shoulderl: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0));
                case gp_shoulderr: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0));
                case gp_shoulderlb: return (((controller.state.XinputState.Gamepad.bLeftTrigger) > GAMEPAD_DEADZONE_TRIGGER) && !((controller.oldState.XinputState.Gamepad.bLeftTrigger) > GAMEPAD_DEADZONE_TRIGGER));
                case gp_shoulderrb: return (((controller.state.XinputState.Gamepad.bRightTrigger) > GAMEPAD_DEADZONE_TRIGGER) && !((controller.oldState.XinputState.Gamepad.bRightTrigger) > GAMEPAD_DEADZONE_TRIGGER));
                case gp_padu: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0));
                case gp_padd: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0));
                case gp_padl: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0));
                case gp_padr: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0));
                case gp_start: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0));
                case gp_select: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0));
                case gp_stickl: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0));
                case gp_stickr: return (((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0) && !((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0));
                default: return 0;
            }
        }
    }
    catch (...) 
    {

    }
    return 0;
}

fn_export double gamepad_button_check_released_oxi(double device, double button)
{
    try
    {
        GameMakerGamepadButtonIDs gp_button = (GameMakerGamepadButtonIDs)(int)button;
        XInputDevice_t& controller = devices[device];
        if (controller.connected) 
        {
            switch (gp_button)
            {
                case gp_face1: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0));
                case gp_face2: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0));
                case gp_face3: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0));
                case gp_face4: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0));
                case gp_shoulderl: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0));
                case gp_shoulderr: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0));
                case gp_shoulderlb: return (!((controller.state.XinputState.Gamepad.bLeftTrigger) > GAMEPAD_DEADZONE_TRIGGER) && ((controller.oldState.XinputState.Gamepad.bLeftTrigger) > GAMEPAD_DEADZONE_TRIGGER));
                case gp_shoulderrb: return (!((controller.state.XinputState.Gamepad.bRightTrigger) > GAMEPAD_DEADZONE_TRIGGER) && ((controller.oldState.XinputState.Gamepad.bRightTrigger) > GAMEPAD_DEADZONE_TRIGGER));
                case gp_padu: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0));
                case gp_padd: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0));
                case gp_padl: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0));
                case gp_padr: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0));
                case gp_start: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0));
                case gp_select: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0));
                case gp_stickl: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0));
                case gp_stickr: return (!((controller.state.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0) && ((controller.oldState.XinputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0));
                default: return 0;
            }
        }
    }
    catch (...) 
    {

    }
    return 0;
}

fn_export double gamepad_set_axis_deadzone_oxi(double device, double deadzone)
{
    XInputDevice_t& controller = devices[device];
    controller.deadzone = deadzone;
    return 0;
}

fn_export double gamepad_axis_value_oxi(double device, double axis)
{
    try
    {
        GameMakerGamepadButtonIDs gp_axis = (GameMakerGamepadButtonIDs)(int)axis;
        XInputDevice_t& controller = devices[device];
        if (controller.connected)
        {
            double normalizedValue = 0;
            switch (gp_axis)
            {
                case gp_axislh: normalizedValue = ((double)controller.state.XinputState.Gamepad.sThumbLX / (double)GAMEPAD_STICK_MAX); break;
                case gp_axislv: normalizedValue = -((double)controller.state.XinputState.Gamepad.sThumbLY / (double)GAMEPAD_STICK_MAX); break;
                case gp_axisrh: normalizedValue = ((double)controller.state.XinputState.Gamepad.sThumbRX / (double)GAMEPAD_STICK_MAX); break;
                case gp_axisrv: normalizedValue = -((double)controller.state.XinputState.Gamepad.sThumbRY / (double)GAMEPAD_STICK_MAX); break;
                default: return 0;
            }

            if (std::abs(normalizedValue) < controller.deadzone) { return 0; }
            return normalizedValue;
        }
    }
    catch (...)
    {

    }
    return 0;
}
#pragma endregion Gamemaker_Extension_Code