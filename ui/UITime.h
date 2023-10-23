#pragma once
#include "../TimeDatah.h"
#include "UIBase.h"
#include <DxLib.h>
#include <vector>
#include <string>

using namespace std;

struct NumberData
{
	int graph = 0;
	int num = 0;
};

struct CurrentTime
{
	int graph = 0;
	int num = 0;
	RECT pos{ 0, 0, 0, 0 };
};

class UITime : public UIBase
{
public:
	UITime();
	virtual ~UITime() {}

	virtual void Initialize(const VECTOR pos) override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Finalize() override;

	void SetCurrentTime(const Time time);
	void SetStartCount(const int count) { startCount = count; }

private:
	vector<NumberData> numberData;
	vector<CurrentTime> currentTime;

	int startCount;
};