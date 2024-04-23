#pragma once
#include "TimeData.h"
#include <vector>
#include <string>

/// <summary>
/// �V�[���Ŏg�p����f�[�^���Ǘ�����N���X
/// </summary>
class ReuseData
{
public:
	ReuseData();
	~ReuseData(){}

	void Initialize();
	void Finalize();

	//�ۑ����Ă���f�[�^�ɏ���������
	void RewriteCharaData(std::vector<std::string>& data, const int dataNum) { data = charaData[dataNum]; }
	void RewriteTimeData(std::vector<int>& data) { data = timeHandle; }
	void RewriteFrameData(std::vector<int>& data) { data = frameHandle; }
	void RewriteCursorData(std::vector<int>& data) { data = cursorHandle; }
	void RewriteTypeData(std::vector<int>& data) { data = charaTypeHandle; }
	void RewriteColorData(std::vector<int>& data) { data = charaColor; }
	void SetCharaColor(std::vector<int> color) { charaColor = color; }

	//���ԃf�[�^
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

