#pragma once
#include "PosData.h"
#include <DxLib.h>
#include <vector>

namespace
{
	constexpr VECTOR gravity = { 0.0f, -0.1f, 0.0f };
	constexpr float stageDeadLine = 350.0f;
}

struct BlockIndexData
{
	PosData index;
	int fallNum = -1;
	bool isFall = false;
	bool isNone = false;
};
