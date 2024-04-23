#include "SceneAlarumRetryGame.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "../tool/InputState.h"
#include "../tool/Load.h"

namespace
{
	using namespace std;

	const char* const uIRetryFilePath = "data/UIAlarumRetry.csv";
}

SceneAlarumRetryGame::SceneAlarumRetryGame(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneAlarum(manager, camera, reuse)
{
}

void SceneAlarumRetryGame::Initialize()
{
	LoadFile(uIRetryFilePath);

	SceneAlarum::Initialize();
}

void SceneAlarumRetryGame::Update(const InputState& inputState)
{
	const bool isSelect = inputState.IsTriggered(InputType::select);
	const bool isCancel = inputState.IsTriggered(InputType::cancel);

	const int selectIndex = currentIndex - 1;

	//ƒLƒƒƒ“ƒZƒ‹
	if (isSelect && selectIndex == MenuAlarumType::disagree)
	{
		sceneState = SceneState::popScene;
		manager_.PopScene();
	}

	//‚¯‚Á‚Ä‚¢
	if (isSelect && selectIndex == MenuAlarumType::agree)
	{
		sceneState = SceneState::popPushScene;

		SceneGame* nextScene = new SceneGame(manager_, camera, reuseData);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
	}

	if (isCancel)
	{
		sceneState = SceneState::popScene;
		manager_.PopScene();
	}

	if (isSelect || isCancel) return;

	SceneAlarum::Update(inputState);
}

void SceneAlarumRetryGame::Draw()
{
	SceneAlarum::Draw();
}

void SceneAlarumRetryGame::Finalize()
{
	SceneAlarum::Finalize();
}