#pragma once

class Mesh;
class Shader;
class Light;
class TextureMultiple;

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
	void SetTexture(shared_ptr<TextureMultiple> texture) { _textureArr = texture; }

	void Render();
	void RenderShadow();
	void RenderReflect();


	shared_ptr<Texture> _texture2;

	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	InstanceID GetInstanceID();

	
	void SetMateral(shared_ptr<Material> material) { _material = material; }
	shared_ptr<Material> GetMaterial() { return _material; }

	void SetShadow(shared_ptr<Material> material) { _shadowMaterial = material; }
	shared_ptr<Material> GetShadowMaterial() { return _shadowMaterial; }

	void SetReflect(shared_ptr<Material> material) { _reflectMaterial = material; }
	shared_ptr<Material> GetReflectMaterial() { return _reflectMaterial; }



	shared_ptr<Mesh> GetMesh() { return _mesh; }

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Shader> _shader;
	shared_ptr<Texture> _texture;
	shared_ptr<TextureMultiple> _textureArr;

	bool _useShadow = false;


private:
	shared_ptr<Material> _material;
	shared_ptr<Material> _shadowMaterial;
	shared_ptr<Material> _reflectMaterial;

};

