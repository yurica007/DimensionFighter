#pragma once
#include "SceneBase.h"
#include "../TimeDatah.h"
#include <vector>

using namespace std;

class UITime;

class SceneResult :	public SceneBase
{
public:
	SceneResult(SceneManager& manager);
	virtual ~SceneResult();

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	void SetRanking(const vector<int> rank);
	void SetClearTime(const Time time);

private:
	virtual bool IsTransScene(const InputState& inputState) override;

	int currentTime;
	vector<int> ranking;
	Time clearTime;
	UITime* ui;
};

