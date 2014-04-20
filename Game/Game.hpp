#pragma once
#include "stdafx.hpp"
#include "DirectGraphics.hpp"

class Game
{
public:
	static int const WINDOW_WIDTH;
	static int const WINDOW_HEIGHT;

public:
	Game(HWND const &hWnd);
	~Game();

public:
	void Run();

protected:
	DirectGraphics directGraphics;

	LPD3DXMESH pMesh;
};

