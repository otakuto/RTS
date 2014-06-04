#pragma once
#include "stdafx.hpp"

class DirectGraphics
{
public:
	DirectGraphics(HWND const & hWnd);
	~DirectGraphics();

public:
	LPDIRECT3DDEVICE9 const & Device() const;

public:
	void createDirect3DDeviceObject(HWND const & hWnd);
	void setRenderingConfiguration();
	void setFogConfiguration();

protected:
	LPDIRECT3D9	direct3D;
	LPDIRECT3DDEVICE9 device;
	float fogStart;
	float fogEnd;
};
