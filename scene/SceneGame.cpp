#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneMenuInGame.h"
#include "SceneResult.h"
#include "../Stage.h"
#include "../character/CharaManager.h"
#include "../ui/UIUpdateTime.h"
#include "../ui/UIClearTime.h"
#include "../ui/UILifeManager.h"
#include "../ui/UICharaType.h"
#include "../tool/InputState.h"
#include "../tool/Camera.h"
#include "../tool/Load.h"
#include "../ReuseData.h"
#include "../GameData.h"
#include "../BackGround.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	constexpr int timeUpper = 60;

	constexpr int gaussPixelSize = 16;
	constexpr int gaussParamMax = 1400;
}

SceneGame::SceneGame(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneBase(manager, camera, reuse),
	backGround(new BackGround()),
	stage(new Stage()), chara(new CharaManager(*stage)),
	updateTime(new UIUpdateTime(reuse, SceneType::game)),
	clearTime(new UIClearTime(reuse, SceneType::game)),
	lifes(new UILifeManager(reuse)),
	charaType(new UICharaType(reuse)),
	startCount(charaNumAll), countDown(charaNumAll), screenGraph(-1), isFade(false)
{
}

void SceneGame::Initialize()
{
	backGround->Initialize(SceneType::game);

	chara->Initialize(reuseData);

	stage->Initialize(reuseData);

	updateTime->Initialize();
	clearTime->Initialize();
	lifes->Initialize(chara->GetCharaHP());

	charaType->Initialize();

	rank.resize(charaNumAll);

	screenGraph = MakeScreen(screenWidth, screenHeight, true);
}

void SceneGame::Update(const InputState& inputState)
{
	backGround->Update();
	const bool isChangeScene = sceneState == SceneState::popPushScene;
	const bool isPushScene = sceneState == SceneState::pushScne;
	if (!isChangeScene) UpdateFadein(isFade);
	else UpdateFadeout(isFade);

	if (isFade) return;

	if (isChangeScene)
	{
		Load* load = new Load();
		load->SaveScore(currentTime);
		delete load;

		const vector<int> ranking = rank;

		SceneResult* nextScene = new SceneResult(manager_, camera, reuseData);
		manager_.ChangeScene(nextScene);
		nextScene->SetRanking(ranking);
		nextScene->Initialize();
		return;
	}

	if (isPushScene)
	{
		SceneMenuInGame* menuScene = new SceneMenuInGame(manager_, camera, reuseData);
		manager_.PushScene(menuScene);
		menuScene->Initialize();
		menuScene->SetPrevScene(this);
		return;
	}

	if (IsRankChara())
	{
		sceneState = SceneState::popPushScene;
	}
	else if (inputState.IsTriggered(InputType::menu))
	{
		sceneState = SceneState::pushScne;
	}
	else sceneState = SceneState::donot;

	camera.Update(4 - chara->GetRemainCharaNum());

	chara->Update(inputState, countDown);

	stage->Update();

	updateTime->Update(currentTime, countDown);
	clearTime->Update();
	lifes->Update(chara->GetCharaHP(), chara->GetIsOnStage());

	charaType->Update(chara->GetCharaPos());

	UpdateTime();

	reuseData.SetNowTime(currentTime);
}

void SceneGame::Draw()
{
	SetDrawScreen(screenGraph);
	ClearDrawScreen();

	camera.SetPosAndVec(SceneType::game);

	chara->Draw();
	stage->Draw();

	updateTime->Draw();
	clearTime->Draw();
	lifes->Draw(screenGraph);
	//charaType->Draw();

	if (sceneState == SceneState::pushScne) GraphFilter(screenGraph, DX_GRAPH_FILTER_GAUSS, gaussPixelSize, gaussParamMax);
	SetDrawScreen(DX_SCREEN_BACK);
	backGround->Draw();
	DrawRotaGraph(halfScreenWidth, halfScreenHeight, 1.0, 0.0, screenGraph, true);
	DrawFadeGraph();
}

void SceneGame::Finalize()
{
	backGround->Finalize();
	delete backGround;

	chara->Finalize();
	delete chara;

	stage->Finalize();
	delete stage;

	updateTime->Finalize();
	delete updateTime;

	clearTime->Finalize();
	delete clearTime;

	lifes->Finalize();
	delete lifes;
}

void SceneGame::UpdateTime()
{
	//始まるまでのカウント更新
	if (0 <= countDown)
	{
		startCount++;
		if (startCount == timeUpper)
		{
			startCount = 0;
			countDown--;
		}
	}
	//始まってからの経過時間の更新
	else
	{
		currentTime.miliSeconds++;
		if (currentTime.miliSeconds == timeUpper)
		{
			currentTime.miliSeconds = 0;
			currentTime.seconds++;
		}

		if (currentTime.seconds == timeUpper)
		{
			currentTime.seconds = 0;
			currentTime.minutes++;
		}
	}
}

bool SceneGame::IsRankChara()
{
	int remainPlayerNum = chara->GetRemainCharaNum();
	const vector<int> hp = chara->GetCharaHP();

	for (int i = 0; i < charaNumAll; i++)
	{
		if (hp[i] != 0 || rank[i] != 0) continue;
		rank[i] = remainPlayerNum + 1;
	}

	for (int i = 0; i < charaNumAll; i++)
	{
		if (remainPlayerNum != 1) continue;
		if (rank[i] == 0) rank[i] = 1;
	}

	if (remainPlayerNum <= 1) return true;

	return false;
}