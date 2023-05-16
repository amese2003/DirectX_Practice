#pragma once
#include "ConstantBuffer.h"

class Shader;


struct MaterialDesc
{
	Color ambient = Color(0.f, 0.f, 0.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(0.f, 0.f, 0.f, 1.f);
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);
};

struct LightDesc
{
	Color ambient = Color(1.f, 1.f, 1.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(1.f, 1.f, 1.f, 1.f);
	Color emissive = Color(1.f, 1.f, 1.f, 1.f);

	Vec3 direction = Vec3(0.f, 0.f, 0.f);;
	float range = 26.f;

	Vec3 Att = Vec3(0.f, 0.f, 0.f);
	float _spot = 90.f;

	Vec3 position = Vec3(0.f, 0.f, 0.f);
	float pad = 0.f;

};

struct LightParams
{
	uint32			lightCount;
	Vec3			eyePosition;
	LightDesc		lights[50];
};


struct TransformData
{
	Vec3   position;
	float  pad;
	Matrix world;
	Matrix matView;
	Matrix matProjection;
	Matrix worldnvTranspose;
	Matrix worldViewProj;
};

