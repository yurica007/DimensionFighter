#include "CharaManager.h"
#include "../Stage.h"
#include "PC.h"
#include "NPC.h"
#include "../tool/Load.h"
#include "../ReuseData.h"
#include <fstream>

namespace
{
	using namespace std;

	constexpr int playerColor = 0;
}

CharaManager::CharaManager(Stage& stage) :
	stage(stage), chara(0), deadCharaNum(0)
{
}

void CharaManager::Initialize(ReuseData& reuseData)
{
	vector<vector<string>> charaData;

	for (int i = 0; i < charaNumAll; i++)
	{
		if (i == 0) chara.push_back(new PC(stage));
		else chara.push_back(new NPC(stage));
		characterData.push_back(chara[i]->GetData());

		vector<string> data;
		reuseData.RewriteCharaData(data, i);
		charaData.push_back(data);
	}

	vector<int> charaColor;
	reuseData.RewriteColorData(charaColor);

	vector<PosData> index;
	for (int i = 0; i < chara.size(); i++)
	{
		chara[i]->Initialize(charaData[charaColor[i]], charaColor[i], index);
		index.push_back(chara[i]->GetCurrentIndex());
	}
}

void CharaManager::Update(const InputState& inputState, const int time)
{
	vector<PosData> currentIndex;

	for (int i = 0; i < chara.size(); i++)
	{
		currentIndex.push_back(chara[i]->GetCurrentIndex());

		chara[i]->SetIndexAll(GetCharaIndex());
		chara[i]->Update(inputState, time, currentIndex);

		KeepCharaData(chara[i]->GetData(), i);

		chara[i]->SetPosAll(GetCharaPos());
	}

	const int prevRemainCharaNum = deadCharaNum;
	deadCharaNum = charaNumAll - GetRemainCharaNum();

	if (prevRemainCharaNum != deadCharaNum) stage.SetDeadCharaNum(deadCharaNum);
}

void CharaManager::Draw()
{
	for (int i = 0; i < chara.size(); i++)
	{
		chara[i]->Draw();
	}
}

void CharaManager::Finalize()
{
	for (int i = 0; i < chara.size(); i++)
	{
		chara[i]->Finalize();
		delete chara[i];
	}
}

const vector<PosData> CharaManager::GetCharaIndex()
{
	vector<PosData> index;

	for (int i = 0; i < chara.size(); i++)
	{
		//if (charaData[i].isDead) continue;
		index.push_back(chara[i]->GetCurrentIndex());
	}

	return index;
}

const int CharaManager::GetRemainCharaNum()
{
	int remainPlayer = charaNumAll;

	for (int i = 0; i < chara.size(); i++)
	{
		if (!characterData[i].isDead) continue;
		remainPlayer--;
	}

	return remainPlayer;
}

const vector<int> CharaManager::GetCharaHP()
{
	vector<int> hp;

	for (int i = 0; i < chara.size(); i++)
	{
		hp.push_back(characterData[i].hp);
	}

	return hp;
}

const std::vector<bool> CharaManager::GetIsOnStage()
{
	vector<bool> isOnStage;

	for (int i = 0; i < chara.size(); i++)
	{
		isOnStage.push_back(characterData[i].isOnStage);
	}

	return isOnStage;
}

const vector<VECTOR> CharaManager::GetCharaPos()
{
	vector<VECTOR> pos;

	for (int i = 0; i < chara.size(); i++)
	{
		//if (charaData[i].isDead) continue;
		pos.push_back(characterData[i].pos);
	}

	return pos;
}