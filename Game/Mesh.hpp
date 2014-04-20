#pragma once
#include "stdafx.hpp"

class Mesh
{
public:
	const LPDIRECT3DDEVICE9 &device;
	LPD3DXMESH mesh;
	LPDIRECT3DTEXTURE9 *texture;
	D3DMATERIAL9 *material;
	DWORD materialLength;

public:
	Mesh(LPDIRECT3DDEVICE9 const &device, LPCWSTR const &file);
	~Mesh();

public:
	void Draw();
};