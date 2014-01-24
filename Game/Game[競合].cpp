#pragma once
#include "stdafx.h"
#include "Game.h"
#include "DirectGraphics.h"
#include "DirectInput.h"

class XFileObject
{
public:

	LPD3DXMESH	m_pMesh;
	DWORD		m_dwNumMaterials;

	LPDIRECT3DTEXTURE9*	m_ppTextures;
	D3DMATERIAL9*		m_pMatrs;

public:
	XFileObject()
	{
		m_pMesh = NULL;
		m_dwNumMaterials = 0;
		m_ppTextures = NULL;
		m_pMatrs = NULL;

	}

	bool LoadXFile(const LPDIRECT3DDEVICE9 &device, const LPCWSTR &file);
	void Render(const LPDIRECT3DDEVICE9 &device);

	void Release();
};

//X�t�@�C���ǂݍ���
bool XFileObject::LoadXFile(const LPDIRECT3DDEVICE9 &device, const LPCWSTR &file)
{
	//X�t�@�C���̃f�B���N�g�����擾(�e�N�X�`���̃��[�h�Ɏg��)
	wchar_t dir[_MAX_DIR];
	_wsplitpath_s(file, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);

	//X�t�@�C�����[�h
	LPD3DXBUFFER pD3DBufMtr;
	if (D3D_OK != D3DXLoadMeshFromX(file, D3DXMESH_SYSTEMMEM, device, NULL, &pD3DBufMtr, NULL, &m_dwNumMaterials, &m_pMesh))
	{
		return false;
	}

	//X�t�@�C���ɖ@�����Ȃ��ꍇ�́A�@������������
	if (!(m_pMesh->GetFVF() & D3DFVF_NORMAL))
	{

		ID3DXMesh* pTempMesh = NULL;

		m_pMesh->CloneMeshFVF(m_pMesh->GetOptions(),
			m_pMesh->GetFVF() | D3DFVF_NORMAL, device, &pTempMesh);

		D3DXComputeNormals(pTempMesh, NULL);
		m_pMesh->Release();
		m_pMesh = pTempMesh;
	}

	//�}�e���A���A�e�N�X�`���̏���
	m_pMatrs = (D3DMATERIAL9*)malloc(sizeof(D3DMATERIAL9)* m_dwNumMaterials);
	m_ppTextures = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9)* m_dwNumMaterials);

	D3DXMATERIAL* d3dxmatrs = (D3DXMATERIAL*)pD3DBufMtr->GetBufferPointer();

	for (int i = 0; i < (int)m_dwNumMaterials; i++)
	{

		//�}�e���A������
		m_pMatrs[i] = d3dxmatrs[i].MatD3D;
		m_pMatrs[i].Ambient = m_pMatrs[i].Diffuse;

		//�e�N�X�`�������[�h
		m_ppTextures[i] = NULL;
		if (d3dxmatrs[i].pTextureFilename != NULL)
		{

			//�e�N�X�`������wchar�ɕϊ�
			wchar_t filename[1024];
			size_t num;
			ZeroMemory(filename, sizeof(filename));
			mbstowcs_s(&num, filename, 1024, d3dxmatrs[i].pTextureFilename, _TRUNCATE);

			//�e�N�X�`���t�@�C���p�X���쐬����
			wchar_t texturefile[1024];
			ZeroMemory(texturefile, sizeof(texturefile));
			swprintf(texturefile, 1024, TEXT("%s%s"), dir, filename);

			if (S_OK != D3DXCreateTextureFromFile(device, texturefile, &m_ppTextures[i]))
			{
				m_ppTextures[i] = NULL;
			}
		}
	}

	pD3DBufMtr->Release();

	return true;

}

//�`��
void XFileObject::Render(const LPDIRECT3DDEVICE9 &device)
{

	device->SetVertexShader(NULL);
	device->SetFVF(m_pMesh->GetFVF());

	for (unsigned int u = 0; u < m_dwNumMaterials; u++)
	{
		device->SetMaterial(&m_pMatrs[u]);
		device->SetTexture(0, m_ppTextures[u]);
		m_pMesh->DrawSubset(u);
	}

}

//���\�[�X�������
void XFileObject::Release()
{
	//�}�e���A�����
	free(m_pMatrs);
	m_pMatrs = NULL;

	//�e�N�X�`�����
	for (unsigned int u = 0; u < m_dwNumMaterials; u++)
	{
		if (m_ppTextures[u])
		{
			m_ppTextures[u]->Release();
		}
	}
	free(m_ppTextures);
	m_ppTextures = NULL;
}

XFileObject a;

DirectInput *directInput;

Game::Game(const HWND &hWnd)
:
directGraphics(hWnd)
{
	LPDIRECT3DDEVICE9 device = directGraphics.Device();

	directInput = new DirectInput(hWnd);

	a.LoadXFile(device, TEXT("object.x"));
}

Game::~Game()
{
	delete directInput;
}

void Game::Run()
{
	directInput->Run();

	LPDIRECT3DDEVICE9 device = directGraphics.Device();
	D3DXMATRIX matrix;

	static float x = 0;
	static float z = 0;
	if (directInput->Key()[DIK_RIGHT])
	{
		x += 0.1;
	}
	if (directInput->Key()[DIK_LEFT])
	{
		x -= 0.1;
	}
	if (directInput->Key()[DIK_UP])
	{
		z += 0.1;
	}
	if (directInput->Key()[DIK_DOWN])
	{
		z -= 0.1;
	}

	/*
	D3DXMatrixLookAtLH
		(
		&matrix,
		&(D3DXVECTOR3(x, 0, z)), // �J�����̈ʒu
		&(D3DXVECTOR3(x + 1, 0, z)),  // �����_�̈ʒu
		&(D3DXVECTOR3(0, 1, 0))   // �J�������猩��������̃x�N�g��
		);
	*/
	D3DXMatrixLookAtLH
		(
		&matrix,
		&(D3DXVECTOR3(x, 1, z)), // �J�����̈ʒu
		&(D3DXVECTOR3(0, 0, 0)),  // �����_�̈ʒu
		&(D3DXVECTOR3(0, 1, 0))   // �J�������猩��������̃x�N�g��
		);
	device->SetTransform(D3DTS_VIEW, &matrix);
	
	//device->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0L);
	device->BeginScene();
	a.Render(device);
	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);
}