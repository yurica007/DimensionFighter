#include "ScenePreparation.h"
#include "SceneManager.h"
#include "SceneSelectLevel.h"
#include "SceneSelectChara.h"
#include "SceneGame.h"
#include "../tool/InputState.h"
#include "../ReuseData.h"
#include "../BackGround.h"
#include "../tool/Camera.h"
#include "../GameData.h"

namespace
{
	using namespace std;

	enum SelectType
	{
		pcType,
		npcLevel,
		startGame,
		all
	};
}

ScenePreparation::ScenePreparation(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneBase(manager, camera, reuse), 
	backGround(new BackGround()), sizeWidth(0),	sizeHeight(0),
	currentIndex(0), isFade(false)
{
}

void ScenePreparation::Initialize()
{
	backGround->Initialize(SceneType::title);

	const int selectHandle = LoadGraph("data/ui/select.png");
	const int notselectHandle = LoadGraph("data/ui/notselect.png");

	vector<int> frameHandle;
	frameHandle.push_back(selectHandle);
	frameHandle.push_back(notselectHandle);

	vector<MenuHandle> frameHandleData;
	for (int i = 0; i < frameHandle.size(); i++)
	{
		frameHandleData.push_back({ 0, 0, 1.0, 0.0, frameHandle[i]});
	}

	const int selectChara = LoadGraph("data/ui/selectChara.png");
	const int selectLevel = LoadGraph("data/ui/selectLevel.png");
	const int start = LoadGraph("data/ui/startGame.png");
	
	vector<int> textHandle;
	textHandle.push_back(selectChara);
	textHandle.push_back(selectLevel);
	textHandle.push_back(start);

	vector<MenuHandle> textHandleData;
	for (int i = 0; i < textHandle.size(); i++)
	{
		const int posY = 160 + 200 * i;
		textHandleData.push_back({ halfScreenWidth, posY, 1.0, 0.0, textHandle[i] });
	}

	handleData.push_back(frameHandleData);
	handleData.push_back(textHandleData);

	GetGraphSize(textHandle.front(), &sizeWidth, &sizeHeight);
}

void ScenePreparation::Update(const InputState& inputState)
{
	backGround->Update();
	const bool isChangeScene = sceneState == SceneState::popPushScene;
	const bool isPushScene = sceneState == SceneState::pushScne;
	if (!isChangeScene) UpdateFadein(isFade);
	else UpdateFadeout(isFade);

	if (isFade) return;

	camera.SetPosAndVec(SceneType::title);

	if (isPushScene)
	{
		if (currentIndex == SelectType::pcType)
		{
			SceneSelectChara* nextScene = new SceneSelectChara(manager_, camera, reuseData);
			manager_.PushScene(nextScene);
			nextScene->Initialize();
			nextScene->SetBackGroundData(backGround);
			nextScene->SetPrevScene(this);
		}
		else if (currentIndex == SelectType::npcLevel)
		{
			SceneSelectLevel* nextScene = new SceneSelectLevel(manager_, camera, reuseData);
			manager_.PushScene(nextScene);
			nextScene->Initialize();
			nextScene->SetBackGroundData(backGround);
			nextScene->SetPrevScene(this);
		}
	}

	if (isChangeScene)
	{
		vector<int> colorNum;
		reuseData.RewriteColorData(colorNum);

		if (colorNum.size() == 0)
		{
			std::vector<int> charaNum;
			for (int i = 0; i < 4; i++)
			{
				charaNum.push_back(i);
			}
			reuseData.SetCharaColor(charaNum);
		}

		SceneGame* nextScene = new SceneGame(manager_, camera, reuseData);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
		return;
	}

	if (inputState.IsTriggered(InputType::select))
	{
		if (currentIndex == SelectType::startGame) sceneState = SceneState::popPushScene;
		else sceneState = SceneState::pushScne;
	}
	else sceneState = SceneState::donot;

	if (inputState.IsTriggered(InputType::up) && 0 < currentIndex )
	{
		currentIndex--;
	}
	if (inputState.IsTriggered(InputType::down) && currentIndex < SelectType::all - 1)
	{
		currentIndex++;
	}
}

void ScenePreparation::Draw()
{
	backGround->Draw();

	for (int i = 0; i < SelectType::all; i++)
	{
		const MenuHandle text = handleData[1][i];

		if (i == currentIndex)
		{
			const MenuHandle select = handleData[0][0];
			DrawRotaGraph(text.posX, text.posY, select.extRate, 0.0, select.handle, true);
		}
		else
		{
			const MenuHandle notselect = handleData[0][1];
			DrawRotaGraph(text.posX, text.posY, notselect.extRate, 0.0, notselect.handle, true);
		}
		DrawRotaGraph(text.posX, text.posY, text.extRate, text.angle, text.handle, true);
	}

	DrawFadeGraph();
}

void ScenePreparation::Finalize()
{
	backGround->Finalize();
}
