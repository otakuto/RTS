#pragma once
#include "stdafx.h"
#include "DirectGraphics.h"

class Game
{
public:
	Game(const HWND &hWnd);
	~Game();

public:
	void Run();

protected:
	DirectGraphics directGraphics;

	LPD3DXMESH pMesh = nullptr;
};

