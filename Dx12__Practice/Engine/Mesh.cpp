#include "pch.h"
#include "Mesh.h"
#include "Graphics.h"
#include "GeometryHelper.h"
#include <fstream>

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
	_vertexBuffer = make_shared<VertexBuffer>();
	_indexBuffer = make_shared<IndexBuffer>();
}

void Mesh::CreateRectangle()
{
	GeometryHelper::CreateQuad(_geometry);


	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}

void Mesh::CreateQuad()
{
	GeometryHelper::CreateQuadWave(_geometry);


	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}

void Mesh::CreateTriangle()
{
	GeometryHelper::CreateTriangle(_geometry);


	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}


void Mesh::CreateCube()
{
	GeometryHelper::CreateCube(_geometry);

	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}

void Mesh::CreateHillDemo()
{
	GeometryHelper::CreateGrid(_geometry, 160.0f, 160.0f, 50, 50, true);
	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}

void Mesh::CreateGrid()
{
	//GeometryHelper::CreateGrid(_geometry, 160.0f, 160.0f, 50, 50);
	 GeometryHelper::CreateGrid(_geometry, 20.f, 30.f, 60, 40);
	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}

void Mesh::CreateSphere()
{
	GeometryHelper::CreateSphere(_geometry);

	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}

void Mesh::CreateCylinder()
{
	GeometryHelper::CreateCylinder(_geometry, 0.5f, 0.3f, 3.0f, 20, 20);

	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}

void Mesh::CreateMesh(const wstring& path)
{
	std::ifstream fin("../Resources/Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"../Resources/Models/skull.txt not found.", 0, 0);
		return;
	}

	uint32 vcount = 0;
	uint32 tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 white(1.0f, 1.0f, 1.0f, 1.0f);

	std::vector<VertexTextureNormalTangentData> vertices(vcount);
	for (uint32 i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;

		vertices[i].Color = white;

		// Normal not used in this demo.
		fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;


	int32 skullIndexCount = 3 * tcount;
	std::vector<uint32> indices(skullIndexCount);
	for (uint32 i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	_geometry->SetVertices(vertices);
	_geometry->SetIndices(indices);

	_vertexBuffer->CreateBuffer(vertices);
	_indexBuffer->CreateBuffer(indices);
}

void Mesh::CreateFloor()
{
	GeometryHelper::CreateFloor(_geometry);

	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
}

void Mesh::CreateWall()
{
	GeometryHelper::CreateWall(_geometry);

	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
}

void Mesh::CreateMirror()
{
	GeometryHelper::CreateRoomMirror(_geometry);

	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
}

void Mesh::CreateTreeSprites()
{
	GeometryHelper::CreateTreeSprites(_geometry);

	_vertexBuffer->CreateBuffer(_geometry->GetVertices());
	_indexBuffer->CreateBuffer(_geometry->GetIndices());
}
