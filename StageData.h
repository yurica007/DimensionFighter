#pragma once
#include <DxLib.h>

struct StageIndex
{
	int width = -1;
	int height = -1;
};

struct StageFallData
{
	bool isFall = false;
	VECTOR pos = { 0.0f, 0.0f, 0.0f };

	int indexWidth = 0;
	int indexHeight = 0;
};

namespace
{
	const VECTOR gravity = { 0.0f, -0.1f, 0.0f };
	const float deadLine = -10.0f;
}