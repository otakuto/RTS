#include "stdafx.hpp"
#include "Mesh.hpp"

Mesh::Mesh(LPDIRECT3DDEVICE9 const &device, LPCWSTR const &file)
:
device(device),
mesh(nullptr),
texture(nullptr),
material(nullptr),
materialLength(0)
{
	
	//X�t�@�C���̃f�B���N�g�����擾(�e�N�X�`���̃��[�h�Ɏg��)
	wchar_t dir[_MAX_DIR];
	_wsplitpath_s(file, nullptr, 0, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

	//X�t�@�C�����[�h
	LPD3DXBUFFER buffer;
	if (D3D_OK != D3DXLoadMeshFromX(file, D3DXMESH_SYSTEMMEM, device, nullptr, &buffer, nullptr, &materialLength, &mesh))
	{
		throw;
	}
	
	//X�t�@�C���ɖ@�����Ȃ��ꍇ�́A�@������������
	if (!(mesh->GetFVF() & D3DFVF_NORMAL))
	{
		ID3DXMesh *pTempMesh = nullptr;

		mesh->CloneMeshFVF(mesh->GetOptions(), (mesh->GetFVF() | D3DFVF_NORMAL), device, &pTempMesh);

		D3DXComputeNormals(pTempMesh, nullptr);
		mesh->Release();
		mesh = pTempMesh;
	}

	//�}�e���A���A�e�N�X�`���̏���
	material = new D3DMATERIAL9[materialLength];
	texture = new LPDIRECT3DTEXTURE9[materialLength];

	D3DXMATERIAL* d3dxmatrs = reinterpret_cast<D3DXMATERIAL*>(buffer->GetBufferPointer());

	for (unsigned int i = 0; i < materialLength; ++i)
	{
		//�}�e���A������
		material[i] = d3dxmatrs[i].MatD3D;
		material[i].Ambient = material[i].Diffuse;

		//�e�N�X�`�������[�h
		texture[i] = nullptr;
		if (d3dxmatrs[i].pTextureFilename != nullptr)
		{
			//�e�N�X�`������wchar�ɕϊ�
			wchar_t filename[1024] = {};
			size_t num;
			mbstowcs_s(&num, filename, 1024, d3dxmatrs[i].pTextureFilename, _TRUNCATE);
			
			//�e�N�X�`���t�@�C���p�X���쐬����
			wchar_t texturefile[1024] = {};
			swprintf(texturefile, 1024, TEXT("%s%s"), dir, filename);

			if (S_OK != D3DXCreateTextureFromFile(device, texturefile, &texture[i]))
			{
				texture[i] = nullptr;
			}
		}
	}

	buffer->Release();
}

Mesh::~Mesh()
{
	delete[] material;
	material = nullptr;

	for (unsigned int i = 0; i < materialLength; ++i)
	{
		if (texture[i])
		{
			texture[i]->Release();
		}
	}
	delete[] texture;
	texture = nullptr;
}

void Mesh::Draw()
{
	device->SetVertexShader(nullptr);
	device->SetFVF(mesh->GetFVF());
	for (unsigned int i = 0; i < materialLength; ++i)
	{
		device->SetMaterial(&material[i]);
		device->SetTexture(0, texture[i]);
		mesh->DrawSubset(i);
	}
}
