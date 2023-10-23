#pragma once
#include "StageData.h"
#include "PlayerData.h"
#include <vector>
#include <string>
#include <DxLib.h>

using namespace std;

struct ModelData
{
	int handle = -1;
	bool isFall = false;
	bool isFalling = false;
	VECTOR pos = { 0.0f, 0.0f, 0.0f };
};

class Stage
{
public:
	Stage();
	~Stage();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void SelectFallCube(const vector<VECTOR> direction, const vector<vector<int>> index);
	const RECT GetStageSize();
	const vector<vector<int>> GetStageFallData();
	const int GetStageWidthSize() { return stageSize; }

private:
	void LoadStage();
	void LoadModelHandle();
	void InitializeStage();
	void LoadTexture();

	void SelectCubeLine(const int beginIndex, const int endIndex, const int currentIndex);
	vector<string> split(const string& input, const char delimiter);

	int stageSize;
	int stageMaxSize;
	vector<ModelData> cubeModel;
	vector<int> modelInitTexture;
	vector<int> modelTranceTexture;

	int fallPlayer;

	VECTOR modelSize;

	int darkTexture;
};

