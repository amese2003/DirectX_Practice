#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;
Vec3 Camera::S_Eyepos = Vec3::Zero;

Camera::Camera() : Super(ComponentType::Camera)
{
	_width = static_cast<float>(GAME->GetGameDesc().width);
	_height = static_cast<float>(GAME->GetGameDesc().height);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	S_Eyepos = GetTransform()->GetPosition();

	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	S_MatView = GetTransform()->GetWorldMatrix().Invert();
	S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);

	CameraData cbuffer;
	cbuffer.matView = S_MatView;
	cbuffer.matProjection = S_MatProjection;

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b0)->PushData(&cbuffer, sizeof(cbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b0);


	D3D12_CPU_DESCRIPTOR_HANDLE objecthandle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b3)->PushData(&GetTransform()->CbPerObjectData, sizeof(GetTransform()->CbPerObjectData));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(objecthandle, CBV_REGISTER::b3);
}
