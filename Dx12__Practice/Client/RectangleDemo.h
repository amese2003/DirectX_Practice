#pragma once
#include "IExecute.h"
#include "Mesh.h"

class Shader;
class VertexColorData;
class Mesh;

class TriangleDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	shared_ptr<Shader> _shader;
	vector<VertexColorData> _vertices;
	shared_ptr<Mesh> _mesh;
};

