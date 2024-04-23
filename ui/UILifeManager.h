#pragma once
#include "../SceneData.h"
#include "../HandleData.h"
#include <vector>

class ReuseData;
class UICharaLife;

class UILifeManager
{
public:
	UILifeManager(ReuseData& reuse);
	~UILifeManager() {}

	void Initialize(const std::vector<int> charaHp);
	void Update(const std::vector<int> charaHp, const std::vector<bool> isOnStage);
	void Draw(const int drawHendle);
	void Finalize();

private:
	ReuseData& reuseData;
	std::vector<UICharaLife*> charaLife;
};

