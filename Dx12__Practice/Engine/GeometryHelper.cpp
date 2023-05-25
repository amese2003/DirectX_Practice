#include "pch.h"
#include "GeometryHelper.h"
#include "MathHelper.h"

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	vector<VertexColorData> vertices;
	vertices.resize(4);

	vertices[0].position = Vec3(-0.5f, 0.5f, 0.5f);
	vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
	vertices[1].position = Vec3(0.5f, 0.5f, 0.5f);
	vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);
	vertices[2].position = Vec3(0.5f, -0.5f, 0.5f);
	vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);
	vertices[3].position = Vec3(-0.5f, -0.5f, 0.5f);
	vertices[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	
	vector<uint32> indecies{0, 1, 2, 0, 2, 3};

	geometry->SetVertices(vertices);
	geometry->SetIndices(indecies);
}



void GeometryHelper::CreateGrid(shared_ptr<Geometry<VertexTextureData>> geometry, int32 sizeX, int32 sizeZ)
{
	vector<VertexTextureData> vtx;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureData v;
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	vector<uint32> idx;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateGrid(shared_ptr<Geometry<VertexTextureNormalData>> geometry, int32 sizeX, int32 sizeZ)
{
	vector<VertexTextureNormalData> vtx;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureNormalData v;
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			v.normal = Vec3(0.f, 1.f, 0.f);

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	vector<uint32> idx;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<VertexTextureData> vtx(24);

	// �ո�
	vtx[0] = VertexTextureData(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f));
	vtx[1] = VertexTextureData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f));
	vtx[2] = VertexTextureData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f));
	vtx[3] = VertexTextureData(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f));
	// �޸�
	vtx[4] = VertexTextureData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f));
	vtx[5] = VertexTextureData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f));
	vtx[6] = VertexTextureData(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f));
	vtx[7] = VertexTextureData(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f));
	// ����
	vtx[8] = VertexTextureData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f));
	vtx[9] = VertexTextureData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f));
	vtx[10] = VertexTextureData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f));
	vtx[11] = VertexTextureData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f));
	// �Ʒ���
	vtx[12] = VertexTextureData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f));
	vtx[13] = VertexTextureData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f));
	vtx[14] = VertexTextureData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f));
	vtx[15] = VertexTextureData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f));
	// ���ʸ�
	vtx[16] = VertexTextureData(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f));
	vtx[17] = VertexTextureData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f));
	vtx[18] = VertexTextureData(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f));
	vtx[19] = VertexTextureData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f));
	// �����ʸ�
	vtx[20] = VertexTextureData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f));
	vtx[21] = VertexTextureData(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f));
	vtx[22] = VertexTextureData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f));
	vtx[23] = VertexTextureData(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f));

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// �����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	float radius = 0.5f; // ���� ������
	uint32 stackCount = 20; // ���� ����
	uint32 sliceCount = 20; // ���� ����

	vector<VertexTextureData> vtx;

	VertexTextureData v;

	// �ϱ�
	v.position = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	vtx.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// ������ ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ���� X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ���� ��ġ�� ����
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			vtx.push_back(v);
		}
	}

	// ����
	v.position = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// �ϱ� �ε���
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ���� �ε���
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ���� �ε���
	uint32 bottomIndex = static_cast<uint32>(vtx.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexTextureNormalData>> geometry)
{
	
	vector<VertexTextureNormalData> vertices;
	vertices.resize(4);

	vertices[0].position = Vec3(-0.5f, 0.5f, 0.5f);
	vertices[0].uv = Vec2(0.f, 1.f);
	vertices[0].normal = Vec3(0.f, 0.f, -1.f);
	vertices[1].position = Vec3(0.5f, 0.5f, 0.5f);
	vertices[1].uv = Vec2(0.f, 0.f);
	vertices[1].normal = Vec3(0.f, 0.f, -1.f);
	vertices[2].position = Vec3(0.5f, -0.5f, 0.5f);
	vertices[2].uv = Vec2(1.f, 1.f);
	vertices[2].normal = Vec3(0.f, 0.f, -1.f);
	vertices[3].position = Vec3(-0.5f, -0.5f, 0.5f);
	vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
	vertices[3].uv = Vec2(1.f, 0.f);
	vertices[3].normal = Vec3(0.f, 0.f, -1.f);

	vector<uint32> indecies{ 0, 1, 2, 0, 2, 3 };

	geometry->SetVertices(vertices);
	geometry->SetIndices(indecies);
}

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexTextureNormalData>> geometry)
{
	vector<VertexTextureNormalData> vtx;
	vtx.resize(4);

	vtx[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vtx[0].uv = Vec2(0.f, 1.f);
	vtx[0].normal = Vec3(0.f, 0.f, -1.f);
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].uv = Vec2(0.f, 0.f);
	vtx[1].normal = Vec3(0.f, 0.f, -1.f);
	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].uv = Vec2(1.f, 1.f);
	vtx[2].normal = Vec3(0.f, 0.f, -1.f);
	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].uv = Vec2(1.f, 0.f);
	vtx[3].normal = Vec3(0.f, 0.f, -1.f);
	geometry->SetVertices(vtx);

	vector<uint32> idx = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateCube(shared_ptr<Geometry<VertexTextureNormalData>> geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<VertexTextureNormalData> vtx(24);

	// �ո�
	vtx[0] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 1.0f));
	vtx[1] = VertexTextureNormalData(Vec3(-w2, +h2, -d2), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 0.0f));
	vtx[2] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 0.0f));
	vtx[3] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 1.0f));
	// �޸�
	vtx[4] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 1.0f));
	vtx[5] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 1.0f));
	vtx[6] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 0.0f));
	vtx[7] = VertexTextureNormalData(Vec3(-w2, +h2, +d2), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 0.0f));
	// ����
	vtx[8] = VertexTextureNormalData(Vec3(-w2, +h2, -d2),  Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 1.0f));
	vtx[9] = VertexTextureNormalData(Vec3(-w2, +h2, +d2),  Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 0.0f));
	vtx[10] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 0.0f));
	vtx[11] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 1.0f));
	// �Ʒ���
	vtx[12] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec3(0.0f, -1.0f, 0.0f), Vec2(1.0f, 1.0f));
	vtx[13] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.0f, 1.0f));
	vtx[14] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.0f, 0.0f));
	vtx[15] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec3(0.0f, -1.0f, 0.0f), Vec2(1.0f, 0.0f));
	// ���ʸ�
	vtx[16] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f));
	vtx[17] = VertexTextureNormalData(Vec3(-w2, +h2, +d2), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f));
	vtx[18] = VertexTextureNormalData(Vec3(-w2, +h2, -d2), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f));
	vtx[19] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f));
	// �����ʸ�
	vtx[20] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f));
	vtx[21] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f));
	vtx[22] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f));
	vtx[23] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f));

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// �����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(shared_ptr<Geometry<VertexTextureNormalData>> geometry)
{
	float radius = 0.5f; // ���� ������
	uint32 stackCount = 20; // ���� ����
	uint32 sliceCount = 20; // ���� ����

	vector<VertexTextureNormalData> vtx;

	VertexTextureNormalData v;

	// �ϱ�
	v.position = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	v.normal = v.position;
	v.normal.Normalize();
	vtx.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// ������ ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ���� X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ���� ��ġ�� ����
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.position;
			v.normal.Normalize();

			vtx.push_back(v);
		}
	}

	// ����
	v.position = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	v.normal = v.position;
	v.normal.Normalize();
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// �ϱ� �ε���
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ���� �ε���
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ���� �ε���
	uint32 bottomIndex = static_cast<uint32>(vtx.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	geometry->SetIndices(idx);
}

//void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry)
//{
//	vector<VertexTextureData> vertices;
//	vertices.resize(4);
//
//	vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
//	vertices[0].uv = Vec2(0.f, 1.f);
//	vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
//	vertices[1].uv = Vec2(0.f, 0.f);
//	vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
//	vertices[2].uv = Vec2(1.f, 1.f);
//	vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
//	vertices[3].uv = Vec2(1.f, 0.f);
//	geometry->SetVertices(vertices);
//
//	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
//	geometry->SetIndices(indices);
//}


void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	vector<VertexTextureNormalTangentData> vtx;
	vtx.resize(4);

	/*vtx[0].position = Vec3(-0.5f, 0.5f, 0.5f);
	vtx[0].uv = Vec2(0.f, 0.f);
	vtx[0].normal = Vec3(0.f, 0.f, -1.f);
	vtx[0].tangent = Vec3(1.0f, 0.0f, 0.0f);

	vtx[1].position = Vec3(0.5f, 0.5f, 0.5f);
	vtx[1].uv = Vec2(1.f, 0.f);
	vtx[1].normal = Vec3(0.f, 0.f, -1.f);
	vtx[1].tangent = Vec3(1.0f, 0.0f, 0.0f);

	vtx[2].position = Vec3(0.5f, -0.5f, 0.5f);
	vtx[2].uv = Vec2(1.f, 1.f);
	vtx[2].normal = Vec3(0.f, 0.f, -1.f);
	vtx[2].tangent = Vec3(1.0f, 0.0f, 0.0f);

	vtx[3].position = Vec3(-0.5f, -0.5f, 0.5f);
	vtx[3].uv = Vec2(0.f, 1.f);
	vtx[3].normal = Vec3(0.f, 0.f, -1.f);
	vtx[3].tangent = Vec3(1.0f, 0.0f, 0.0f);*/

	vtx[0].position = Vec3(-0.1f, -0.1f, 0.f);
	vtx[0].uv = Vec2(0.f, 1.f);
	vtx[0].normal = Vec3(0.f, 0.f, -1.f);
	vtx[0].tangent = Vec3(1.0f, 0.0f, 0.0f);

	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].uv = Vec2(0.f, 0.f);
	vtx[1].normal = Vec3(0.f, 0.f, -1.f);
	vtx[1].tangent = Vec3(1.0f, 0.0f, 0.0f);

	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].uv = Vec2(1.f, 1.f);
	vtx[2].normal = Vec3(0.f, 0.f, -1.f);
	vtx[2].tangent = Vec3(1.0f, 0.0f, 0.0f);

	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].uv = Vec2(1.f, 0.f);
	vtx[3].normal = Vec3(0.f, 0.f, -1.f);
	vtx[3].tangent = Vec3(1.0f, 0.0f, 0.0f);
	geometry->SetVertices(vtx);

	vector<uint32> idx = { 0, 1, 2, 2, 1, 3 };
	//vector<uint32> idx = { 0, 1, 2, 0, 2, 3 };
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateCube(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<VertexTextureNormalTangentData> vtx(24);

	// �ո�
	vtx[0] = VertexTextureNormalTangentData(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f), Color(1.f, 0.f, 0.f, 1.f));
	vtx[1] = VertexTextureNormalTangentData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[2] = VertexTextureNormalTangentData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[3] = VertexTextureNormalTangentData(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f), Color(1.f, 0.f, 0.f, 1.f));


	// �޸�
	vtx[4] = VertexTextureNormalTangentData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[5] = VertexTextureNormalTangentData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[6] = VertexTextureNormalTangentData(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[7] = VertexTextureNormalTangentData(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// ����
	vtx[8] = VertexTextureNormalTangentData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[9] = VertexTextureNormalTangentData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[10] = VertexTextureNormalTangentData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[11] = VertexTextureNormalTangentData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// �Ʒ���
	vtx[12] = VertexTextureNormalTangentData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[13] = VertexTextureNormalTangentData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[14] = VertexTextureNormalTangentData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[15] = VertexTextureNormalTangentData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// ���ʸ�
	vtx[16] = VertexTextureNormalTangentData(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[17] = VertexTextureNormalTangentData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[18] = VertexTextureNormalTangentData(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[19] = VertexTextureNormalTangentData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// �����ʸ�
	vtx[20] = VertexTextureNormalTangentData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[21] = VertexTextureNormalTangentData(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[22] = VertexTextureNormalTangentData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[23] = VertexTextureNormalTangentData(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// �ո�
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// �޸�
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// ����
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// �Ʒ���
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ���ʸ�
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// �����ʸ�
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateGrid(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, int32 sizeX, int32 sizeZ)
{
	vector<VertexTextureNormalTangentData> vtx;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureNormalTangentData v;
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			v.normal = Vec3(0.f, 1.f, 0.f);
			v.tangent = Vec3(1.f, 0.f, 0.f);

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	vector<uint32> idx;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	float radius = 0.5f; // ���� ������
	uint32 stackCount = 20; // ���� ����
	uint32 sliceCount = 20; // ���� ����

	vector<VertexTextureNormalTangentData> vtx;

	VertexTextureNormalTangentData v;

	// �ϱ�
	v.position = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	v.normal = v.position;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 0.0f);
	v.tangent.Normalize();
	vtx.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// ������ ���鼭 ������ ����Ѵ� (�ϱ�/���� �������� ���� X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// ���� ��ġ�� ����
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.position;
			v.normal.Normalize();

			v.tangent.x = -radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vtx.push_back(v);
		}
	}

	vtx[0].Color = Color(1.f, 0.f, 0.f, 1.f);

	// ����
	v.position = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	v.normal = v.position;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 0.0f);
	v.tangent.Normalize();
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// �ϱ� �ε���
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ���� �ε���
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ���� �ε���
	uint32 bottomIndex = static_cast<uint32>(vtx.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateGrid(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, float width, float depth, uint32 m, uint32 n, bool demo)
{
	uint32 vertexCount = m * n;
	uint32 faceCount = (m - 1) * (n - 1) * 2;

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	vector<VertexTextureNormalTangentData> vtx;
	vtx.resize(vertexCount);

	for (uint32 i = 0; i < m; i++)
	{
		float z = halfDepth - i * dz;

		for (uint32 j = 0; j < n; j++)
		{
			float x = -halfWidth + j * dx;

			vtx[i * n + j].position = XMFLOAT3(x, 0.0f, z);
			vtx[i * n + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vtx[i * n + j].tangent = XMFLOAT3(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			vtx[i * n + j].uv.x = j * du;
			vtx[i * n + j].uv.y = i * dv;
		}
	}

	if (demo == true)
	{
		for (size_t i = 0; i < vtx.size(); i++)
		{
			Vec3 position = vtx[i].position;
			position.y = GetHeight(position.x, position.z);
			vtx[i].position = position;

			// Color the vertex based on its height.
			if (position.y < -10.0f)
			{
				// Sandy beach color.
				vtx[i].Color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);
			}
			else if (position.y < 5.0f)
			{
				// Light yellow-green.
				vtx[i].Color = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
			}
			else if (position.y < 12.0f)
			{
				// Dark yellow-green.
				vtx[i].Color = XMFLOAT4(0.1f, 0.48f, 0.19f, 1.0f);
			}
			else if (position.y < 20.0f)
			{
				// Dark brown.
				vtx[i].Color = XMFLOAT4(0.45f, 0.39f, 0.34f, 1.0f);
			}
			else
			{
				// White snow.
				vtx[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
	


	vector<uint32> idx(faceCount * 3);

	// Iterate over each quad and compute indices.
	uint32 k = 0;
	for (uint32 i = 0; i < m - 1; ++i)
	{
		for (uint32 j = 0; j < n - 1; ++j)
		{
			//  [1]
			//   |	\
			//  [0] - [2]
			idx[k] = i * n + j;
			idx[k + 1] = i * n + j + 1;
			idx[k + 2] = (i + 1) * n + j;

			//  [4] - [5]
			//      \  |
			//		  [3]
			idx[k + 3] = (i + 1) * n + j;
			idx[k + 4] = i * n + j + 1;
			idx[k + 5] = (i + 1) * n + j + 1;

			k += 6; // next quad
		}
	}



	geometry->SetVertices(vtx);
	geometry->SetIndices(idx);
}

Vec3 GeometryHelper::GetHeightNormal(float x, float z)
{
	XMFLOAT3 n(
		-0.03f * z * cosf(0.1f * x) - 0.3f * cosf(0.1f * z),
		1.0f,
		-0.3f * sinf(0.1f * x) + 0.03f * x * sinf(0.1f * z));

	XMVECTOR unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
	XMStoreFloat3(&n, unitNormal);

	return n;
}

void GeometryHelper::CreateCylinder(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount)
{

	vector<VertexTextureNormalTangentData> vtx;
	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	UINT ringCount = stackCount + 1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for (uint32 i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;

		// vertices of ring
		float dTheta = 2.0f * XM_PI / sliceCount;
		for (uint32 j = 0; j <= sliceCount; ++j)
		{
			VertexTextureNormalTangentData vertex;

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			vertex.position = XMFLOAT3(r * c, y, r * s);

			vertex.uv.x = (float)j / sliceCount;
			vertex.uv.y = 1.0f - (float)i / stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			vertex.tangent = XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius - topRadius;
			XMFLOAT3 bitangent(dr * c, -height, dr * s);

			XMVECTOR T = ::XMLoadFloat3(&vertex.tangent);
			XMVECTOR B = ::XMLoadFloat3(&bitangent);
			XMVECTOR N = ::XMVector3Normalize(::XMVector3Cross(T, B));
			::XMStoreFloat3(&vertex.normal, N);

			vtx.push_back(vertex);
		}
	}

	

	vector<uint32> indices;

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	UINT ringVertexCount = sliceCount + 1;

	// Compute indices for each stack.
	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j + 1);

			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j + 1);
			indices.push_back(i * ringVertexCount + j + 1);
		}
	}

	geometry->SetVertices(vtx);
	geometry->SetIndices(indices);

	CreateCylinderTopCap(geometry, bottomRadius, topRadius, height, sliceCount, stackCount);
	CreateCylinderBottomCap(geometry, bottomRadius, topRadius, height, sliceCount, stackCount);
}

void GeometryHelper::CreateCylinderTopCap(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount)
{
	uint32 baseIndex = (uint32)geometry->GetVertexCount();

	float y = 0.5f * height;
	float dTheta = 2.0f * XM_PI / sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius * cosf(i * dTheta);
		float z = topRadius * sinf(i * dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		//meshData.vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));

		Vec3 position = {x, y, z};
		Vec2 uv = {u, v};
		Vec3 tangent = {1.0f, 0.0f, 0.0f};
		Vec3 normal = {0.0f, 1.0f, 0.0f};


		VertexTextureNormalTangentData vertex = { position, uv, normal, tangent, Color(1,1,1,1)};
		geometry->AddVertex(vertex);
	}

	// Cap center vertex.
	//meshData.vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));
	Vec3 position = { 0.0f, y, 0.0f };
	Vec3 normal = { 0.0f, 1.0f, 0.0f };
	Vec3 tangent = { 1.0f, 0.0f, 0.0f };
	Vec2 uv = { 0.5f, 0.5f };
	VertexTextureNormalTangentData vertex = { position, uv, normal, tangent, Color(1,1,1,1) };
	geometry->AddVertex(vertex);

	// Index of center vertex.
	uint32 centerIndex = (uint32)geometry->GetVertexCount() - 1;

	for (uint32 i = 0; i < sliceCount; ++i)
	{
		geometry->AddIndex(centerIndex);
		geometry->AddIndex(baseIndex + i + 1);
		geometry->AddIndex(baseIndex + i);

		/*meshData.indices.push_back(centerIndex);
		meshData.indices.push_back(baseIndex + i + 1);
		meshData.indices.push_back(baseIndex + i);*/
	}
}

void GeometryHelper::CreateCylinderBottomCap(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount)
{
	// 
	// Build bottom cap.
	//

	uint32 baseIndex = (uint32)geometry->GetVertexCount();
	float y = -0.5f * height;

	// vertices of ring
	float dTheta = 2.0f * XM_PI / sliceCount;
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius * cosf(i * dTheta);
		float z = bottomRadius * sinf(i * dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		Vec3 position = { x, y, z };
		Vec3 normal = { 0.0f, -1.0f, 0.0f };
		Vec3 tangent = { 1.0f, 0.0f, 0.0f };
		Vec2 uv = { u, v };
		
		VertexTextureNormalTangentData vertex = { position, uv, normal, tangent, Color(1,1,1,1) };
		geometry->AddVertex(vertex);
	}

	// Cap center vertex.
	Vec3 position = { 0.0f, y, 0.0f };
	Vec3 normal = { 0.0f, -1.0f, 0.0f };
	Vec3 tangent = { 1.0f, 0.0f, 0.0f };
	Vec2 uv = { 0.5f, 0.5f };
	VertexTextureNormalTangentData vertex = { position, uv, normal, tangent, Color(1,1,1,1) };
	geometry->AddVertex(vertex);

	// Cache the index of center vertex.
	uint32 centerIndex = (uint32)geometry->GetVertexCount() - 1;

	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//meshData.indices.push_back(centerIndex);
		//meshData.indices.push_back(baseIndex + i);
		//meshData.indices.push_back(baseIndex + i + 1);

		geometry->AddIndex(centerIndex);
		geometry->AddIndex(baseIndex + i);
		geometry->AddIndex(baseIndex + i + 1);
	}
}

void GeometryHelper::CreateFloor(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	vector<VertexTextureNormalTangentData> vtx;
	vtx.resize(6);


	// Floor: Observe we tile texture coordinates.
	vtx[0] = VertexTextureNormalTangentData({-3.5f, 0.0f, -10.0f}, { 0.0f, 4.0f }, { 0.0f, 1.0f, 0.0f });
	vtx[1] = VertexTextureNormalTangentData({ -3.5f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	vtx[2] = VertexTextureNormalTangentData({ 7.5f, 0.0f, 0.0f }, { 4.0f, 0.0f }, {0.0f, 1.0f, 0.0f} );

	vtx[3] = VertexTextureNormalTangentData({ -3.5f, 0.0f, -10.0f }, { 0.0f, 4.0f }, { 0.0f, 1.0f, 0.0f });
	vtx[4] = VertexTextureNormalTangentData({ 7.5f, 0.0f, 0.0f }, { 4.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	vtx[5] = VertexTextureNormalTangentData({ 7.5f, 0.0f, -10.0f }, { 4.0f, 4.0f }, { 0.0f, 1.0f, 0.0f });

	

	// Mirror
	//vtx[24] = VertexTextureNormalTangentData({-2.5f, 0.0f, 0.0f}, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });
	//vtx[25] = VertexTextureNormalTangentData({ -2.5f, 4.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	//vtx[26] = VertexTextureNormalTangentData({ 2.5f, 4.0f, 0.0f }, { 1.0f, 0.0f },{0.0f, 0.0f, -1.0f} );

	//vtx[27] = VertexTextureNormalTangentData({-2.5f, 0.0f, 0.0f}, { 0.0f, 1.0f } ,{0.0f, 0.0f, -1.0f });
	//vtx[28] = VertexTextureNormalTangentData({ 2.5f, 4.0f, 0.0f }, { 1.0f, 0.0f } ,{0.0f, 0.0f, -1.0f});
	//vtx[29] = VertexTextureNormalTangentData({ 2.5f, 0.0f, 0.0f }, { 1.0f, 1.0f } ,{0.0f, 0.0f , -1.0f});

	geometry->SetVertices(vtx);

}

void GeometryHelper::CreateWall(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	vector<VertexTextureNormalTangentData> vtx;
	vtx.resize(18);
	// Wall: Observe we tile texture coordinates, and that we
	// leave a gap in the middle for the mirror.
	vtx[0] = VertexTextureNormalTangentData({ -3.5f, 0.0f, 0.0f }, { 0.0f, 2.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[1] = VertexTextureNormalTangentData({ -3.5f, 4.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[2] = VertexTextureNormalTangentData({ -2.5f, 4.0f, 0.0f }, { 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f });

	vtx[3] = VertexTextureNormalTangentData({ -3.5f, 0.0f, 0.0f }, { 0.0f, 2.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[4] = VertexTextureNormalTangentData({ -2.5f, 4.0f, 0.0f }, { 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[5] = VertexTextureNormalTangentData({ -2.5f, 0.0f, 0.0f }, { 0.5f, 2.0f }, { 0.0f, 0.0f, -1.0f });

	vtx[6] = VertexTextureNormalTangentData({ 2.5f, 0.0f, 0.0f }, { 0.0f, 2.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[7] = VertexTextureNormalTangentData({ 2.5f, 4.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[8] = VertexTextureNormalTangentData({ 7.5f, 4.0f, 0.0f }, { 2.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });

	vtx[9] = VertexTextureNormalTangentData({ 2.5f, 0.0f, 0.0f }, { 0.0f, 2.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[10] = VertexTextureNormalTangentData({ 7.5f, 4.0f, 0.0f }, { 2.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[11] = VertexTextureNormalTangentData({ 7.5f, 0.0f, 0.0f }, { 2.0f, 2.0f }, { 0.0f, 0.0f, -1.0f });

	vtx[12] = VertexTextureNormalTangentData({ -3.5f, 4.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[13] = VertexTextureNormalTangentData({ -3.5f, 6.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[14] = VertexTextureNormalTangentData({ 7.5f, 6.0f, 0.0f }, { 6.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });

	vtx[15] = VertexTextureNormalTangentData({ -3.5f, 4.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[16] = VertexTextureNormalTangentData({ 7.5f, 6.0f, 0.0f }, { 6.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[17] = VertexTextureNormalTangentData({ 7.5f, 4.0f, 0.0f }, { 6.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });


	geometry->SetVertices(vtx);
}

void GeometryHelper::CreateRoomMirror(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	vector<VertexTextureNormalTangentData> vtx;
	vtx.resize(6);

	// Mirror
	vtx[0] = VertexTextureNormalTangentData({ -2.5f, 0.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[1] = VertexTextureNormalTangentData({ -2.5f, 4.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[2] = VertexTextureNormalTangentData({ 2.5f, 4.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });

	vtx[3] = VertexTextureNormalTangentData({ -2.5f, 0.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[4] = VertexTextureNormalTangentData({ 2.5f, 4.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	vtx[5] = VertexTextureNormalTangentData({ 2.5f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f , -1.0f });

	geometry->SetVertices(vtx);
}

void GeometryHelper::CreateTreeSprites(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry)
{
	int treecount = 16;

	vector<VertexTextureNormalTangentData> vtx;
	vtx.resize(treecount);

	for (int i = 0; i < treecount; i++)
	{
		float x = MathHelper::RandF(-45.f, 45.f);
		float z = MathHelper::RandF(-45.f, 45.f);
		float y = GetHeight(x, z);

		 
		y += 8.0f;

		vtx[i].position = Vec3(x, y, z);
		vtx[i].sizeW = Vec2(20.f, 20.f);
	}


	vector<uint32> indices = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	geometry->SetVertices(vtx);
	geometry->SetIndices(indices);
}
