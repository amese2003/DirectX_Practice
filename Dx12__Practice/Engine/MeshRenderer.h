#pragma once

class Mesh;
class Shader;
class Light;

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void Update() override;

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
	void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }

	void Render();

	shared_ptr<Mesh> GetMesh() { return _mesh; }

	void SetSpot(shared_ptr<Light> spot) { _spotLight = spot; }
	void SetDir(shared_ptr<Light> dir) { _dirLight = dir; }
	void SetPoint(shared_ptr<Light> point) { _point = point; }

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Shader> _shader;
	shared_ptr<Texture> _texture;

	// юс╫ц
private:
	shared_ptr<Light> _spotLight;
	shared_ptr<Light> _dirLight;
	shared_ptr<Light> _point;

};

