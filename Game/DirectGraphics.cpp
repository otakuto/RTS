#pragma once
#include "stdafx.hpp"
#include "DirectGraphics.hpp"
#include "Game.hpp"

DirectGraphics::DirectGraphics(HWND const &hWnd)
:
fogStart(150),
fogEnd(1000)
{
	direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!direct3D)
	{
		throw;
	}

	//D3DPRESENT_PARAMETERS構造体初期化のためディスプレイモードを取得する
	D3DDISPLAYMODE dmode = {};
	direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dmode);

	//デバイスの状態を指定する
	D3DPRESENT_PARAMETERS d3dpp = {};
	{
		//ウィンドウモード
		d3dpp.Windowed = true;
		d3dpp.BackBufferWidth = Game::WINDOW_WIDTH;
		d3dpp.BackBufferHeight = Game::WINDOW_HEIGHT;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = dmode.Format;
		d3dpp.BackBufferCount = 1;

		//Zバッファの設定
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}

	//ディスプレイアダプタを表すためのデバイスを作成する
	//描画処理方法と頂点処理方法はハードウェア処理が未対応であると失敗するので、処理方法の組み合わせを変えて再度作成を試みる
	HRESULT hr;
	D3DDEVTYPE devtype;
	DWORD behaviorFlags;
	for (int c = 0; c < 4; ++c)
	{
		switch (c)
		{
		case 0:
			//ハードウェアで描画処理を行う
			devtype = D3DDEVTYPE_HAL;
			//頂点処理をハードウェアで行う
			behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			break;
		case 1:
			//ハードウェアで描画処理を行う
			devtype = D3DDEVTYPE_HAL;
			//頂点処理をソフトウェアで行う
			behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			break;
		case 2:
			//ソフトウェアで描画処理を行う
			devtype = D3DDEVTYPE_REF;
			//頂点処理をハードウェアで行う
			behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			break;
		case 3:
			//ソフトウェアで描画処理を行う
			devtype = D3DDEVTYPE_REF;
			//頂点処理をソフトウェアで行う
			behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			break;
		}

		hr = direct3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			devtype,
			hWnd,
			behaviorFlags,
			&d3dpp,
			&device);


		if (FAILED(hr))
		{
			continue;
		}
		break;
	}

	if (FAILED(hr))
	{
		//ソフトウェア処理にも対応していない
		return;
	}

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	//ビュー(カメラ)の設定
	D3DXMATRIX d3dxmatView;
	D3DXMatrixLookAtLH(
		&d3dxmatView,
		&(D3DXVECTOR3(-2.0f, 0.0f, -2.0f)), // カメラの位置
		&(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),  // 注視点の位置
		&(D3DXVECTOR3(0.0f, 1.0f, 0.0f))   // カメラから見た上方向のベクトル
		);
	device->SetTransform(D3DTS_VIEW, &d3dxmatView);

	//プロジェクション(射影)の設定
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(
		&projection,
		static_cast<float>(D3DXToRadian(90)),
		static_cast<float>(Game::WINDOW_WIDTH / Game::WINDOW_HEIGHT),	//アスペクト比
		1,               //クリッピング距離（近いほう）
		100.0f);        //クリッピング距離（遠いほう)
	device->SetTransform(D3DTS_PROJECTION, &projection);

	//ライトの設定
	device->SetRenderState(D3DRS_LIGHTING, true);
	D3DLIGHT9 light = {};
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 0.0f;
	light.Diffuse.g = 0.0f;
	light.Diffuse.b = 0.0f;
	light.Diffuse.a = 0;

	light.Specular.r = 0.0f;
	light.Specular.g = 0.0f;
	light.Specular.b = 0.0f;
	light.Specular.a = 0;

	light.Ambient.r = 1.0f;
	light.Ambient.g = 1.0f;
	light.Ambient.b = 1.0f;
	light.Ambient.a = 0;

	light.Direction = D3DXVECTOR3(0, -1, -0);
	device->SetLight(0, &light);
	device->LightEnable(0, true);
	return;

	//ウィンドウハンドル確認
	if (!hWnd)
	{
		throw TEXT("Direct3Dの作成に失敗しました\nウィンドウハンドルがありません");
	}

	//Direct3Dオブジェクトの作成
	if (!(direct3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		throw TEXT("Direct3Dの作成に失敗しました");
	}

	D3DCAPS9 caps;
	//ダブルステンシルのサポートを確認
	direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if (!(caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED))
	{
		MessageBox(0, TEXT("ダブルステンシルに対応していません"), TEXT(""), MB_OK);
	}
	createDirect3DDeviceObject(hWnd);
	setRenderingConfiguration();
	setFogConfiguration();
}

DirectGraphics::~DirectGraphics()
{
	SAFE_RELEASE(direct3D);
	SAFE_RELEASE(device);
}

void DirectGraphics::createDirect3DDeviceObject(HWND const &hWnd)
{
	D3DPRESENT_PARAMETERS parameters = {};
	parameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	parameters.BackBufferCount = 1;
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	parameters.Windowed = true;
	parameters.EnableAutoDepthStencil = true;
	//Zバッファ24ビット、ステンシルバッファ8ビットなのでOK
	parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	//ダブルステンシル
	parameters.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device)))
	{
		if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device)))
		{
			MessageBox(0, TEXT("HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します"), nullptr, MB_OK);
			if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device)))
			{
				if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device)))
				{
					throw TEXT("DIRECT3Dデバイスの作成に失敗しました");
				}
			}
		}
	}
}

void DirectGraphics::setRenderingConfiguration()
{
	//Zバッファー処理を有効にする
	device->SetRenderState(D3DRS_ZENABLE, true);
	//カリングする
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//ライトを有効にする
	device->SetRenderState(D3DRS_LIGHTING, true);
	//アンビエントライト(環境光)を設定する
	device->SetRenderState(D3DRS_AMBIENT, 0x00000000);
	//スペキュラ(鏡面反射)を有効にする
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
}

void DirectGraphics::setFogConfiguration()
{
	D3DCAPS9 caps = {};
	device->GetDeviceCaps(&caps);
	if (!(caps.RasterCaps & D3DPRASTERCAPS_FOGTABLE))
	{
		if (MessageBox(0, TEXT("ピクセルフォグが使えない可能性があります。続行しますか?"), TEXT("質問"), MB_ABORTRETRYIGNORE) == IDABORT)
		{
			throw TEXT("フォグ設定に失敗しました");
		}
	}
	device->SetRenderState(D3DRS_FOGENABLE, true);
	device->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(150, 255, 255, 255));
	device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	device->SetRenderState(D3DRS_FOGSTART, static_cast<DWORD>(fogStart));
	device->SetRenderState(D3DRS_FOGEND, static_cast<DWORD>(fogEnd));
}

LPDIRECT3DDEVICE9 const &DirectGraphics::Device() const
{
	return device;
}

