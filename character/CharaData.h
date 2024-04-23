#pragma once
#include <DxLib.h>

namespace
{
	constexpr float radian = DX_PI_F / 180.0f;

	//各方向のベクトル
	constexpr float leftVec = 90.0f * radian;
	constexpr float backVec = 180.0f * radian;
	constexpr float rightVec = 270.0f * radian;
	constexpr float frontVec = 0.0f * radian;

	constexpr int charaNumAll = 4;
	constexpr int maxHp = 5;
	constexpr double modelScale = 0.3;
	constexpr float charaDeadLine = 349.0f;
	constexpr float initDropRotateY = -1.0f;
}

enum class ActType
{
	wait,
	move,
	drop
};

enum class CharaPower
{
	one,
	line,
	row,
	round
};

struct CharaData
{
	int model = -1;

	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	float dropRotateY = initDropRotateY;
	VECTOR moveRotate = { 0.0f, 0.0f, 0.0f };

	bool isDead = false;
	bool isOnStage = true;
	int hp = maxHp;

	ActType actType = ActType::wait;
	float moveSpeed = 0.0f;
	int stiffenTime = 0;
	CharaPower power = CharaPower::one;
};