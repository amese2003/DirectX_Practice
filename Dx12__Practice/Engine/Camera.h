#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective, // ���� ����
	Orthographic, // ���� ����
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;

	void UpdateMatrix();

	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFOV(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }

	Matrix& GetViewMatrix() { return _matView; }
	Matrix& GetProjectionMatrix() { return _matProjection; }

private:
	Matrix _matView = Matrix::Identity;
	Matrix _matProjection = Matrix::Identity;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _width = 0.f;
	float _height = 0.f;


	float _theta = 1.5f * XM_PI;
	float _phi = 0.1f * XM_PI;
	float _radius = 15.0f;
	Vec3 _eyePosW = { 0, 0, 0 };

public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
	static Vec3 S_Eyepos;
};