#include "pch.h"
#include "MeshRenderer.h"

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
	CMD_LIST->IASetVertexBuffers(0, 1, &_mesh->GetVertexBuffer()->_vertexBufferView); // Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&_mesh->GetIndexBuffer()->_indexBufferView);


	{
		auto worldPos = GetGameObject()->GetTransform()->GetPosition();

		D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer()->PushData(0, &worldPos, sizeof(worldPos));
		GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b0);
	}

	//CMD_LIST->DrawInstanced(_vertexBuffer->_count, 1, 0, 0);
	GRAPHICS->GetTableDescHeap()->CommitTable();
	CMD_LIST->DrawIndexedInstanced(_mesh->GetIndexBuffer()->_count, 1, 0, 0, 0);
}
