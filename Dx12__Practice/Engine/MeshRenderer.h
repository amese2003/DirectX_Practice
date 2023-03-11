#pragma once

class Mesh;
class Shader;

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void Update() override;

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }

	void Render();

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Shader> _shader;

};

