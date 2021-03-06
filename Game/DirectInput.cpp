﻿#pragma once
#include "stdafx.hpp"
#include "DirectInput.hpp"

int const DirectInput::KEY_LENGTH = 256;

DirectInput::DirectInput(HWND const & hWnd)
	:
	key(KEY_LENGTH)
{
	DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&directInput), nullptr);

	directInput->CreateDevice(GUID_SysKeyboard, &keyboardDevice, nullptr);
	keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	keyboardDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY));
	keyboardDevice->Acquire();

	directInput->CreateDevice(GUID_SysMouse, &mouseDevice, nullptr);
	mouseDevice->SetDataFormat(&c_dfDIMouse2);
	mouseDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY));
	{
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;
		mouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	}
	mouseDevice->Acquire();
}

DirectInput::~DirectInput()
{
	release(directInput);
	keyboardDevice->Unacquire();
	release(keyboardDevice);
	mouseDevice->Unacquire();
	release(mouseDevice);
}

std::vector<unsigned char const> const & DirectInput::Key() const
{
	return key;
}

DIMOUSESTATE2 const & DirectInput::MouseState() const
{
	return mouseState;
}

void DirectInput::Run()
{
	if (FAILED(keyboardDevice->GetDeviceState((sizeof(unsigned char const) * KEY_LENGTH), key.data())))
	{
		keyboardDevice->Acquire();
	}

	if (FAILED(mouseDevice->GetDeviceState(sizeof(mouseState), &mouseState)))
	{
		mouseDevice->Acquire();
	}
}