#include "UITime.h"
#include <sstream>
#include <fstream>

namespace
{
	const char* const numberFilePath = "data/number/";
	const char* const dataExtension = ".png";

	const char* const numberDataFilePath = "data/NumberData.csv";

	const int graphWidthSize = 13;
	const int graphHeightSize = 17;
	const int graphSpace = 15;
}

UITime::UITime() :
	startCount(0)
{	
}

void UITime::Initialize(const VECTOR pos)
{
	ifstream ifs(numberDataFilePath);
	string line;
	vector<int> num;

	while (getline(ifs, line))
	{
		vector<string> strvec = split(line, ',');
		const string numFile = strvec[0];
		const string numData = numberFilePath + numFile + dataExtension;
		num.push_back(LoadGraph(numData.c_str()));
	}

	numberData.resize(num.size());

	for (int i = 0; i < numberData.size(); i++)
	{
		numberData[i].graph = num[i];
		numberData[i].num = i;
	}

	currentTime.resize(8);

	uiPos = pos;
}

void UITime::Update()
{
}

void UITime::Draw()
{
	for (int i = 0; i < currentTime.size(); i++)
	{
		const auto timePos = currentTime[i].pos;
		DrawExtendGraph(timePos.left, timePos.top, timePos.right, timePos.bottom, currentTime[i].graph, false);
	}

	if (0 < startCount && startCount <= 3)
	{
		for (int i = 0; i < numberData.size(); i++)
		{
			if (startCount == numberData[i].num)
			{
				DrawRotaGraph(640, 360, 0.8, 0.0, numberData[i].graph, true);
			}
//			DrawFormatString(0, 600, 0xffffff, "%d", startCount);
		}
	}	
}

void UITime::Finalize()
{
}

void UITime::SetCurrentTime(const Time time)
{
	currentTime[0].num = time.minutes / 10;
	currentTime[1].num = time.minutes % 10;
	currentTime[2].num = 10;
	currentTime[3].num = time.seconds / 10;
	currentTime[4].num = time.seconds % 10;
	currentTime[5].num = 10;
	currentTime[6].num = time.miliSeconds / 10;
	currentTime[7].num = time.miliSeconds % 10;

	for (int i = 0; i < currentTime.size(); i++)
	{
		currentTime[i].pos.left = i * graphSpace + static_cast<int>(uiPos.x);
		currentTime[i].pos.top = static_cast<int>(uiPos.y);
		currentTime[i].pos.right = i * graphSpace + static_cast<int>(uiPos.x) + graphWidthSize;
		currentTime[i].pos.bottom = static_cast<int>(uiPos.y) + graphHeightSize;
	}

	for (int i = 0; i < numberData.size(); i++)
	{
		for (int j = 0; j < currentTime.size(); j++)
		{
			if (currentTime[j].num == numberData[i].num)
			{
				currentTime[j].graph = numberData[i].graph;
			}
		}
	}
}
