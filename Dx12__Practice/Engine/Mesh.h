#pragma once
#include "VertexData.h"

class VertexBuffer;

class Mesh : public ResourceBase
{
	using Super = ResourceBase;

public:
	Mesh();
	virtual ~Mesh();

	void Init();
	void CreateDefaultRectangle();

	void Render();

private:
	ComPtr<ID3D12Device> _device;

	shared_ptr<Geometry<VertexColorData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
};

