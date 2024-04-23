#include "ReuseData.h"
#include "tool/Load.h"
#include <fstream>
#include <DxLib.h>

namespace
{
	using namespace std;

	const char* const charaDataFilePath = "data/Charadata.csv";
	const char* const timeFilePath = "data/TimeData.csv";
	const char* const MenuFrameFilePath = "data/UIMenu.csv";
	const char* const cursorMenuFilePath = "data/MenuCursor.csv";
	const char* const charaTypeFilePath = "data/UICharaType.csv";
}

ReuseData::ReuseData()
{
}

void ReuseData::Initialize()
{
	LoadCharadata();

	LoadTimeHandle();
	LoadMenuFrameHandle();
	LoadCursorHandle();
	LoadCharaType();

	Load* load = new Load();
	highScore = load->LoadHighScore();
	delete load;
}

void ReuseData::Finalize()
{
}

void ReuseData::SetHighscore(const Time score)
{
	Time time;
	RewriteScoreData(time);

	if (time.minutes == time.seconds && time.minutes == time.miliSeconds && time.seconds == time.miliSeconds && time.miliSeconds < 0) highScore = score;

	if (score.minutes < time.minutes) highScore = score;
	if (score.minutes == time.minutes)
	{
		if (score.seconds < time.seconds) highScore = score;
		if (score.seconds == time.seconds && score.miliSeconds < time.miliSeconds) highScore = score;
	}
}

void ReuseData::LoadCharadata()
{
	Load* load = new Load();
	ifstream ifs(charaDataFilePath);
	string line;

	while (getline(ifs, line))
	{
		const vector<string> strvec = load->LoadCSVData(line, ',');
		const int loadLine = load->GetLoadLine();

		//à–¾•¶‚ÍÈ‚­
		if (loadLine == commentLine) continue;
		charaData.push_back(strvec);
	}
	delete load;
}

void ReuseData::LoadTimeHandle()
{
	Load* load = new Load();
	load->LoadGraphFile(timeFilePath, timeHandle);
	delete load;
}

void ReuseData::LoadMenuFrameHandle()
{
	Load* load = new Load();
	load->LoadGraphFile(MenuFrameFilePath, frameHandle);
	delete load;
}

void ReuseData::LoadCursorHandle()
{
	Load* load = new Load();
	load->LoadGraphFile(cursorMenuFilePath, cursorHandle);
	delete load;

	cursorHandle.push_back(cursorHandle.back());
}

void ReuseData::LoadCharaType()
{
	Load* load = new Load();
	load->LoadGraphFile(charaTypeFilePath, charaTypeHandle);
	delete load;
}