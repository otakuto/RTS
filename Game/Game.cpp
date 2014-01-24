#pragma once
#include "stdafx.h"
#include "Game.h"
#include "DirectGraphics.h"
#include "DirectInput.h"
#include "Mesh.h"

Mesh *a;

DirectInput *directInput;

Game::Game(const HWND &hWnd)
:
directGraphics(hWnd)
{
	const LPDIRECT3DDEVICE9 &device = directGraphics.Device();

	directInput = new DirectInput(hWnd);

	a = new Mesh(device, TEXT("object.x"));
}

Game::~Game()
{
	delete directInput;
	delete a;
}

void Game::Run()
{
	directInput->Run();

	const LPDIRECT3DDEVICE9 &device = directGraphics.Device();
	{
		static D3DXVECTOR3 eye = D3DXVECTOR3(0, 1, 1);
		if (directInput->Key()[DIK_D])
		{
			eye.x += static_cast<float>(0.1);
		}
		if (directInput->Key()[DIK_A])
		{
			eye.x -= static_cast<float>(0.1);
		}
		if (directInput->Key()[DIK_SPACE])
		{
			eye.y += static_cast<float>(0.1);
		}
		if (directInput->Key()[DIK_LSHIFT])
		{
			eye.y -= static_cast<float>(0.1);
		}
		if (directInput->Key()[DIK_W])
		{
			eye.z += static_cast<float>(0.1);
		}
		if (directInput->Key()[DIK_S])
		{
			eye.z -= static_cast<float>(0.1);
		}

		static D3DXVECTOR3 at = D3DXVECTOR3(0, 0, 0);
		at.x = eye.x + 1;
		at.y = eye.y - 1;
		at.z = eye.z + 1;
		directInput->MouseState().lX * 0.1;
		directInput->MouseState().lY * 0.1;

		static D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

		D3DXMATRIX matrix;
		D3DXMatrixLookAtLH(&matrix, &eye, &at, &up);
		device->SetTransform(D3DTS_VIEW, &matrix);
	}

	device->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1, 0);
	device->BeginScene();
	/*
	{
	static float j = 0;
	j += std::_Pi / 12 * 0.1;
	for (int i = 0; i < 10; ++i)
	{
	device->SetTransform(D3DTS_WORLD, D3DXMatrixMultiply(&D3DXMATRIX(), D3DXMatrixTranslation(&D3DXMATRIX(), 0, i * 2, 0), D3DXMatrixRotationY(&D3DXMATRIX(), (std::_Pi / 12) * i + j)));
	a->Draw();
	}
	}
	*/
	{
		static unsigned char stage[10][10] = {0};

		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				switch (stage[i][j])
				{
				case 0:
					device->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&D3DXMATRIX(), i, 0, j));
					a->Draw();
					break;
				}
			}
		}
	}
	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);
}