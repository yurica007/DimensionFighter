#include "Character1.h"
#include "../InputState.h"

namespace
{
	const int initHP = 5;
	const char* const modelDataPath = "data/model/SphereAndArrow.mv1";
	const char* const pinkTexturePath = "data/model/texture/Pink.png";
}

Character1::Character1()
{
}

void Character1::Initialize(const RECT stageMaxSize, const int stageWidthSize)
{
	model = MV1LoadModel(modelDataPath);

	const int graph = LoadGraph(pinkTexturePath);
	MV1SetTextureGraphHandle(model, 0, graph, false);
	stageSize = stageMaxSize;
	
	hp = initHP;
	stageWidth = stageWidthSize;

	SpawnRandom();
}

void Character1::Update(const InputState& inputState, const vector<vector<int>> fallData, const int time)
{
	if (isDead) return;
	VECTOR moveVec = UpdateAction(true, inputState, time);

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

void Character1::Draw()
{
	if (!isDead) MV1DrawModel(model);
}

void Character1::Finalize()
{
	MV1DeleteModel(model);
}