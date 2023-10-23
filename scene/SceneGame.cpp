#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "SceneResult.h"
#include "../Stage.h"
#include "../character/CharacterManager.h"
#include "../ui/UITime.h"
#include "../ui/UILife.h"
#include "../PlayerData.h"
#include "../InputState.h"
#include <DxLib.h>

namespace
{
	VECTOR uiPos = { 0.0f, 700.0f, 0.0f };
	const int gageSpace = 210;
}

SceneGame::SceneGame(SceneManager& manager) :
	SceneBase(manager), stage(new Stage()), character(new CharacterManager()),
	time(new UITime()), prevTime(0), startCount(0), countDown(0)

{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Initialize()
{
	for (int i = 0; i < playerNum; i++)
	{
		life.push_back(new UILife());
	}

	stage->Initialize();
	character->Initialize(stage->GetStageSize(), stage->GetStageWidthSize());
	time->Initialize(uiPos);

	for (int i = 0; i < playerNum; i++)
	{
		life[i]->Initialize({ 325.0f + i * 210.0f, 60.0f, 0.0f });
	}


	startCount = playerNum;
	countDown = playerNum;
}

void SceneGame::Update(const InputState& inputState)
{
//	stage->SelectFallCube(character->GetData());
	stage->SelectFallCube(character->GetDirection(), character->GetCurrentIndex());
	time->SetStartCount(countDown);
	time->SetCurrentTime(currentTime);

	stage->Update();
	//character->Update(inputState, stage->GetStageFallData(), countDown);
	character->Update(inputState, stage->GetStageFallData(), countDown);

	time->Update();

	vector<int> hp = character->GetHP();
	for (int i = 0; i < playerNum; i++)
	{
		life[i]->SetCurrentLife(i, hp[i]);
		life[i]->Update();
	}

	if (countDown != 0)
	{
		startCount++;
		if (startCount == 60)
		{
			startCount = 0;
			if (0 < countDown) countDown--;
		}
	}	
	else
	{
		currentTime.miliSeconds++;
		if (currentTime.miliSeconds == 60)
		{
			currentTime.miliSeconds = 0;
			currentTime.seconds++;
		}

		if (currentTime.seconds == 60)
		{
			currentTime.seconds = 0;
			currentTime.minutes++;
		}
	}

	if (IsTransScene(inputState)) return;
}

void SceneGame::Draw()
{
	stage->Draw();
	character->Draw();
	time->Draw();

	for (int i = 0; i < playerNum; i++)
	{
		life[i]->Draw();
	}

//	DrawFormatString(0, 700, 0xfffff, "%dF%dF%d", currentTime.minutes, currentTime.seconds, currentTime.miliSeconds);
}

void SceneGame::Finalize()
{
	stage->Finalize();
	character->Finalize();
	time->Finalize();

	for (int i = 0; i < playerNum; i++)
	{
		life[i]->Finalize();
		delete life[i];
	}

	delete stage;
	delete character;
	delete time;
}

void SceneGame::SetTranceTime(const int tranceCurrentTime)
{
	prevTime = tranceCurrentTime;
	currentTime.seconds -= tranceCurrentTime;
}

bool SceneGame::IsTransScene(const InputState& inputState)
{
	bool isEnd = false;
	int remainPlayerNum = playerNum;
	const vector<int> hp = character->GetHP();
	vector<int> rank;
	rank.resize(playerNum);

	for (int i = 0; i < playerNum; i++)
	{
		if (hp[i] == 0)
		{
			rank[i] = remainPlayerNum;
			remainPlayerNum--;
		}		
	}

	for (int i = 0; i < playerNum; i++)
	{
		if (remainPlayerNum == 1)
		{
			if (rank[i] == 0)
			{
				rank[i] = 1;
			}
		}
	}

	if (remainPlayerNum == 1)
	{
		isEnd = true;
	}

	const Time clearTime = currentTime;
	if (inputState.IsTriggered(InputType::debug) || isEnd)
	{
		SceneResult* nextScene = new SceneResult(manager_);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
		nextScene->SetRanking(rank);
		nextScene->SetClearTime(clearTime);
		return true;
	}

	if (inputState.IsTriggered(InputType::cancel))
	{
		SceneMenu* menuScene = new SceneMenu(manager_);
		manager_.PushuScene(menuScene);
		menuScene->Initialize();
		return true;
	}

	return false;
}
