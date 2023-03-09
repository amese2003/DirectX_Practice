#include "pch.h"
#include "Mesh.h"
#include "Graphics.h"

Mesh::Mesh() : Super(ResourceType::Mesh)
{
}

Mesh::~Mesh()
{
}

void Mesh::Init()
{
	_device = DEVICE;

	_geometry = make_shared<Geometry<VertexColorData>>();
	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_indexBuffer = make_shared<IndexBuffer>(_device);

	CreateDefaultRectangle();
}

void Mesh::CreateDefaultRectangle()
{
	vector<VertexColorData> vec(3);
	vec[0].position = Vec3(0.f, 0.5f, 0.5f);
	vec[0].color = Color(1.f, 0.f, 0.f, 1.f);
	vec[1].position = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Color(0.f, 1.f, 0.f, 1.f);
	vec[2].position = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Color(0.f, 0.f, 1.f, 1.f);

	vector<uint32> indecies{0, 1, 2};

	_geometry->SetVertices(vec);
	_geometry->SetIndices(indecies);

	_vertexBuffer->Create(vec);
	_indexBuffer->Create(indecies);
}


void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBuffer->_vertexBufferView); // Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&_indexBuffer->_indexBufferView);

	//CMD_LIST->DrawInstanced(_vertexBuffer->_count, 1, 0, 0);
	CMD_LIST->DrawIndexedInstanced(_indexBuffer->_count, 1, 0, 0, 0);
}