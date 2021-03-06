﻿#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <array>
#include <random>
#include <string>
#include <utility>

#include <d3dx9.h>
#include <dinput.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

template<typename T>
inline void release(T & t)
{
	if (t)
	{
		t->Release();
		t = nullptr;
	}
}
