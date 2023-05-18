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
	_shader->Update();
	Render();
}

void MeshRenderer::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->GetVertexBufferView()); // Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->_indexBufferView);


	MaterialDesc pushDesc;
	shared_ptr<Material> material = _mesh->GetMaterial();

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
	CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
	
}
