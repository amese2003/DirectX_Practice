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

	if (_texture)
	{
		GRAPHICS->GetTableDescHeap()->SetShaderResourceView(_texture->GetCpuHandle(), SRV_REGISTER::t0);
	}


	MaterialDesc pushDesc;
	pushDesc.ambient = _mesh->GetMaterial()->GetAmbient();
	pushDesc.diffuse = _mesh->GetMaterial()->GetDiffuse();
	pushDesc.specular = _mesh->GetMaterial()->GetSpecular();
	pushDesc.emissive = Color(1.f, 1.f, 1.f, 1.f);

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b2)->PushData(&pushDesc, sizeof(pushDesc));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b2);
	

	GRAPHICS->GetTableDescHeap()->CommitTable();
	CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
	
}
