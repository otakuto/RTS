#pragma once
#include "stdafx.hpp"
#include "Game.hpp"
#include "DirectGraphics.hpp"
#include "DirectInput.hpp"
#include "Mesh.hpp"
#include "Process.hpp"

int const Game::WINDOW_WIDTH = 800;
int const Game::WINDOW_HEIGHT = 800;

Game::Game(HWND const & hWnd)
	:
	directGraphics(hWnd),
	directInput(hWnd)
{
}

Game::~Game()
{
}

void Game::Run()
{
	directInput.Run();

	LPDIRECT3DDEVICE9 const & device = directGraphics.Device();
	static Mesh a = Mesh(device, TEXT("object.x"));
	static Process p = Process(device);

	{
		static float theta = 0;
		static float iota = 0;
		theta -= directInput.MouseState().lX * 0.01;
		iota += directInput.MouseState().lY * 0.01;
		if (iota > (std::_Pi / 2))
		{

		}

		float A = 0.1;
		static D3DXVECTOR3 eye = D3DXVECTOR3(0, 1, 1);
		if (directInput.Key()[DIK_D])
		{
			eye.x += std::cos(theta - (std::_Pi / 2)) * A;
			eye.z += std::sin(theta - (std::_Pi / 2)) * A;
		}
		if (directInput.Key()[DIK_A])
		{
			eye.x += std::cos(theta + (std::_Pi / 2)) * A;
			eye.z += std::sin(theta + (std::_Pi / 2)) * A;
		}
		if (directInput.Key()[DIK_SPACE])
		{
			eye.y += A;
		}
		if (directInput.Key()[DIK_LSHIFT])
		{
			eye.y -= A;
		}
		if (directInput.Key()[DIK_W])
		{
			eye.x += std::cos(theta) * A;
			eye.z += std::sin(theta) * A;
		}
		if (directInput.Key()[DIK_S])
		{
			eye.x -= std::cos(theta) * A;
			eye.z -= std::sin(theta) * A;
		}

		static D3DXVECTOR3 at = D3DXVECTOR3(0, 0, 0);
		at.x = eye.x + std::cos(theta);
		at.y = eye.y - 1;
		at.z = eye.z + std::sin(theta);
		
		static D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

		D3DXMATRIX matrix;
		D3DXMatrixLookAtLH(&matrix, &eye, &at, &up);
		device->SetTransform(D3DTS_VIEW, &matrix);
	}

	device->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1, 0);
	device->BeginScene();

	{
		static float j = 0;
		j += std::_Pi / 12 * 0.1;
		for (int i = 0; i < 10; ++i)
		{
			device->SetTransform(D3DTS_WORLD, D3DXMatrixMultiply(&D3DXMATRIX(), D3DXMatrixTranslation(&D3DXMATRIX(), 0, i * 2, 0), D3DXMatrixRotationY(&D3DXMATRIX(), (std::_Pi / 12) * i + j)));
			a.Draw();
		}
	}
	p.Run();
	p.Draw();

	if (true)
	{
		static unsigned char stage[10][10] = {};

		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				switch (stage[i][j])
				{
				case 0:
					device->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&D3DXMATRIX(), i, 0, j));
					a.Draw();
					break;
				}
			}
		}
	}
	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);
}
