#pragma once
#include "../HandleData.h"
#include <DxLib.h>
#include <vector>

class ReuseData;

class UICharaType
{
public:
	UICharaType(ReuseData& reuse);
	~UICharaType() {}

	void Initialize();
	void Update(const std::vector<VECTOR> charaPos);
	void Draw();
	void Finalize();

private:
	ReuseData& reuseData;

	std::vector<HandleData> typeHandleData;
};

