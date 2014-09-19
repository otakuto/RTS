#include "stdafx.hpp"
#include "Mesh.hpp"

Mesh::Mesh(IDirect3DDevice9 * const device, std::wstring const & file)
	:
	device(device),
	mesh(nullptr),
	material()
{
	//Xファイルのディレクトリを取得(テクスチャのロードに使う)
	wchar_t dir[_MAX_DIR];
	_wsplitpath_s(file.c_str(), nullptr, 0, dir, _MAX_DIR, nullptr, 0, nullptr, 0);

	//Xファイルロード
	ID3DXBuffer * buffer = nullptr;
	{
		DWORD materialLength = 0;
		if (FAILED(D3DXLoadMeshFromX(file.c_str(), D3DXMESH_SYSTEMMEM, device, nullptr, &buffer, nullptr, &materialLength, &mesh)))
		{
			throw;
		}
		material.resize(materialLength);
	}
	
	//法線ベクトルがない場合は法線ベクトルを書き込む
	if (!(mesh->GetFVF() & D3DFVF_NORMAL))
	{
		ID3DXMesh * tmp = nullptr;

		mesh->CloneMeshFVF(mesh->GetOptions(), (mesh->GetFVF() | D3DFVF_NORMAL), device, &tmp);

		D3DXComputeNormals(tmp, nullptr);
		mesh->Release();
		mesh = tmp;
	}

	//マテリアル、テクスチャの準備
	D3DXMATERIAL * d3dxmatrs = reinterpret_cast<D3DXMATERIAL *>(buffer->GetBufferPointer());
	for (unsigned int i = 0; i < material.size(); ++i)
	{
		material[i].first = d3dxmatrs[i].MatD3D;
		material[i].first.Ambient = material[i].first.Diffuse;

		//テクスチャをロード
		if (d3dxmatrs[i].pTextureFilename != nullptr)
		{
			//テクスチャ名をwcharに変換
			wchar_t filename[1024] = {};
			size_t num;
			mbstowcs_s(&num, filename, 1024, d3dxmatrs[i].pTextureFilename, _TRUNCATE);

			//テクスチャファイルパスを作成する
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
