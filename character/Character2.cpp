#include "Character2.h"
#include "../InputState.h"

namespace
{
	const int initHP = 5;
	const char* const modelDataPath = "data/model/SphereAndArrow.mv1";
	const char* const yellowTexturePath = "data/model/texture/Yellow.png";
}

Character2::Character2()
{
}

void Character2::Initialize(const RECT stageMaxSize, const int stageWidthSize)
{
	model = MV1LoadModel(modelDataPath);

	const int graph = LoadGraph(yellowTexturePath);
	MV1SetTextureGraphHandle(model, 0, graph, false);
	stageSize = stageMaxSize;

	hp = initHP;
	stageWidth = stageWidthSize;

	SpawnRandom();
}

void Character2::Update(const InputState& inputState, const vector<vector<int>> fallData, const int time)
{
	if (isDead) return;
	VECTOR moveVec = UpdateAction(false, inputState, time);

	if (VSize(moveVec) != 0)
	{
		moveVec = VScale(moveVec, moveSpeed);
	}
	data.pos = VAdd(data.pos, moveVec);

	isFall = IsOnStage(fallData);

	if (isFall)
	{
		data.pos = VAdd(data.pos, fallVelocity);
	}
	if (data.pos.y < deadLine)
	{
		if (--hp == 0)
		{
			isDead = true;
			return;
		}
		SpawnRandom();
	}

	MV1SetPosition(model, data.pos);
	MV1SetRotationXYZ(model, rotate);
}

void Character2::Draw()
{
	if (!isDead) MV1DrawModel(model);
}

void Character2::Finalize()
{
	MV1DeleteModel(model);
}
