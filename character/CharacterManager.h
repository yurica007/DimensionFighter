#pragma once
#include "../StageData.h"
#include "../PlayerData.h"
#include <vector>
#include <DxLib.h>

class CharacterBase;
class InputState;

using namespace std;

class CharacterManager
{
public:
	CharacterManager();
	~CharacterManager() {}

	void Initialize(const RECT stageMaxSize, const int stageWidthSize);
	//void Update(const InputState& inputState, const vector<StageFallData> fallData, const int time);
	void Update(const InputState& inputState, const vector<vector<int>> fallData, const int time);
	void Draw();
	void Finalize();

	const vector<PlayerData> GetData();

	const vector<VECTOR> GetPos();
	const vector<VECTOR> GetDirection();
	const vector<vector<int>> GetCurrentIndex();
	const vector<int> GetHP();

private:
	vector<CharacterBase*> character;
};

