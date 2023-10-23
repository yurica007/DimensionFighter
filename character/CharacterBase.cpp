#include "CharacterBase.h"
#include "../InputState.h"

namespace
{
	const VECTOR initRotate = { 0.0f, -1.0f, 0.0f };

	enum
	{
		moveLeft,
		moveTop,
		moveRight,
		moveBottom,
		action,
		all,

		none
	};

	enum
	{
		width,
		height
	};
}

CharacterBase::CharacterBase() :
	model(-1), stageSize({ 0, 0, 0, 0 }), direction(initRotate),
	rotate(initRotate), isFall(false), isDead(false),
	hp(0), currentAction(-1), flame(none), stageWidth(0),
	fallVelocity(gravity)
{
}

void CharacterBase::Initialize(const RECT stageMaxSize, const int stageWidthSize)
{
}

void CharacterBase::Update(const InputState& inputState, const vector<vector<int>> fallData, const int time)
{
}

void CharacterBase::Draw()
{
}

void CharacterBase::Finalize()
{
}

const void CharacterBase::SpawnRandom()
{
	vector<int> spawnIndex;
	spawnIndex.resize(2);

	spawnIndex[width] = GetRand(stageWidth - 1) - stageWidth / 2;
	spawnIndex[height] = GetRand(stageWidth - 1) - stageWidth / 2;

	const float spawnWidthPos = static_cast<float>(spawnIndex[width]) + 0.5f;
	const float spawnDepthPos = static_cast<float>(spawnIndex[height]) + 0.5f;

	data.pos = { spawnWidthPos, 0.8f, spawnDepthPos };
}

const vector<int> CharacterBase::ConvertPosToIndex()
{
	vector<int> currentIndex;
	currentIndex.resize(2);

	currentIndex[width] = data.pos.x + stageWidth / 2;
	currentIndex[height] = data.pos.z + stageWidth / 2;

	return currentIndex;
}

const bool CharacterBase::IsOnStage(const vector<vector<int>> fallData)
{
	VECTOR modelSize = { 0.3f, 0.3f, 0.3f };
	vector<int> currentIndex = ConvertPosToIndex();

	//ステージ内の落下判定
	for (int i = 0; i < fallData.size(); i++)
	{
		if (fallData[i][width] != -1 && fallData[i][height] != -1)
		{
			const bool widthCheck = fallData[i][width] == currentIndex[width];
			const bool heightCheck = fallData[i][height] == currentIndex[height];

			if (widthCheck && heightCheck) return true;
		}
	}

	//ステージ外に移動しても落下しない
	if (data.pos.x < stageSize.left + modelSize.x / 2)
	{
		data.pos.x = stageSize.left + modelSize.x / 2;
	}
	if (stageSize.top < data.pos.z + modelSize.z / 2)
	{
		data.pos.z = stageSize.top - modelSize.z / 2;
	}
	if (stageSize.right < data.pos.x + modelSize.x / 2)
	{
		data.pos.x = stageSize.right - modelSize.x / 2;
	}
	if (data.pos.z < stageSize.bottom + modelSize.z / 2)
	{
		data.pos.z = stageSize.bottom + modelSize.z / 2;
	}
	if (data.pos.y < 0.8f) return true;

	return false;
}

const VECTOR CharacterBase::UpdateAction(const bool isPlayer, const InputState& inputState, const int time)
{
	VECTOR moveVec = { 0.0f, 0.0f, 0.0f };

	if (isPlayer) return ManualAction(inputState, time, moveVec);
	else return AutoAction(time, moveVec);
}

const VECTOR CharacterBase::AutoAction(const int time, VECTOR& moveVec)
{
	if (isFall) return moveVec;

	flame++;
	if (flame == 25)
	{
		currentAction = none;		
	}

	if (flame == 50)
	{
		flame = 0;
		currentAction = GetRand(all);
	}

	if (currentAction == moveLeft)
	{
		rotate.y = leftVec;
		moveVec.x = -1.0f;
	}
	else if (currentAction == moveRight)
	{
		rotate.y = rightVec;
		moveVec.x = 1.0f;
	}

	if (currentAction == moveTop)
	{
		rotate.y = upVec;
		moveVec.z = 1.0f;
	}
	else if (currentAction == moveBottom)
	{
		rotate.y = downVec;
		moveVec.z = -1.0f;
	}

	if (currentAction == action && time == 0)
	{
		data.rotate = rotate;
	}
	else
	{
		data.rotate = initRotate;
	}

	return moveVec;
}

const VECTOR CharacterBase::ManualAction(const InputState& inputState, const int time, VECTOR& moveVec)
{
	if (isFall) return moveVec;

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
	else if (inputState.IsPressed(InputType::up))
	{
		rotate.y = upVec;
		moveVec.z = 1.0f;
	}
	else if (inputState.IsPressed(InputType::down))
	{
		rotate.y = downVec;
		moveVec.z = -1.0f;
	}

	if (inputState.IsTriggered(InputType::select) && time == 0)
	{
		data.rotate = rotate;
	}
	else
	{
		data.rotate = initRotate;
	}

	return moveVec;
}
