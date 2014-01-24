#pragma once
#include "stdafx.hpp"
#include "DirectGraphics.hpp"

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

