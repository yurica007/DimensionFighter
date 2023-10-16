#include "Player.h"
#include "InputState.h"

namespace
{
	const int initHP = 5;
	const char* const modelDataPath = "resource/model/Sphere.mv1";
	const char* const greenTexturePath = "resource/model/texture/Green.png";
}

Player::Player() :
	model(-1), stageSize({ 0, 0, 0, 0 }),
	rotate({ 0.0f, 0.0f, 0.0f }), isFall(false),
	hp(0)
{
	data.rotate = { 0.0f, -1.0f, 0.0f };
	hp = initHP;
}

Player::~Player()
{
}

void Player::Initialize(const RECT stageMaxSize)
{
	model = MV1LoadModel(modelDataPath);
	
	const int graph = LoadGraph(greenTexturePath);
	stageSize = stageMaxSize;
}

void Player::Update(const InputState& inputState)
{
	VECTOR moveVec = { 0.0f, 0.0f, 0.0f };

	if (!isFall)
	{
		if (inputState.IsPressed(InputType::left))
		{
			rotate.y = leftVec;
			moveVec.x = -1.0f;
		}
		else if (inputState.IsPressed(InputType::right))
		{
			rotate.y = rightVec;
			moveVec.x = 1.0f;
		}

		if (inputState.IsPressed(InputType::up))
		{
			rotate.y = upVec;
			moveVec.z = 1.0f;
		}
		else if (inputState.IsPressed(InputType::down))
		{
			rotate.y = downVec;
			moveVec.z = -1.0f;
		}

		if (inputState.IsTriggered(InputType::select))
		{
			data.rotate = rotate;
		}
		else
		{
			data.rotate = { 0.0f, -1.0f, 0.0f };
		}
	}

	isFall = IsOnStage();
	if (isFall)
	{
		moveVec.y = -1.0f;
	}
	if (data.pos.y < -10.0f)
	{
		data.pos = { 0.0f, 0.8f, 0.0f };
		moveVec.y = 0.0f;
		hp--;
	}

	if (VSize(moveVec) != 0)
	{
		moveVec = VScale(moveVec, 0.05f);
	}
	data.pos = VAdd(data.pos, moveVec);

	MV1SetPosition(model, data.pos);
	MV1SetRotationXYZ(model, rotate);
}

void Player::Draw()
{
	MV1DrawModel(model);
	DrawFormatString(0, 0, 0xfffff, "x：%f, y：%f, z：%f", data.pos.x, data.pos.y, data.pos.z);
	DrawFormatString(0, 100, 0xfffff, "HP：%d", hp);

	if (isFall) DrawString(0, 200, "落下", 0xfffff);
}

void Player::Finalize()
{
	MV1DeleteModel(model);
}

const bool Player::IsOnStage()
{
	VECTOR modelSize = { 0.3f, 0.3f, 0.3f };

	if (data.pos.x + modelSize.x / 2 < stageSize.left) return true;
	if (stageSize.top < data.pos.z - modelSize.z / 2) return true;
	if (stageSize.right < data.pos.x - modelSize.x / 2) return true;
	if (data.pos.z + modelSize.z / 2 < stageSize.bottom) return true;

	return false;
}
