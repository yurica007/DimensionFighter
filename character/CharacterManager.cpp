#include "CharacterManager.h"
#include "Character1.h"
#include "Character2.h"
#include "Character3.h"
#include "Character4.h"
#include "../PlayerData.h"

CharacterManager::CharacterManager() :
	character(0)
{
}

void CharacterManager::Initialize(const RECT stageMaxSize, const int stageWidthSize)
{
	character.resize(playerNum);

	character[0] = new Character1();
	character[1] = new Character2();
	character[2] = new Character3();
	character[3] = new Character4();

	for (int i = 0; i < playerNum; i++)
	{
		character[i]->Initialize(stageMaxSize, stageWidthSize);
	}
}

//void CharacterManager::Update(const InputState& inputState, const vector<StageFallData> fallData, const int time)
//{
//	for (int i = 0; i < playerNum; i++)
//	{
//		character[i]->Update(inputState, fallData, time);
//	}
//}

void CharacterManager::Update(const InputState& inputState, const vector<vector<int>> fallData, const int time)
{
	for (int i = 0; i < playerNum; i++)
	{
		character[i]->Update(inputState, fallData, time);
	}
}

void CharacterManager::Draw()
{
	for (int i = 0; i < playerNum; i++)
	{
		character[i]->Draw();
	}
}

void CharacterManager::Finalize()
{
	for (int i = 0; i < playerNum; i++)
	{
		character[i]->Finalize();
		delete character[i];
	}
}

const vector<PlayerData> CharacterManager::GetData()
{
	vector<PlayerData> playerData;

	for (int i = 0; i < playerNum; i++)
	{
		playerData.push_back(character[i]->GetData());
	}

	return playerData;
}

const vector<VECTOR> CharacterManager::GetPos()
{
	vector<VECTOR> pos;

	for (int i = 0; i < playerNum; i++)
	{
		pos.push_back(character[i]->GetData().pos);
	}

	return pos;
}

const vector<VECTOR> CharacterManager::GetDirection()
{
	vector<VECTOR> direction;

	for (int i = 0; i < playerNum; i++)
	{
		direction.push_back(character[i]->GetData().rotate);
	}
	
	return direction;
}

const vector<vector<int>> CharacterManager::GetCurrentIndex()
{
	vector<vector<int>> index;

	for (int i = 0; i < playerNum; i++)
	{
		index.push_back(character[i]->GetCurrentIndex());
	}

	return index;
}

const vector<int> CharacterManager::GetHP()
{
	vector<int> hp;

	for (int i = 0; i < playerNum; i++)
	{
		hp.push_back(character[i]->GetHP());
	}

	return hp;
}