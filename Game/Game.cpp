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
	LPDIRECT3DDEVICE9 const & device = directGraphics.Device();
	static Mesh a = Mesh(device, TEXT("object.x"));
	static Process p = Process(device);
	p.Run();
	
	static bool cell[2][64][64] = {};
	static int view = 0;
	static int buffer = 0;
	static int fr = 0;
	
	{
		static bool t = true;
		if (t)
		{
			std::random_device random;
			std::vector<unsigned int> seed(16);
			std::generate(seed.begin(), seed.end(), std::ref(random));
			std::mt19937 engine(std::seed_seq(seed.begin(), seed.end()));
			std::uniform_int_distribution<> distribution(0, 1);
			auto f = [&engine, &distribution]()
			{
				return distribution(engine);
			};

			for (int i = 0; i < 64; ++i)
			{
				for (int j = 0; j < 64; ++j)
				{
					cell[view][i][j] = f();
				}
			}

			cell[view][6][6] = true;
			cell[view][6][7] = true;
			cell[view][6][8] = true;
			cell[view][5][8] = true;
			cell[view][4][6] = true;
			t = false;
		}
	}



	//if (fr % 10 == 0)
	{
		view = buffer;
		++buffer;
		buffer %= 2;

		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 64; ++j)
			{
				int life = 0;
				for (int k = -1; k <= 1; ++k)
				{
					for (int l = -1; l <= 1; ++l)
					{
						if (!((k == 0) && (l == 0)))
						{
							int x = i + k;
							int y = j + l;
							if ((0 <= x) && (x < 64) && (0 <= y) && (y < 64))
							{
								if (cell[view][x][y])
								{
									++life;
								}
							}
						}
					}
				}
				if (life <= 1)
				{
					cell[buffer][i][j] = false;
				}
				else if (life == 2)
				{
					cell[buffer][i][j] = cell[view][i][j];
				}
				else if (life == 3)
				{
					cell[buffer][i][j] = true;
				}
				else if (life >= 4)
				{
					cell[buffer][i][j] = false;
				}
			}
		}
	}
	++fr;
	

	{
		directInput.Run();

		static float theta = 0;
		static float iota = 0;
		theta -= directInput.MouseState().lX * 0.01;
		iota -= directInput.MouseState().lY * 0.01;
		if (iota > (std::_Pi / 2))
		{
			iota = std::_Pi / 2;
		}
		if (iota < -(std::_Pi / 2))
		{
			iota = -std::_Pi / 2;
		}

		static float A = 0.4;
		static D3DXVECTOR3 eye = D3DXVECTOR3(0, 0, 0);
		auto key = directInput.Key();
		if (key[DIK_D])
		{
			eye.x += std::cos(theta - (std::_Pi / 2)) * A;
			eye.z += std::sin(theta - (std::_Pi / 2)) * A;
		}
		if (key[DIK_A])
		{
			eye.x += std::cos(theta + (std::_Pi / 2)) * A;
			eye.z += std::sin(theta + (std::_Pi / 2)) * A;
		}
		if (key[DIK_SPACE])
		{
			eye.y += A;
		}
		if (key[DIK_LSHIFT])
		{
			eye.y -= A;
		}
		if (key[DIK_W])
		{
			eye.x += std::cos(theta) * A;
			eye.z += std::sin(theta) * A;
		}
		if (key[DIK_S])
		{
			eye.x -= std::cos(theta) * A;
			eye.z -= std::sin(theta) * A;
		}

		static int e = 0;
		if (key[DIK_1])
		{
			++e;
		}
		else
		{
			e = 0;
		}

		if (e == 1)
		{
			cell[buffer][6][6] = true;
			cell[buffer][6][7] = true;
			cell[buffer][6][8] = true;
			cell[buffer][5][8] = true;
			cell[buffer][4][6] = true;
		}

		static D3DXVECTOR3 at = D3DXVECTOR3(0, 0, 0);
		at.x = eye.x + std::cos(theta) * std::cos(iota);
		at.y = eye.y + std::sin(iota);
		at.z = eye.z + std::sin(theta) * std::cos(iota);
		
		static D3DXVECTOR3 up = D3DXVECTOR3(0, 0, 0);
		up.x = -std::cos(theta) * std::sin(iota);
		up.y = std::cos(iota);
		up.z = -std::sin(theta) * std::sin(iota);
		device->SetTransform(D3DTS_VIEW, D3DXMatrixLookAtLH(&D3DXMATRIX(), &eye, &at, &up));
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
	p.Draw();

	{
		static int i = 0;
		p.position.x = -1 + std::cos(i * std::_Pi / 36);
		p.position.y = 1;
		p.position.z = -1 + std::sin(i * std::_Pi / 36);
		++i;
	}

	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 64; ++j)
		{
			if (cell[view][i][j])
			{
				device->SetTransform(D3DTS_WORLD, D3DXMatrixTranslation(&D3DXMATRIX(), i, 0, j));
				a.Draw();
			}
		}
	}
	
	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);
}
