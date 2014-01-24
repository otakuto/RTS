#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>

#include <vector>
#include <list>
#include <map>
#include <memory>
#include <functional>
#include <array>
#include <random>

#include <boost/coroutine/coroutine.hpp>

#include <d3dx9.h>
#include <dinput.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

template<typename T>
inline void SAFE_RELEASE(T &p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}

template<typename T>
inline void SAFE_DELETE(T &p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

template<typename T>
inline void SAFE_DELETE_ARRAY(T &p)
{
	if (p)
	{
		delete[] p;
		p = nullptr;
	}
}

template<typename T>
inline RECT RECT_INIT(const T &left, const T &top, const T &right, const T &bottom)
{
	RECT rect = {left, top, right, bottom};
	return rect;
}
