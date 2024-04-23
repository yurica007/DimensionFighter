#pragma once
#include "../SceneData.h"
#include "../HandleData.h"
#include <vector>

class UILife;

class UICharaLife
{
public:
	UICharaLife();
	~UICharaLife() {}

	void Initialize(const HandleData handleData, const int remainNum);
	void Update(const int remainNum, const bool isOnStage);
	void Draw(const int drawHendle);
	void Finalize();

private:
	std::vector<UILife*> life;

	HandleData lifeHandle;
	int lifeNum;

	int shakeWidth;
	int shakeHeight;

	int screenGraph;
};

