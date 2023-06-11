#pragma once
#include "Geometry.h"
#include "VertexData.h"

class GeometryHelper
{
public:
	static void CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color);
	static void CreateGrid(shared_ptr<Geometry<VertexColorData>> geometry, float width, float depth, uint32 m, uint32 n);

	static void CreateGrid(shared_ptr<Geometry<VertexTextureData>> geometry, int32 sizeX, int32 sizeZ);
	static void CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry);

	static void CreateRectangle(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalData>> geometry, int32 sizeX, int32 sizeZ);
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalData>> geometry);

	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateTriangle(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateQuadWave(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, int32 sizeX, int32 sizeZ);
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);


	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, float width, float depth, uint32 m, uint32 n, bool demo = false);

	static float GetHeight(float x, float z) { return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z)); }
	static Vec3 GetHeightNormal(float x, float z);

	static void CreateCylinder(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount);
	static void CreateCylinderTopCap(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount);
	static void CreateCylinderBottomCap(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount);
	

	static void CreateFloor(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateWall(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateRoomMirror(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);

	static void CreateTreeSprites(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
};

