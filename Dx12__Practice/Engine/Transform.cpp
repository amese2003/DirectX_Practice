#include "pch.h"
#include "Transform.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Waves.h"
#include "Material.h"
#include "MathHelper.h"


Transform::Transform() : Super(ComponentType::Transform)
{

}

Transform::~Transform()
{

}

void Transform::Awake()
{
}

void Transform::Update()
{
	
	TransformData cbuffer;
	cbuffer.position = _position;
	cbuffer.pad = 1.f;
	cbuffer.world = GetWorldMatrix();
	cbuffer.matView = Camera::S_MatView;
	cbuffer.matProjection = Camera::S_MatProjection;
	cbuffer.worldnvTranspose = MathHelper::InverseTranspose(cbuffer.world);
	cbuffer.worldViewProj = cbuffer.world * cbuffer.matView * cbuffer.matProjection;

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GRAPHICS->GetConstantBuffer(CBV_REGISTER::b1)->PushData(&cbuffer, sizeof(cbuffer));
	GRAPHICS->GetTableDescHeap()->SetConstantBuffer(handle, CBV_REGISTER::b1);

	//CbPerObjectData.gWorld = GetTransform()->GetWorldMatrix();
	//CbPerObjectData.gWorldInvTranspose = MathHelper::InverseTranspose(CbPerObjectData.gWorld);
	//CbPerObjectData.gWorldViewProj = cbuffer.offset * cbuffer.matView * cbuffer.matProjection;;
	
}

Vec3 ToEulerAngles(Quaternion q)
{
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdateTransform()
{
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	_matLocal = matScale * matRotation * matTranslation;

	if (HasParent())
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	else
		_matWorld = _matLocal;

	Quaternion quat;
	_matWorld.Decompose(_scale, quat, _position);
	_rotation = ToEulerAngles(quat);

	// Children
	for (const shared_ptr<Transform>& child : _children)
		child->UpdateTransform();
}

void Transform::SetScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		Vec3 parentScale = _parent->GetScale();
		Vec3 scale = worldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}

void Transform::SetRotation(const Vec3& worldRotation)
{
	if (HasParent())
	{
		Matrix inverseMatrix = _parent->GetWorldMatrix().Invert();

		Vec3 rotation;
		rotation.TransformNormal(worldRotation, inverseMatrix);

		SetLocalRotation(rotation);
	}
	else
		SetLocalRotation(worldRotation);
}

void Transform::SetPosition(const Vec3& worldPosition)
{
	if (HasParent())
	{
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();

		Vec3 position;
		position.Transform(worldPosition, worldToParentLocalMatrix);

		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}
