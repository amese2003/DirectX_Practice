#pragma once


struct VertexTextureData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };

	static vector<D3D12_INPUT_ELEMENT_DESC> descs;
};

struct VertexColorData
{
	Vec3 position = { 0, 0, 0 };
	Color color = { 0, 0, 0, 0 };

	static vector<D3D12_INPUT_ELEMENT_DESC> descs;
};

struct VertexTextureNormalData
{
	Vec3 position = { 0, 0, 0 };
	Vec3 normal = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };

	static vector<D3D12_INPUT_ELEMENT_DESC> descs;
};

struct VertexTextureNormalTangentData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
	Vec3 tangent = { 0, 0, 0 };
	Color Color = { 1, 1, 1, 1 };
	Vec2 sizeW = { 0, 0 };
};

class MeshVertex
{
public:
	static VertexTextureNormalTangentData VertexData;
	static vector<D3D12_INPUT_ELEMENT_DESC> VertexDesc;
};