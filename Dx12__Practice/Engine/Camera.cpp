#include "pch.h"
#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	SetLens(0.25f * MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}

Camera::~Camera()
{
}

XMVECTOR Camera::GetPosition()	const
{
	return XMLoadFloat3(&_position);
}

XMFLOAT3 Camera::GetPosition3f()	const
{
	return _position;
}

void Camera::SetPosition(float x, float y, float z)
{
	_position = XMFLOAT3(x, y, z);
	_viewDirty = true;
}

void Camera::SetPosition(const XMFLOAT3& v)
{
	_position = v;
	_viewDirty = true;
}

XMVECTOR Camera::GetRight()	const
{
	return XMLoadFloat3(&_right);
}

XMFLOAT3 Camera::GetRight3f()	const
{
	return _right;
}

XMVECTOR Camera::GetUp() const
{
	return XMLoadFloat3(&_up);
}

XMFLOAT3 Camera::GetUp3f() const
{
	return _up;
}

XMVECTOR Camera::GetLook() const
{
	return XMLoadFloat3(&_look);
}

XMFLOAT3 Camera::GetLook3f() const
{
	return _look;
}

float Camera::GetNearZ() const
{
	return _nearZ;
}

float Camera::GetFarZ() const
{
	return _farZ;
}

float Camera::GetAspect() const
{
	return _aspect;
}

float Camera::GetFovY() const
{
	return _fovY;
}

float Camera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / _nearZ);
}

float Camera::GetNearWindowWidth()	const
{
	return _aspect * _nearWindowHeight;
}

float Camera::GetNearWindowHeight()	const
{
	return _nearWindowHeight;
}

float Camera::GetFarWindowWidth()	const
{
	return _aspect * _farWindowHeight;
}

float Camera::GetFarWindowHeight()	const
{
	return _farWindowHeight;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	_fovY = fovY;
	_aspect = aspect;
	_nearZ = zn;
	_farZ = zf;

	_nearWindowHeight = 2.0f * _nearZ * tanf(0.5f * _fovY);
	_farWindowHeight = 2.0f * _farZ * tanf(0.5f * _fovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(_fovY, _aspect, _nearZ, _farZ);
	XMStoreFloat4x4(&_projMat, P);
}

void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&_position, pos);
	XMStoreFloat3(&_look, L);
	XMStoreFloat3(&_right, R);
	XMStoreFloat3(&_up, U);

	_viewDirty = true;
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);

	_viewDirty = true;
}

XMMATRIX Camera::GetView()const
{
	assert(!_viewDirty);
	return XMLoadFloat4x4(&_viewMat);
}

XMMATRIX Camera::GetProj()const
{
	return XMLoadFloat4x4(&_projMat);
}


XMFLOAT4X4 Camera::GetView4x4f()const
{
	assert(!_viewDirty);
	return _viewMat;
}

XMFLOAT4X4 Camera::GetProj4x4f()const
{
	return _projMat;
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&_right);
	XMVECTOR p = XMLoadFloat3(&_position);
	XMStoreFloat3(&_position, XMVectorMultiplyAdd(s, r, p));

	_viewDirty = true;
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&_look);
	XMVECTOR p = XMLoadFloat3(&_position);
	XMStoreFloat3(&_position, XMVectorMultiplyAdd(s, l, p));

	_viewDirty = true;
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&_right), angle);

	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), R));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), R));

	_viewDirty = true;
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), R));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), R));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), R));

	_viewDirty = true;
}

void Camera::UpdateViewMatrix()
{
	if (_viewDirty)
	{
		XMVECTOR R = XMLoadFloat3(&_right);
		XMVECTOR U = XMLoadFloat3(&_up);
		XMVECTOR L = XMLoadFloat3(&_look);
		XMVECTOR P = XMLoadFloat3(&_position);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&_right, R);
		XMStoreFloat3(&_up, U);
		XMStoreFloat3(&_look, L);

		_viewMat(0, 0) = _right.x;
		_viewMat(1, 0) = _right.y;
		_viewMat(2, 0) = _right.z;
		_viewMat(3, 0) = x;

		_viewMat(0, 1) = _up.x;
		_viewMat(1, 1) = _up.y;
		_viewMat(2, 1) = _up.z;
		_viewMat(3, 1) = y;

		_viewMat(0, 2) = _look.x;
		_viewMat(1, 2) = _look.y;
		_viewMat(2, 2) = _look.z;
		_viewMat(3, 2) = z;

		_viewMat(0, 3) = 0.0f;
		_viewMat(1, 3) = 0.0f;
		_viewMat(2, 3) = 0.0f;
		_viewMat(3, 3) = 1.0f;

		_viewDirty = false;
	}
}


