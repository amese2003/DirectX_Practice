#include "pch.h"
#include "Mesh.h"
#include "Graphics.h"
#include "GeometryHelper.h"

Mesh::Mesh() : Super(ResourceType::Mesh)
{
}

Mesh::~Mesh()
{
}

void Mesh::Init()
{
	_device = DEVICE;

	_geometry = make_shared<Geometry<VertexTextureNormalTangentData>>();
	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_indexBuffer = make_shared<IndexBuffer>(_device);
}

void Mesh::CreateRectangle()
{
	GeometryHelper::CreateQuad(_geometry);

	_vertexBuffer->CreateTexture(_geometry->GetVertices());
	_indexBuffer->CreateTexture(_geometry->GetIndices());
}

void Mesh::CreateCube()
{
	GeometryHelper::CreateCube(_geometry);

	_vertexBuffer->CreateTexture(_geometry->GetVertices());
	_indexBuffer->CreateTexture(_geometry->GetIndices());
}
