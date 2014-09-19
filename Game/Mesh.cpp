#include "stdafx.hpp"
#include "Mesh.hpp"

Mesh::Mesh(IDirect3DDevice9 * const device, std::wstring const & file)
	:
	device(device),
	mesh(nullptr),
	material()
{
	//X�t�@�C���̃f�B���N�g�����擾(�e�N�X�`���̃��[�h�Ɏg��)
	wchar_t dir[_MAX_DIR];
	_wsplitpath_s(file.c_str(), nullptr, 0, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

	//X�t�@�C�����[�h
	ID3DXBuffer * buffer = nullptr;
	{
		DWORD materialLength = 0;
		if (FAILED(D3DXLoadMeshFromX(file.c_str(), D3DXMESH_SYSTEMMEM, device, nullptr, &buffer, nullptr, &materialLength, &mesh)))
		{
			throw;
		}
		material.resize(materialLength);
	}
	
	//�@���x�N�g�����Ȃ��ꍇ�͖@���x�N�g������������
	if (!(mesh->GetFVF() & D3DFVF_NORMAL))
	{
		ID3DXMesh * tmp = nullptr;

		mesh->CloneMeshFVF(mesh->GetOptions(), (mesh->GetFVF() | D3DFVF_NORMAL), device, &tmp);

		D3DXComputeNormals(tmp, nullptr);
		mesh->Release();
		mesh = tmp;
	}

	//�}�e���A���A�e�N�X�`���̏���
	D3DXMATERIAL * d3dxmatrs = reinterpret_cast<D3DXMATERIAL *>(buffer->GetBufferPointer());
	for (unsigned int i = 0; i < material.size(); ++i)
	{
		material[i].first = d3dxmatrs[i].MatD3D;
		material[i].first.Ambient = material[i].first.Diffuse;

		//�e�N�X�`�������[�h
		if (d3dxmatrs[i].pTextureFilename != nullptr)
		{
			//�e�N�X�`������wchar�ɕϊ�
			wchar_t filename[1024] = {};
			size_t num;
			mbstowcs_s(&num, filename, 1024, d3dxmatrs[i].pTextureFilename, _TRUNCATE);

			//�e�N�X�`���t�@�C���p�X���쐬����
			std::wstring texturefile = dir;
			texturefile += filename;
			if (FAILED(D3DXCreateTextureFromFile(device, texturefile.c_str(), &material[i].second)))
			{
				material[i].second = nullptr;
			}
		}
	}
	buffer->Release();
}

Mesh::~Mesh()
{
	mesh->Release();
}

void Mesh::Draw() const
{
	device->SetVertexShader(nullptr);
	device->SetFVF(mesh->GetFVF());
	for (unsigned int i = 0; i < material.size(); ++i)
	{
		device->SetMaterial(&material[i].first);
		device->SetTexture(0, material[i].second);
		mesh->DrawSubset(i);
	}
}
