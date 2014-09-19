#pragma once
#include "stdafx.hpp"
#include "DirectGraphics.hpp"
#include "Game.hpp"

DirectGraphics::DirectGraphics(HWND const & hWnd)
	:
	direct3D(Direct3DCreate9(D3D_SDK_VERSION)),
	device(nullptr),
	fogStart(1),
	fogEnd(100)
{
	if (!direct3D)
	{
		throw;
	}

	//ディスプレイアダプタ
	[&]()
	{
		//D3DPRESENT_PARAMETERS構造体初期化のためディスプレイモードを取得する
		D3DDISPLAYMODE dmode = {};
		direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dmode);

		//デバイスの状態を指定する
		D3DPRESENT_PARAMETERS parameters = {};
		{
			//ウィンドウモード
			parameters.Windowed = true;
			parameters.BackBufferWidth = Game::WINDOW_WIDTH;
			parameters.BackBufferHeight = Game::WINDOW_HEIGHT;
			parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			parameters.BackBufferFormat = dmode.Format;
			parameters.BackBufferCount = 1;

			//Zバッファの設定
			parameters.EnableAutoDepthStencil = true;
			parameters.AutoDepthStencilFormat = D3DFMT_D16;
		}

		for (auto const deviceType : {D3DDEVTYPE_HAL, D3DDEVTYPE_REF})
		{
			for (auto const behaviorFlags : {D3DCREATE_HARDWARE_VERTEXPROCESSING, D3DCREATE_SOFTWARE_VERTEXPROCESSING})
			{
				if (SUCCEEDED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hWnd, behaviorFlags, &parameters, &device)))
				{
					return;
				}
			}
		}
		throw;
	}();

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	//プロジェクション
	{
		D3DXMATRIX projection;
		D3DXMatrixPerspectiveFovLH(&projection, D3DXToRadian(90), static_cast<float>(Game::WINDOW_WIDTH) / static_cast<float>(Game::WINDOW_HEIGHT), fogStart, fogEnd);
		device->SetTransform(D3DTS_PROJECTION, &projection);
	}

	//ライト
	{
		D3DLIGHT9 light = {};
		light.Type = D3DLIGHT_DIRECTIONAL;

		light.Diffuse.r = 0.0f;
		light.Diffuse.g = 0.0f;
		light.Diffuse.b = 0.0f;
		light.Diffuse.a = 0.0f;

		light.Specular.r = 0.0f;
		light.Specular.g = 0.0f;
		light.Specular.b = 0.0f;
		light.Specular.a = 0.0f;

		light.Ambient.r = 1.0f;
		light.Ambient.g = 1.0f;
		light.Ambient.b = 1.0f;
		light.Ambient.a = 0.0f;

		light.Direction = D3DXVECTOR3(0, -1, 0);
		device->SetRenderState(D3DRS_LIGHTING, true);
		device->SetLight(0, &light);
		device->LightEnable(0, true);
	}
}

DirectGraphics::~DirectGraphics()
{
	release(direct3D);
	release(device);
}

LPDIRECT3DDEVICE9 const & DirectGraphics::Device() const
{
	return device;
}
