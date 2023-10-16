#pragma once
#include <DxLib.h>

struct StageIndex
{
	int width = -1;
	int height = -1;
};

struct PlayerData
{
	VECTOR pos = { 0.0f, 0.8f, 0.0f };
	VECTOR rotate = { 0.0f, 0.0f, 0.0f };
};

namespace
{
	const float radian = DX_PI_F / 180.0f;

	const float leftVec = 90.0f * radian;
	const float upVec = 180.0f * radian;
	const float rightVec = 270.0f * radian;
	const float downVec = 0.0f * radian;
}