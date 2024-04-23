#include "UICharaLife.h"
#include "UILife.h"
#include "../tool/Load.h"
#include "../GameData.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	const char* const uICharaLifeFilePath = "data/UICharaLife.csv";

	constexpr int uiLifeDistX = 25;
	constexpr int shakeRate = 7;
}

UICharaLife::UICharaLife() :
	lifeNum(0),	shakeWidth(0), shakeHeight(0), screenGraph(-1)
{
}

void UICharaLife::Initialize(const HandleData handleData, const int remainNum)
{
	lifeHandle = handleData;
	lifeNum = remainNum;

	Load* load = new Load();
	vector<int> handle;
	load->LoadGraphFile(uICharaLifeFilePath, handle);
	delete load;

	int posX = handleData.posX - lifeNum / 2 * uiLifeDistX, posY = handleData.posY;
	for (int i = 0; i < lifeNum; i++)
	{
		life.push_back(new UILife());
		vector<HandleData> data;
		for (int j = 0; j < handle.size(); j++)
		{
			data.push_back({ posX, posY, 1.0, 0.0, handle[j] });
		}
		life[i]->Initialize(data);

		posX += uiLifeDistX;
	}

	screenGraph = MakeScreen(screenWidth, screenHeight, true);
}

void UICharaLife::Update(const int remainNum, const bool isOnStage)
{
	lifeNum = remainNum;

	if (!isOnStage && remainNum != 0)
	{
		shakeWidth = GetRand(shakeRate) - (shakeRate / 2 + 1);
		shakeHeight = GetRand(shakeRate) - (shakeRate / 2 + 1);
	}
}

void UICharaLife::Draw(const int drawHendle)
{
	SetDrawScreen(screenGraph);
	ClearDrawScreen();
	DrawRotaGraph(lifeHandle.posX, lifeHandle.posY, lifeHandle.extRate, lifeHandle.angle, lifeHandle.handle, true);

	for (int i = 0; i < life.size(); i++)
	{
		bool isValid = true;
		if (lifeNum <= i) isValid = false;

		life[i]->Draw(isValid);
	}

	SetDrawScreen(drawHendle);

	const int posX = halfScreenWidth + shakeWidth, posY = halfScreenHeight + shakeHeight;
	DrawRotaGraph(posX, posY, 1.0, 0.0, screenGraph, true);
}

void UICharaLife::Finalize()
{
	for (int i = 0; i < life.size(); i++)
	{
		life[i]->Finalize();
	}
}