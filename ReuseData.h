#pragma once
#include "TimeData.h"
#include <vector>
#include <string>

/// <summary>
/// シーンで使用するデータを管理するクラス
/// </summary>
class ReuseData
{
public:
	ReuseData();
	~ReuseData(){}

	void Initialize();
	void Finalize();

	//保存しているデータに書き換える
	void RewriteCharaData(std::vector<std::string>& data, const int dataNum) { data = charaData[dataNum]; }
	void RewriteTimeData(std::vector<int>& data) { data = timeHandle; }
	void RewriteFrameData(std::vector<int>& data) { data = frameHandle; }
	void RewriteCursorData(std::vector<int>& data) { data = cursorHandle; }
	void RewriteTypeData(std::vector<int>& data) { data = charaTypeHandle; }
	void RewriteColorData(std::vector<int>& data) { data = charaColor; }
	void SetCharaColor(std::vector<int> color) { charaColor = color; }

	//時間データ
	void RewriteTimeData(Time& data) { data = currentTime; }
	void SetNowTime(const Time time) { currentTime = time; }

	void RewriteScoreData(Time& data) { data = highScore; }
	void SetHighscore(const Time score);
	
private:
	void LoadCharadata();

	void LoadTimeHandle();
	void LoadMenuFrameHandle();
	void LoadCursorHandle();
	void LoadCharaType();

	std::vector<std::vector<std::string>> charaData;

	std::vector<int> timeHandle;
	std::vector<int> frameHandle;
	std::vector<int> cursorHandle;
	std::vector<int> charaTypeHandle;

	std::vector<int> charaColor;

	Time currentTime;
	Time highScore;
};

