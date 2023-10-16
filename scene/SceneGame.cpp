#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneMenu.h"
#include "SceneResult.h"
#include "../Player.h"
#include "../Stage.h"
#include "../InputState.h"
#include <DxLib.h>

SceneGame::SceneGame(SceneManager& manager) :
	SceneBase(manager), stage(new Stage()), player(new Player()),
	prevTime(0)

{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Initialize()
{
	stage->Initialize();
	player->Initialize(stage->GetStageSize());

	prevTime = GetNowCount();
	currentTime.seconds = prevTime - prevTime;
}

void SceneGame::Update(const InputState& inputState)
{
	stage->SelectFallCube(player->GetData());

	stage->Update();
	player->Update(inputState);

	currentTime.seconds = (GetNowCount() - prevTime) / 1000;
	if (currentTime.seconds == 60)
	{
		currentTime.seconds = 0;
		currentTime.minutes++;
		prevTime = GetNowCount();
	}

	if (IsTransScene(inputState)) return;
}

void SceneGame::Draw()
{
	stage->Draw();
	player->Draw();
	
	DrawFormatString(0, 700, 0xfffff, "%dF%d", currentTime.minutes, currentTime.seconds);
}

void SceneGame::Finalize()
{
	stage->Finalize();
	player->Finalize();

	delete stage;
	delete player;
}

bool SceneGame::IsTransScene(const InputState& inputState)
{
	if (inputState.IsTriggered(InputType::debug) || player->GetHP() == 0)
	{
		SceneResult* nextScene = new SceneResult(manager_);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
		return true;
	}

	if (inputState.IsTriggered(InputType::cancel))
	{
		SceneMenu* menuScene = new SceneMenu(manager_);
		manager_.PushuScene(menuScene);
		menuScene->Initialize();
		return true;
	}
}
