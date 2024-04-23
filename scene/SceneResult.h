#pragma once
#include "SceneBase.h"
#include "../TimeData.h"
#include "../HandleData.h"
#include <vector>
#include <fstream>

class BackGround;
class UIUpdateTime;
class UIClearTime;
class UICharaType;

class SceneResult :	public SceneBase
{
public:
	SceneResult(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneResult() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	/// <summary>
	/// ランキング情報を受取る
	/// </summary>
	/// <param name="rank">ランキング情報</param>
	void SetRanking(const std::vector<int> rank) { ranking = rank; }

private:
	void LoadFile(std::ifstream& ifs, std::vector<int>& handle);

	BackGround* backGround;
	UIUpdateTime* updateTime;
	UIClearTime* clearTime;

	std::vector<int> ranking;
	std::vector<int> cushionModelHandle;
	std::vector<int> charaModelHandle;
	std::vector<HandleData> resultHandleData;

	int stage;

	int screenGraph;
	int time;

	bool isFade;
};

