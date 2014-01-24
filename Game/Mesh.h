#pragma once
#include "stdafx.h"

class Mesh
{
public:
	const LPDIRECT3DDEVICE9 &device;
	LPD3DXMESH mesh;
	LPDIRECT3DTEXTURE9 *texture;
	D3DMATERIAL9 *material;
	DWORD materialLength;

public:
	Mesh(const LPDIRECT3DDEVICE9 &device, const LPCWSTR &file);
	~Mesh();

public:
	void Draw();
};