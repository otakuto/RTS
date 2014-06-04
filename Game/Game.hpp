#pragma once
#include "stdafx.hpp"
#include "DirectGraphics.hpp"
#include "DirectInput.hpp"

class Game
{
public:
	static int const WINDOW_WIDTH;
	static int const WINDOW_HEIGHT;

public:
	Game(HWND const & hWnd);
	~Game();

public:
	void Run();

private:
	DirectGraphics directGraphics;
	DirectInput directInput;
};
