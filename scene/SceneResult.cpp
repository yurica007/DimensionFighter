#include "SceneResult.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "../InputState.h"
#include <DxLib.h>

SceneResult::SceneResult(SceneManager& manager) :
	SceneBase(manager), currentTime(0)
{
}

SceneResult::~SceneResult()
{
}

void SceneResult::Initialize()
{
	currentTime = GetNowCount();
}

void SceneResult::Update(const InputState& inputState)
{
	if (IsTransScene(inputState)) return;
}

void SceneResult::Draw()
{
	DrawString(0, 0, "result", 0xfffff);
}

void SceneResult::Finalize()
{
}

bool SceneResult::IsTransScene(const InputState& inputState)
{
	if (inputState.IsTriggered(InputType::debug))
	{
		SceneTitle* nextScene = new SceneTitle(manager_);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
		return true;
	}
}
