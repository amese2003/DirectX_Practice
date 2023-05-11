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

void Mesh::CreateHillDemo()
{
	GeometryHelper::CreateGrid(_geometry, 160.0f, 160.0f, 50, 50, true);
	_vertexBuffer->CreateTexture(_geometry->GetVertices());
	_indexBuffer->CreateTexture(_geometry->GetIndices());
}

void Mesh::CreateGrid()
{
	//GeometryHelper::CreateGrid(_geometry, 160.0f, 160.0f, 50, 50);
	 GeometryHelper::CreateGrid(_geometry, 20.f, 30.f, 60, 40);
	_vertexBuffer->CreateTexture(_geometry->GetVertices());
	_indexBuffer->CreateTexture(_geometry->GetIndices());
}

void Mesh::CreateSphere()
{
	GeometryHelper::CreateSphere(_geometry);

	_vertexBuffer->CreateTexture(_geometry->GetVertices());
	_indexBuffer->CreateTexture(_geometry->GetIndices());
}

void Mesh::CreateCylinder()
{
	GeometryHelper::CreateCylinder(_geometry, 0.5f, 0.3f, 3.0f, 20, 20);

	_vertexBuffer->CreateTexture(_geometry->GetVertices());
	_indexBuffer->CreateTexture(_geometry->GetIndices());
}