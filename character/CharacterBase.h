#pragma once
#include "../StageData.h"
#include "../PlayerData.h"
#include <DxLib.h>
#include <vector>

using namespace std;

class InputState;

class CharacterBase
{
public:
	CharacterBase();
	virtual ~CharacterBase() {}

	virtual void Initialize(const RECT stageMaxSize, const int stageWidthSize);
	virtual void Update(const InputState& inputState, const vector<vector<int>> fallData, const int time);
	virtual void Draw();
	virtual void Finalize();

	const PlayerData GetData() { return data; }
	const int GetHP() { return hp; }
	const bool IsDead() { return isDead; }
	const vector<int> GetCurrentIndex() { return ConvertPosToIndex(); }

protected:
	const void SpawnRandom();
	const vector<int> ConvertPosToIndex();
	const bool IsOnStage(const vector<vector<int>> fallData);

	const VECTOR UpdateAction(const bool isPlayer, const InputState& inputState, const int time);
	const VECTOR AutoAction(const int time, VECTOR& moveVec);
	const VECTOR ManualAction(const InputState& inputState, const int time, VECTOR& moveVec);

	int model;
	RECT stageSize;

	PlayerData data;
	VECTOR direction;
	VECTOR rotate;

	bool isFall;
	bool isDead;
	int hp;

	int currentAction;
	int flame;

	int stageWidth;
	VECTOR fallVelocity;
};

