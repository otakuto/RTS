#pragma once
#include "stdafx.hpp"

class Mesh
{
private:
	IDirect3DDevice9 * const device;
	LPD3DXMESH mesh;
	std::vector<std::pair<D3DMATERIAL9, LPDIRECT3DTEXTURE9>> material;
	
public:
	Mesh(IDirect3DDevice9 * const device, std::wstring const & file);
	~Mesh();

public:
	void Draw() const;
};
