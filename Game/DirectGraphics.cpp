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

	//D3DPRESENT_PARAMETERS�\���̏������̂��߃f�B�X�v���C���[�h���擾����
	D3DDISPLAYMODE dmode = {};
	direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dmode);

	//�f�o�C�X�̏�Ԃ��w�肷��
	D3DPRESENT_PARAMETERS d3dpp = {};
	{
		//�E�B���h�E���[�h
		d3dpp.Windowed = true;
		d3dpp.BackBufferWidth = Game::WINDOW_WIDTH;
		d3dpp.BackBufferHeight = Game::WINDOW_HEIGHT;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = dmode.Format;
		d3dpp.BackBufferCount = 1;

		//Z�o�b�t�@�̐ݒ�
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}

	//�f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬����
	//�`�揈�����@�ƒ��_�������@�̓n�[�h�E�F�A���������Ή��ł���Ǝ��s����̂ŁA�������@�̑g�ݍ��킹��ς��čēx�쐬�����݂�
	HRESULT hr;
	D3DDEVTYPE devtype;
	DWORD behaviorFlags;
	for (int c = 0; c < 4; ++c)
	{
		switch (c)
		{
		case 0:
			//�n�[�h�E�F�A�ŕ`�揈�����s��
			devtype = D3DDEVTYPE_HAL;
			//���_�������n�[�h�E�F�A�ōs��
			behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			break;
		case 1:
			//�n�[�h�E�F�A�ŕ`�揈�����s��
			devtype = D3DDEVTYPE_HAL;
			//���_�������\�t�g�E�F�A�ōs��
			behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			break;
		case 2:
			//�\�t�g�E�F�A�ŕ`�揈�����s��
			devtype = D3DDEVTYPE_REF;
			//���_�������n�[�h�E�F�A�ōs��
			behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			break;
		case 3:
			//�\�t�g�E�F�A�ŕ`�揈�����s��
			devtype = D3DDEVTYPE_REF;
			//���_�������\�t�g�E�F�A�ōs��
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
		//�\�t�g�E�F�A�����ɂ��Ή����Ă��Ȃ�
		return;
	}

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	//�r���[(�J����)�̐ݒ�
	D3DXMATRIX d3dxmatView;
	D3DXMatrixLookAtLH(
		&d3dxmatView,
		&(D3DXVECTOR3(-2.0f, 0.0f, -2.0f)), // �J�����̈ʒu
		&(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),  // �����_�̈ʒu
		&(D3DXVECTOR3(0.0f, 1.0f, 0.0f))   // �J�������猩��������̃x�N�g��
		);
	device->SetTransform(D3DTS_VIEW, &d3dxmatView);

	//�v���W�F�N�V����(�ˉe)�̐ݒ�
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(
		&projection,
		static_cast<float>(D3DXToRadian(90)),
		static_cast<float>(Game::WINDOW_WIDTH / Game::WINDOW_HEIGHT),	//�A�X�y�N�g��
		1,               //�N���b�s���O�����i�߂��ق��j
		100.0f);        //�N���b�s���O�����i�����ق�)
	device->SetTransform(D3DTS_PROJECTION, &projection);

	//���C�g�̐ݒ�
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

	//�E�B���h�E�n���h���m�F
	if (!hWnd)
	{
		throw TEXT("Direct3D�̍쐬�Ɏ��s���܂���\n�E�B���h�E�n���h��������܂���");
	}

	//Direct3D�I�u�W�F�N�g�̍쐬
	if (!(direct3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		throw TEXT("Direct3D�̍쐬�Ɏ��s���܂���");
	}

	D3DCAPS9 caps;
	//�_�u���X�e���V���̃T�|�[�g���m�F
	direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if (!(caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED))
	{
		MessageBox(0, TEXT("�_�u���X�e���V���ɑΉ����Ă��܂���"), TEXT(""), MB_OK);
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
	//Z�o�b�t�@24�r�b�g�A�X�e���V���o�b�t�@8�r�b�g�Ȃ̂�OK
	parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	//�_�u���X�e���V��
	parameters.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device)))
	{
		if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device)))
		{
			MessageBox(0, TEXT("HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�"), nullptr, MB_OK);
			if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device)))
			{
				if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device)))
				{
					throw TEXT("DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���");
				}
			}
		}
	}
}

void DirectGraphics::setRenderingConfiguration()
{
	//Z�o�b�t�@�[������L���ɂ���
	device->SetRenderState(D3DRS_ZENABLE, true);
	//�J�����O����
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//���C�g��L���ɂ���
	device->SetRenderState(D3DRS_LIGHTING, true);
	//�A���r�G���g���C�g(����)��ݒ肷��
	device->SetRenderState(D3DRS_AMBIENT, 0x00000000);
	//�X�y�L����(���ʔ���)��L���ɂ���
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
}

void DirectGraphics::setFogConfiguration()
{
	D3DCAPS9 caps = {};
	device->GetDeviceCaps(&caps);
	if (!(caps.RasterCaps & D3DPRASTERCAPS_FOGTABLE))
	{
		if (MessageBox(0, TEXT("�s�N�Z���t�H�O���g���Ȃ��\��������܂��B���s���܂���?"), TEXT("����"), MB_ABORTRETRYIGNORE) == IDABORT)
		{
			throw TEXT("�t�H�O�ݒ�Ɏ��s���܂���");
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

