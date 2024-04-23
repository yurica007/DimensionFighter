#include "SceneBase.h"
#include "SceneMenuInGame.h"
#include "SceneManager.h"
#include "SceneAlarumRetryGame.h"
#include "SceneAlarumFinishGame.h"
#include "ScenePreparation.h"
#include "../tool/InputState.h"
#include "../tool/Load.h"
#include "../tool/Camera.h"
#include "../ReuseData.h"
#include "../GameData.h"
#include "../ui/UIMenu.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	const char* const uITextMenuFilePath = "data/UIGameMenuText.csv";

	//X座標
	constexpr int backFramePosX = halfScreenWidth;
	constexpr int menuFramePosX = 0;
	constexpr int cursorPosX = 240;
	constexpr int textPosX = halfScreenWidth;

	//Y座標
	constexpr int backFramePosY = halfScreenHeight;
	constexpr int menuFramePosY = 0;
	constexpr int cursorPosY = 0;
	constexpr int textPosY = 200;

	//前との距離
	constexpr int textDistY = 100;

	enum MenuType
	{
		backText,
		retryText,
		reselect,
		finishText
	};
}

SceneMenuInGame::SceneMenuInGame(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneMenuBase(manager, camera, reuse), UI(new UIMenu(reuse, camera, SceneType::menuInGame))
{
}

void SceneMenuInGame::Initialize()
{
	camera.SetPosAndVec(SceneType::menuInGame);
	
	vector<MenuHandle> frameHandleData;
	{
		vector<int> handle;
		reuseData.RewriteFrameData(handle);

		int posX = 0, posY = 0;
		for (int i = 0; i < handle.size(); i++)
		{
			if (i == FrameType::backFrame) posX = backFramePosX, posY = backFramePosY;
			else posX = menuFramePosX, posY = menuFramePosY;

			frameHandleData.push_back({ posX, posY, 1.0, 0.0, handle[i] });
		}
	}

	vector<MenuHandle> cursorHandleData;
	{
		vector<int> handle;
		reuseData.RewriteCursorData(handle);

		int posX = 0, posY = 0;
		for (int i = 0; i < handle.size(); i++)
		{
			posX = cursorPosX, posY = cursorPosY;
			cursorHandleData.push_back({ posX, posY, 1.5, 0.0, handle[i] });
		}
	}

	Load* load = new Load();
	vector<int> textHandle;
	load->LoadGraphFile(uITextMenuFilePath, textHandle);
	delete load;

	vector<MenuHandle> textHandleData;
	{
		int posX = textPosX, posY = textPosY;
		for (int i = 0; i < textHandle.size(); i++)
		{
			textHandleData.push_back({ posX, posY, 1.0, 0.0, textHandle[i] });
			posY += textDistY;
		}
	}

	currentIndex = -1;
	maxIndex = static_cast<int>(textHandleData.size());

	vector<vector<MenuHandle>> handleData;
	handleData.push_back(frameHandleData);
	handleData.push_back(textHandleData);
	handleData.push_back(cursorHandleData);

	UI->Initialize(handleData, currentIndex);
}

void SceneMenuInGame::Update(const InputState& inputState)
{
	const bool isSelect = inputState.IsTriggered(InputType::select);
	const bool isCancel = inputState.IsTriggered(InputType::menu);

	//つづける
	if (isSelect && currentIndex == MenuType::backText)
	{
		sceneState = SceneState::popScene;
		prevScene->ResetSceneState();
		manager_.PopScene();
	}

	//やりなおす
	if (isSelect && currentIndex == MenuType::retryText)
	{
		sceneState = SceneState::pushScne;
		
		SceneAlarumRetryGame* alarum = new SceneAlarumRetryGame(manager_, camera, reuseData);
		manager_.PushScene(alarum);
		alarum->Initialize();
	}

	//なんいどをえらぶ
	if (isSelect && currentIndex == MenuType::reselect)
	{
		sceneState = SceneState::popPushScene;
		
		ScenePreparation* nextScene = new ScenePreparation(manager_, camera, reuseData);
		manager_.ChangePushScene(nextScene);
		nextScene->Initialize();
	}

	//やめる
	if (isSelect && currentIndex == MenuType::finishText)
	{
		sceneState = SceneState::pushScne;

		SceneAlarumFinishGame* alarum = new SceneAlarumFinishGame(manager_, camera, reuseData);
		manager_.PushScene(alarum);
		alarum->Initialize();
	}

	if (isCancel)
	{
		sceneState = SceneState::popScene;
		prevScene->ResetSceneState();
		manager_.PopScene();
	}

	if (isSelect || isCancel) return;
	else sceneState = SceneState::donot;

	UI->Update(currentIndex);

	if (currentIndex == -1) currentIndex = 0;

	//メニューはループするようにする
	if (inputState.IsTriggered(InputType::up))
	{
		currentIndex = (((currentIndex - 1) + maxIndex) % maxIndex);
	}
	else if (inputState.IsTriggered(InputType::down))
	{
		currentIndex = ((currentIndex + 1) % maxIndex);
	}
}

void SceneMenuInGame::Draw()
{
	UI->Draw(sceneState);
}

void SceneMenuInGame::Finalize()
{
}