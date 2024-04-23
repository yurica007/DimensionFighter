#pragma once
#include "../SceneData.h"
#include "../HandleData.h"
#include "../TimeData.h"
#include <vector>

class ReuseData;

class UIUpdateTime
{
public:
	UIUpdateTime(ReuseData& reuse, const SceneType scene);
	~UIUpdateTime() {}

	void Initialize();
	void Update(const Time currentTime, const int countDown);
	void Draw();
	void Finalize();

private:
	ReuseData& reuseData;
	SceneType sceneType;

	std::vector<int> timeHandle;
	std::vector<HandleData> countHandle;

	std::vector<HandleData> uIHandleData;
	std::vector<HandleData> timeHandleData;

	int count;
};