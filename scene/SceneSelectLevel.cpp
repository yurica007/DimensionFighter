#include "SceneSelectLevel.h"
#include "SceneManager.h"
#include "../tool/InputState.h"
#include "../tool/Camera.h"
#include "../tool/Load.h"
#include "../ReuseData.h"
#include "../BackGround.h"
#include "../GameData.h"
#include "../ui/UIMenu.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	const char* const uILevelFilePath = "data/UILevel.csv";

	constexpr double turnMaxRate = 4.0;

	//XÀ•W
	constexpr int backFramePosX = halfScreenWidth;
	constexpr int menuFramePosX = 0;
	constexpr int cursorStarkPosX = 100;
	constexpr int textPosX = halfScreenWidth;

	//YÀ•W
	constexpr int backFramePosY = halfScreenHeight;
	constexpr int menuFramePosY = 0;
	constexpr int cursorPosY = 0;
	constexpr int titleTextPosY = 220;
	constexpr int textPosY = 400;
	
	//‘O‚Æ‚Ì‹——£
	constexpr int textDistX = 110;
	constexpr int textDistY = 45;
}

SceneSelectLevel::SceneSelectLevel(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneMenuBase(manager, camera, reuse),
	backGround(nullptr),
	UI(new UIMenu(reuse, camera, SceneType::selectLevel)),
	halfIndex(0)
{
}

void SceneSelectLevel::Initialize()
{
	Load* load = new Load();
	load->LoadGraphAndTexGraphtFile(uILevelFilePath, frameHandle, textHandle);
	delete load;

	vector<MenuHandle> frameHandleData;
	{
		int posX = 0, posY = 0;
		for (int i = 0; i < frameHandle.size(); i++)
		{
			if (i == FrameType::backFrame) posX = backFramePosX, posY = backFramePosY;
			else posX = menuFramePosX, posY = menuFramePosY;

			frameHandleData.push_back({ posX, posY, 1.0, 0.0, frameHandle[i] });
		}
	}

	vector<MenuHandle> cursorHandleData;
	{
		vector<int> handle;
		reuseData.RewriteCursorData(handle);

		int posX = 0, posY = 0;
		for (int i = 0; i < handle.size(); i++)
		{
			posX = cursorStarkPosX, posY = cursorPosY;
			cursorHandleData.push_back({ posX, posY, 1.0, 0.0, handle[i] });
		}
	}

	vector<MenuHandle> textHandleData;
	{
		int posX = textPosX, posY = titleTextPosY;
		for (int i = 0; i < textHandle.size(); i++)
		{
			textHandleData.push_back({ posX, posY, 1.0, 0.0, textHandle[i] });

			posX = textPosX, posY = textPosY;

			if (i % 2 == 0) posX -= textDistX;
			else posX += textDistX;

			if (i / 2 == 0) posY -= textDistY;
			else posY += textDistY;
		}
	}

	currentIndex = 0;
	maxIndex = static_cast<int>(textHandleData.size()) - 1;
	halfIndex = maxIndex / 2;

	vector<vector<MenuHandle>> handleData;
	handleData.push_back(frameHandleData);
	handleData.push_back(textHandleData);
	handleData.push_back(cursorHandleData);

	UI->Initialize(handleData, currentIndex);
}

void SceneSelectLevel::Update(const InputState& inputState)
{
	backGround->Update();
	const bool isPopScene = sceneState == SceneState::popScene;
	
	if (isPopScene)
	{
		prevScene->ResetSceneState();
		manager_.PopScene();
		return;
	}

	if (inputState.IsTriggered(InputType::select))
	{
		sceneState = SceneState::popScene;
	}
	else sceneState = SceneState::donot;

	camera.SetPosAndVec(SceneType::selectLevel);
	
	UI->Update(currentIndex);

	if (currentIndex == 0) currentIndex = 1;

	if (inputState.IsTriggered(InputType::left) && 1 < currentIndex)
	{
		currentIndex--;
	}
	if (inputState.IsTriggered(InputType::right) && currentIndex < maxIndex)
	{
		currentIndex++;
	}
	if (inputState.IsTriggered(InputType::up) && 0 < currentIndex / (halfIndex + 1))
	{
		currentIndex -= halfIndex;
	}
	if (inputState.IsTriggered(InputType::down) && currentIndex / (halfIndex + 1) < 1)
	{
		currentIndex += halfIndex;
	}
}

void SceneSelectLevel::Draw()
{
	backGround->Draw();
	UI->Draw(sceneState);
}

void SceneSelectLevel::Finalize()
{
}