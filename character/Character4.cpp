#include "Character4.h"
#include "../InputState.h"

namespace
{
	const int initHP = 5;
	const char* const modelDataPath = "data/model/SphereAndArrow.mv1";
	const char* const greenTexturePath = "data/model/texture/Green.png";
}

Character4::Character4()
{
}

void Character4::Initialize(const RECT stageMaxSize, const int stageWidthSize)
{
	model = MV1LoadModel(modelDataPath);

	const int graph = LoadGraph(greenTexturePath);
	MV1SetTextureGraphHandle(model, 0, graph, false);
	stageSize = stageMaxSize;

	hp = initHP;
	stageWidth = stageWidthSize;

	SpawnRandom();
}

void Character4::Update(const InputState& inputState, const vector<vector<int>> fallData, const int time)
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

void Character4::Draw()
{
	if (!isDead) MV1DrawModel(model);
}

void Character4::Finalize()
{
	MV1DeleteModel(model);
}
