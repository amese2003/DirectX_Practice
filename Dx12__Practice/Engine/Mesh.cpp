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

//void Mesh::CreateDefaultRectangle()
//{
//	vector<VertexColorData> vec(4);
//	vec[0].position = Vec3(-0.5f, 0.5f, 0.5f);
//	vec[0].color = Color(1.f, 0.f, 0.f, 1.f);
//	vec[1].position = Vec3(0.5f, 0.5f, 0.5f);
//	vec[1].color = Color(0.f, 1.f, 0.f, 1.f);
//	vec[2].position = Vec3(0.5f, -0.5f, 0.5f);
//	vec[2].color = Color(0.f, 0.f, 1.f, 1.f);
//	vec[3].position = Vec3(-0.5f, -0.5f, 0.5f);
//	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
//
//	vector<uint32> indecies{0, 1, 2, 0, 2, 3};
//
//	_geometry->SetVertices(vec);
//	_geometry->SetIndices(indecies);
//
//	_vertexBuffer->Create(vec);
//	_indexBuffer->Create(indecies);
//}


void Mesh::CreateDefaultRectangle()
{
	vector<VertexTextureNormalTangentData> vec(4);
	//GeometryHelper::CreateRectangle(_geometry, Color(0.5f, 0.5f, 0.5f, 1.f));

	GeometryHelper::CreateQuad(_geometry);

	_vertexBuffer->CreateTexture(_geometry->GetVertices());
	_indexBuffer->CreateTexture(_geometry->GetIndices());

	/*vec[0].position = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Color(1.f, 0.f, 0.f, 1.f);
	vec[1].position = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Color(0.f, 1.f, 0.f, 1.f);
	vec[2].position = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Color(0.f, 0.f, 1.f, 1.f);
	vec[3].position = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);*/

	/*_vertexBuffer->Create(vec);
	_indexBuffer->Create(indecies);*/
}

