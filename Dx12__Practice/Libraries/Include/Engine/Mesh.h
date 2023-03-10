#pragma once
#include "VertexData.h"

class VertexBuffer;
class ConstTransform;

class Mesh : public ResourceBase
{
	using Super = ResourceBase;

public:
	Mesh();
	virtual ~Mesh();

	void Init();
	void CreateDefaultRectangle();
	void Render();

	//void SetTransform(const ConstTransform& t) { _transform = t; }
	void SetTransform(const Transform& t);

private:
	ComPtr<ID3D12Device> _device;

	shared_ptr<Geometry<VertexColorData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

	Transform _transform = {};
};

