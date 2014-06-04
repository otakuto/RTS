#pragma once
#include "stdafx.hpp"

class Mesh
{
public:
	LPDIRECT3DDEVICE9 const & device;
	LPD3DXMESH mesh;
	LPDIRECT3DTEXTURE9 * texture;
	D3DMATERIAL9 * material;
	DWORD materialLength;

public:
	Mesh(LPDIRECT3DDEVICE9 const & device, LPCWSTR const & file);
	~Mesh();

public:
	void Draw() const;
};