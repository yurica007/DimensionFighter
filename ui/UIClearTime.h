#pragma once
#include "../SceneData.h"
#include "../HandleData.h"
#include <vector>

class ReuseData;

class UIClearTime
{
public:
	UIClearTime(ReuseData& reuse, const SceneType scene);
	virtual ~UIClearTime() {}

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	ReuseData& reuseData;
	SceneType sceneType;

	std::vector<int> timeHandle;

	std::vector<HandleData> uIHandleData;
	std::vector<HandleData> scoreHandleData;

	float time;
	float angle;

	bool isNewRecord;

	bool isDraw;
};