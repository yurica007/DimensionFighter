#pragma once
#include "StageData.h"
#include <dxlib.h>
#include <vector>

using namespace std;

class InputState;

class Player
{
public:
	Player();
	~Player();

	void Initialize(const RECT stageMaxSize);
	void Update(const InputState& inputState);
	void Draw();
	void Finalize();

	const PlayerData GetData() { return data; }
	const int GetHP() { return hp; }

private:
	const bool IsOnStage();

	int model;
	RECT stageSize;

	PlayerData data;
	VECTOR rotate;
	bool isFall;
	int hp;
};

