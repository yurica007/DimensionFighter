#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "../InputState.h"
#include <DxLib.h>

SceneTitle::SceneTitle(SceneManager& manager) :
	SceneBase(manager), currentTime(0)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Initialize()
{
	currentTime = GetNowCount();
}

void SceneTitle::Update(const InputState& inputState)
{
	if (IsTransScene(inputState)) return;
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "title", 0xfffff);
}

void SceneTitle::Finalize()
{
}

bool SceneTitle::IsTransScene(const InputState& inputState)
{
	if (inputState.IsTriggered(InputType::debug))
	{
		SceneGame* nextScene = new SceneGame(manager_);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
		return true;
	}
}
