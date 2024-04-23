#include "SceneAlarum.h"
#include "../GameData.h"
#include "../tool/InputState.h"
#include "../tool/Camera.h"
#include "../tool/Load.h"
#include "../character/CharaData.h"
#include "../ui/UIMenu.h"
#include "../ReuseData.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	const char* const uIRetryFilePath = "data/AlarumRetryUIData.csv";

	//Xç¿ïW
	constexpr int backFramePosX = halfScreenWidth;
	constexpr int menuFramePosX = 0;
	constexpr int cursorStarkPosX = 120;
	constexpr int textPosX = halfScreenWidth;

	//Yç¿ïW
	constexpr int backFramePosY = halfScreenHeight;
	constexpr int menuFramePosY = 0;
	constexpr int cursorPosY = 0;
	constexpr int messageTextPosY = 320;
	constexpr int textPosY = 530;

	constexpr int textDist = 200;
}

SceneAlarum::SceneAlarum(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneMenuBase(manager, camera, reuse), UI(new UIMenu(reuse, camera, SceneType::alarum))
{
}

void SceneAlarum::Initialize()
{
	camera.SetPosAndVec(SceneType::alarum);

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
	int posX = textPosX, posY = messageTextPosY;
	for (int i = 0; i < textHandle.size(); i++)
	{
		textHandleData.push_back({ posX, posY, 1.0, 0.0, textHandle[i] });

		posX = textPosX, posY = textPosY;

		if (i % 2 == 0) posX -= textDist;
		else posX += textDist;
	}

	currentIndex = 0;
	maxIndex = static_cast<int>(textHandle.size()) - 1;

	vector<vector<MenuHandle>> handleData;
	handleData.push_back(frameHandleData);
	handleData.push_back(textHandleData);
	handleData.push_back(cursorHandleData);

	UI->Initialize(handleData, currentIndex);
}

void SceneAlarum::Update(const InputState& inputState)
{
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
}

void SceneAlarum::Draw()
{
	UI->Draw(sceneState);
}

void SceneAlarum::Finalize()
{
}

void SceneAlarum::LoadFile(const string filePath)
{
	Load* load = new Load();
	load->LoadGraphAndTexGraphtFile(filePath, frameHandle, textHandle);
	delete load;
}
