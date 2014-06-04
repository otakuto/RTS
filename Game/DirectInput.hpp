#pragma once
#include "stdafx.hpp"

class DirectInput
{
public:
	DirectInput(HWND const & hWnd);
	~DirectInput();

public:
	std::vector<unsigned char const> const & Key() const;
	DIMOUSESTATE2 const & MouseState() const;
public:
	void Run();

protected:
	static int const KEY_LENGTH;

protected:
	LPDIRECTINPUT8 directInput;

	LPDIRECTINPUTDEVICE8 keyboardDevice;
	std::vector<unsigned char const> key;

	LPDIRECTINPUTDEVICE8 mouseDevice;
	DIMOUSESTATE2 mouseState;
};
