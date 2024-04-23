#include "UILifeManager.h"
#include "UICharaLife.h"
#include "../tool/Load.h"
#include "../ReuseData.h"

namespace
{
	using namespace std;

	const char* const uICharaFrameFilePath = "data/UICharaFrame.csv";

	constexpr int uIFrameHeadPosX = 325;
	constexpr int uIPosY = 50;
	constexpr int uIFrameDistX = 210;
}

UILifeManager::UILifeManager(ReuseData& reuse) :
	reuseData(reuse)
{
}

void UILifeManager::Initialize(const std::vector<int> charaHp)
{
	Load* load = new Load();
	vector<int> handle;
	load->LoadGraphFile(uICharaFrameFilePath, handle);
	delete load;

	vector<int> colorNum;
	reuseData.RewriteColorData(colorNum);

	int posX = uIFrameHeadPosX, posY = uIPosY;
	for (int i = 0; i < colorNum.size(); i++)
	{
		charaLife.push_back(new UICharaLife());

		const HandleData handleData = { posX, posY, 1.0, 0.0, handle[colorNum[i]] };
		charaLife[i]->Initialize(handleData, charaHp[i]);

		posX += uIFrameDistX;
	}
}

void UILifeManager::Update(const std::vector<int> charaHp, const std::vector<bool> isOnStage)
{
	for (int i = 0; i < charaLife.size(); i++)
	{
		charaLife[i]->Update(charaHp[i], isOnStage[i]);
	}
}

void UILifeManager::Draw(const int drawHendle)
{
	for (int i = 0; i < charaLife.size(); i++)
	{
		charaLife[i]->Draw(drawHendle);
	}
}

void UILifeManager::Finalize()
{
	for (int i = 0; i < charaLife.size(); i++)
	{
		charaLife[i]->Finalize();
	}
}
