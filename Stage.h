#pragma once
#include "StageData.h"
#include <vector>
#include <string>
#include <DxLib.h>

using namespace std;

struct ModelData
{
	int handle = -1;
	bool isFall = false;
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

	void SelectFallCube(const PlayerData playerData);
	const RECT GetStageSize();

private:
	void LoadStage();
	void LoadModelHandle();
	void InitializeStage();

	void SelectCubeLine(const int beginIndex, const int endIndex, const int currentIndex);
	vector<string> split(const string& input, const char delimiter);

	int stageSize;
	int stageMaxSize;
	vector<ModelData> model;

	VECTOR modelSize;
	VECTOR initPos;
};

