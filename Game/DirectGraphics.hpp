#pragma once
#include "stdafx.hpp"

class DirectGraphics
{
public:
	DirectGraphics(HWND const & hWnd);
	~DirectGraphics();

public:
	LPDIRECT3DDEVICE9 const & Device() const;

private:
	LPDIRECT3D9	direct3D;
	LPDIRECT3DDEVICE9 device;
	float fogStart;
	float fogEnd;
};
