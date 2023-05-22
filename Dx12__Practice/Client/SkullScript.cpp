#include "pch.h"
#include "SkullScript.h"

void SkullScript::Start()
{

}

void SkullScript::Update()
{
	double dt = TIME->DeltaTime();

	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::KEY_1))
		pos += GetTransform()->GetRight() * _speed * dt;

	if (INPUT->GetButton(KEY_TYPE::KEY_2))
		pos -= GetTransform()->GetRight() * _speed * dt;

	if (INPUT->GetButton(KEY_TYPE::KEY_3))
		pos += GetTransform()->GetLook() * _speed * dt;

	if (INPUT->GetButton(KEY_TYPE::KEY_4))
		pos -= GetTransform()->GetLook() * _speed * dt;


	GetTransform()->SetLocalPosition(pos);
}
