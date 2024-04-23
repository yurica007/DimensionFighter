#include "SceneTitle.h"
#include "SceneManager.h"
#include "ScenePreparation.h"
#include "../tool/InputState.h"
#include "../tool/Camera.h"
#include "../BackGround.h"
#include "../Easing.h"
#include "../GameData.h"
#include <DxLib.h>

namespace
{
	using namespace std;
	
	constexpr int gaussPixelSize = 16;
	constexpr int gaussParamMax = 1400;
}

SceneTitle::SceneTitle(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneBase(manager, camera, reuse),
	backGround(new BackGround()), titleHandle(-1), textHandle(-1), time(0.0f), exRate(0.0f),
	swayRate(0.0f),	isZoom(true), isFade(false)
{
}

void SceneTitle::Initialize()
{
	backGround->Initialize(SceneType::title);

	titleHandle = LoadGraph("data/ui/title.png");
	textHandle = LoadGraph("data/ui/text.png");
}

void SceneTitle::Update(const InputState& inputState)
{
	backGround->Update();
	const bool isPushScene = sceneState == SceneState::pushScne;
	if (!isPushScene) UpdateFadein(isFade);

	if (isFade) return;

	if (isPushScene)
	{
		ScenePreparation* nextScene = new ScenePreparation(manager_, camera, reuseData);
		manager_.PushScene(nextScene);
		nextScene->Initialize();
		return;
	}

	if (inputState.IsTriggered(InputType::select))
	{
		if (!isZoom) sceneState = SceneState::pushScne;
		else
		{
			isZoom = false;
			time = 0.0f;
			exRate = 1.0f;
		}
	}
	else sceneState = SceneState::donot;

	camera.SetPosAndVec(SceneType::title);

	time++;
	if (isZoom)
	{
		if (time < 60.0f)
		{
			Easing* easing = new Easing();
			exRate = easing->OutBounce(time, 60.0f, 1.0f, 0.0f);
			delete easing;
		}
		else isZoom = false;
	}
	else
	{
		if (60.0f < time) time = 0;
		Easing* easing = new Easing();
		swayRate = easing->InSine(time, 15.0f, 10.0f, 0.0f);
		delete easing;
	}
}

void SceneTitle::Draw()
{
	backGround->Draw();
	DrawRotaGraph(halfScreenWidth, halfScreenHeight, exRate, 0.0, titleHandle, true);
	
	if (!isZoom) DrawRotaGraph(halfScreenWidth, halfScreenHeight + 200 + static_cast<int>(swayRate), 1.0, 0.0, textHandle, true);
	DrawFadeGraph();
}

void SceneTitle::Finalize()
{
	backGround->Finalize();
	delete backGround;
}