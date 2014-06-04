#pragma once
#include "stdafx.hpp"
#include "Mesh.hpp"

class Process
{
public:
	Process(LPDIRECT3DDEVICE9 const & device);
	~Process();

	void Run();
	void Draw() const;

private:
	LPDIRECT3DDEVICE9 const & device;
	Mesh mesh;
	D3DXVECTOR3 position;
};
