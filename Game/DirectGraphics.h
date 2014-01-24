#pragma once
#include "stdafx.h"

class DirectGraphics
{
public:
	DirectGraphics(const HWND &hWnd);
	~DirectGraphics();

public:
	const LPDIRECT3DDEVICE9 &Device() const;

public:
	void createDirect3DDeviceObject(const HWND &hWnd);
	void setRenderingConfiguration();
	void setFogConfiguration();

protected:
	LPDIRECT3D9	direct3D;
	LPDIRECT3DDEVICE9 device;
	float fogStart;
	float fogEnd;
};
