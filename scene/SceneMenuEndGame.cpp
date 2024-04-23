#include "SceneMenuEndGame.h"
#include "SceneManager.h"
#include "ScenePreparation.h"
#include "SceneGame.h"
#include "../tool/InputState.h"
#include "../tool/Camera.h"
#include "../tool/Load.h"
#include "../ReuseData.h"
#include "../GameData.h"
#include "../ui/UIMenu.h"
#include <DxLib.h>
#include <fstream>

namespace
{
	using namespace std;

	const char* const uITextMenuFilePath = "data/UIResultMenuText.csv";

	//X���W
	constexpr int backFramePosX = halfScreenWidth;
	constexpr int menuFramePosX = 0;
	constexpr int cursorPosX = 240;
	constexpr int textPosX = halfScreenWidth;

	//Y���W
	constexpr int backFramePosY = halfScreenHeight;
	constexpr int menuFramePosY = 0;
	constexpr int cursorPosY = 0;
	constexpr int textPosY = 260;

	//�O�Ƃ̋���
	constexpr int textDistY = 100;

	enum MenuType
	{
		selectText,
		againText,
		endText
	};
}

SceneMenuEndGame::SceneMenuEndGame(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneMenuBase(manager, camera, reuse), UI(new UIMenu(reuse, camera, SceneType::menuEndGame))
{
}

void SceneMenuEndGame::Initialize()
{
	camera.SetPosAndVec(SceneType::menuEndGame);
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

	//�O���t�B�b�N�̓Ǎ���
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

void SceneMenuEndGame::Update(const InputState& inputState)
{
	const bool isSelect = inputState.IsTriggered(InputType::select);

	//�Ȃ񂢂ǂ������
	if (isSelect && currentIndex == MenuType::selectText)
	{
		sceneState = SceneState::pushScne;

		ScenePreparation* nextScene = new ScenePreparation(manager_, camera, reuseData);
		manager_.ChangePushScene(nextScene);
		nextScene->Initialize();
	}

	//����������
	if (isSelect && currentIndex == MenuType::againText)
	{
		sceneState = SceneState::popPushScene;

		SceneGame* nextScene = new SceneGame(manager_, camera, reuseData);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
	}

	//�����
	if (isSelect && currentIndex == MenuType::endText)
	{
		sceneState = SceneState::popScene;
		manager_.EndScene();
	}

	if (isSelect) return;
	else sceneState = SceneState::donot;

	UI->Update(currentIndex);

	if (currentIndex == -1) currentIndex = 0;

	//���j���[�̓��[�v����悤�ɂ���
	if (inputState.IsTriggered(InputType::up))
	{
		currentIndex = (((currentIndex - 1) + maxIndex) % maxIndex);
	}
	else if (inputState.IsTriggered(InputType::down))
	{
		currentIndex = ((currentIndex + 1) % maxIndex);
	}
}

void SceneMenuEndGame::Draw()
{
	UI->Draw(sceneState);
}

void SceneMenuEndGame::Finalize()
{
}