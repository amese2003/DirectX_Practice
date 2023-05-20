#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"

MeshRenderer::MeshRenderer() : Super(ComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
	Render();
}

void MeshRenderer::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->GetVertexBufferView()); // Slot: (0~15)



	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->_indexBufferView);


	MaterialDesc pushDesc;
	shared_ptr<Material> material = _mesh->GetMaterial();

	material->UpdateShader();

	if (material)
	{
		pushDesc.ambient = material->GetAmbient();
		pushDesc.diffuse = material->GetDiffuse();
		pushDesc.specular = material->GetSpecular();
		pushDesc.emissive = Color(1.f, 1.f, 1.f, 1.f);
	}
	
	
	MaterialData cbuffer;
	cbuffer.mat = pushDesc;
	cbuffer.texTransform = Matrix::Identity;

	if (_texture)
	{
		cbuffer.texTransform *= 5;
		GRAPHICS->GetTableDescHeap()->SetShaderResourceView(_texture->GetCpuHandle(), SRV_REGISTER::t0);
	}


	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b2)->PushData(&cbuffer, sizeof(cbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b2);
	

	GRAPHICS->GetTableDescHeap()->CommitTable();
	
	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
	else
		CMD_LIST->DrawInstanced(_mesh->GetVertexBuffer()->_count, 1, 0, 0);
	
}

void MeshRenderer::RenderShadow()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->GetVertexBufferView()); // Slot: (0~15)



	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->_indexBufferView);

	MaterialDesc pushDesc;
	shared_ptr<Material> material = _mesh->GetMaterial();

	material->UpdateShader();

	if (material)
	{
		pushDesc.ambient = material->GetAmbient();
		pushDesc.diffuse = material->GetDiffuse();
		pushDesc.specular = material->GetSpecular();
		pushDesc.emissive = Color(1.f, 1.f, 1.f, 1.f);
	}

	shared_ptr<Light> dirlight = CUR_SCENE->GetLight()->GetLight();

	MaterialData cbuffer;
	cbuffer.mat = pushDesc;
	cbuffer.texTransform = Matrix::Identity;

	if (_texture)
	{
		cbuffer.texTransform *= 5;
		GRAPHICS->GetTableDescHeap()->SetShaderResourceView(_texture->GetCpuHandle(), SRV_REGISTER::t0);
	}




	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b2)->PushData(&cbuffer, sizeof(cbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b2);


	GRAPHICS->GetTableDescHeap()->CommitTable();

	if (_mesh->GetIndexBuffer()->GetComPtr() != nullptr)
		CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
	else
		CMD_LIST->DrawInstanced(_mesh->GetVertexBuffer()->_count, 1, 0, 0);
}
