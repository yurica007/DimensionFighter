#include "SceneSelect.h"
#include "SceneManager.h"
#include "ScenePreparation.h"
#include "../BackGround.h"
#include "../tool/InputState.h"
#include "../tool/Camera.h"
#include "../tool/Load.h"
#include "../Easing.h"
#include "../GameData.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	enum UIType
	{
		frame,
		text,
		cursor
	};

	enum MenuType
	{
		start,
		practice,
		config,
		end,
		all
	};
}

SceneSelect::SceneSelect(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneBase(manager, camera, reuse),
	backGround(nullptr), sizeWidth(0),	sizeHeight(0), keepOut(-1),
	prevIndex(0), currentIndex(0), fps(0),
	time(0.0f), quad(0.0f), isFade(false)
{
}

void SceneSelect::Initialize()
{
	const int selectHandle = LoadGraph("data/ui/select.png");
	const int notselectHandle = LoadGraph("data/ui/notselect.png");

	const int startHandle = LoadGraph("data/ui/start.png");
	const int finishHandle = LoadGraph("data/ui/finish.png");

	Load* load = new Load();
	load->LoadGraphFile("data/UICursor.csv", cursor);
	delete load;

	keepOut = LoadGraph("data/ui/line_keepout.png");

	vector<int> frameHandle;
	frameHandle.push_back(selectHandle);
	frameHandle.push_back(notselectHandle);

	vector<MenuHandle> frameHandleData;
	for (int i = 0; i < frameHandle.size(); i++)
	{
		frameHandleData.push_back({ 0, 0, 0.75, 0.0, frameHandle[i] });
	}

	vector<int> textHandle;
	textHandle.push_back(startHandle);
	textHandle.push_back(-1);
	textHandle.push_back(-1);
	textHandle.push_back(finishHandle);

	vector<MenuHandle> textHandleData;
	for (int i = 0; i < textHandle.size(); i++)
	{
		const int posY = 144 + 144 * i;
		textHandleData.push_back({ halfScreenWidth, posY, 1.0, 0.0, textHandle[i] });
	}

	handleData.push_back(frameHandleData);
	handleData.push_back(textHandleData);

	GetGraphSize(textHandle.front(), &sizeWidth, &sizeHeight);
}

void SceneSelect::Update(const InputState& inputState)
{
	backGround->Update();
	const bool isEndScene = sceneState == SceneState::popScene;
	const bool isChangeScne = sceneState == SceneState::popPushScene;
	if (!isEndScene && !isChangeScne) UpdateFadein(isFade);
	else UpdateFadeout(isFade);

	if (isFade) return;

	if (isEndScene)
	{
		manager_.EndScene();
		return;
	}

	if (isChangeScne)
	{
		ScenePreparation* nextScene = new ScenePreparation(manager_, camera, reuseData);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
		return;
	}

	if (inputState.IsTriggered(InputType::select))
	{
		ChangeScene();
		return;
	}
	else sceneState = SceneState::donot;

	camera.SetPosAndVec(SceneType::selectLevel);

	prevIndex = currentIndex;

	if (inputState.IsTriggered(InputType::up))
	{
		currentIndex = (((currentIndex - 1) + MenuType::all) % MenuType::all);
	}
	if (inputState.IsTriggered(InputType::down))
	{
		currentIndex = ((currentIndex + 1) % MenuType::all);
	}

	if (cursor.size() - 1 < ++fps) fps = 0;

	if (60.0f < ++time) time = 0.0f;

	Easing* easing = new Easing();
	quad = easing->InSine(time, 15.0f, 10.0f, 0.0f);
	delete easing;
}

void SceneSelect::Draw()
{
	backGround->Draw();

	for (int i = 0; i < MenuType::all; i++)
	{
		const MenuHandle text = handleData[UIType::text][i];

		if (i == currentIndex)
		{
			const MenuHandle select = handleData[UIType::frame][0];
			DrawRotaGraph(text.posX, text.posY, select.extRate, 0.0, select.handle, true);
		}
		else
		{
			const MenuHandle notselect = handleData[UIType::frame][1];
			DrawRotaGraph(text.posX, text.posY, notselect.extRate, 0.0, notselect.handle, true);
		}

		if (text.handle != -1) DrawRotaGraph(text.posX, text.posY, text.extRate, text.angle, text.handle, true);
		else DrawRotaGraph(text.posX, text.posY, 0.75, 0.0, keepOut, true);
	}

	DrawFadeGraph();
}

void SceneSelect::Finalize()
{
	backGround->Finalize();
	delete backGround;
}

void SceneSelect::ChangeScene()
{
	if (currentIndex == MenuType::start)
	{
		sceneState = SceneState::popPushScene;	
	}

	if (currentIndex == MenuType::config)
	{
	}

	if (currentIndex == MenuType::practice)
	{
	}

	if (currentIndex == MenuType::end)
	{
		sceneState = SceneState::popScene;
	}
}
