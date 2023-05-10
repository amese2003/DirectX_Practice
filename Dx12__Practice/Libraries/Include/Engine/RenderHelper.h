#pragma once


struct CameraData
{
	Matrix matView;
	Matrix matProjection;
};

struct TransformData
{
	Matrix offset;
	Matrix matView;
	Matrix matProjection;
};

