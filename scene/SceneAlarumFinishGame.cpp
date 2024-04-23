#include "SceneAlarumFinishGame.h"
#include "SceneManager.h"
#include "../tool/InputState.h"
#include "../tool/Load.h"

namespace
{
	using namespace std;

	const char* const uIFinishFilePath = "data/UIAlarumFinish.csv";
}

SceneAlarumFinishGame::SceneAlarumFinishGame(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneAlarum(manager, camera, reuse)
{
}

void SceneAlarumFinishGame::Initialize()
{
	LoadFile(uIFinishFilePath);

	SceneAlarum::Initialize();
}

void SceneAlarumFinishGame::Update(const InputState& inputState)
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
		manager_.EndScene();
	}

	if (isCancel)
	{
		sceneState = SceneState::popScene;
		manager_.PopScene();
	}

	if (isSelect || isCancel) return;

	SceneAlarum::Update(inputState);
}

void SceneAlarumFinishGame::Draw()
{
	SceneAlarum::Draw();
}

void SceneAlarumFinishGame::Finalize()
{
	SceneAlarum::Finalize();
}