#pragma once


struct CameraData
{
	Matrix matView;
	Matrix matProjection;
};

struct TransformData
{
	Matrix matWorld;
};

struct AnimationData
{
	Vec2 spriteOffset;
	Vec2 spriteSize;
	Vec2 textureSize;
	float useAnimation;
	float padding;
};