#pragma once
#include "stdafx.hpp"
#include "Process.hpp"

Process::Process(LPDIRECT3DDEVICE9 const & device)
	:
	device(device),
	mesh(device, TEXT("s.x")),
	position(0, 0, 0)
{
}

Process::~Process()
{
}

void Process::Run()
{
}

void Process::Draw() const
{
	device->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&D3DXMATRIX(), position.x, position.y, position.z));
	mesh.Draw();
}
