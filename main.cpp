#include "scene/SceneManager.h"
#include "scene/SceneTitle.h"
#include "scene/SceneSelectLevel.h"
#include "scene/SceneGame.h"
#include "scene/SceneResult.h"
#include "tool/InputState.h"
#include "tool/Camera.h"
#include "GameData.h"
#include "ReuseData.h"
#include "SceneData.h"
#include <DxLib.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//windowモード設定
	ChangeWindowMode(windowMode);
	//ウインドウ名設定
	SetMainWindowText(titleText);
	//画面サイズの設定
	SetGraphMode(screenWidth, screenHeight, colorDepth);

	//エラーが起きたら直ちに終了
	if (DxLib_Init() == -1) return -1;

	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);
	
	InputState* inputState = new InputState();
	Camera* camera = new Camera();
	ReuseData* reuseData = new ReuseData();

	SceneManager* sceneManager = new SceneManager();

	SceneTitle* startScene = new SceneTitle(*sceneManager, *camera, *reuseData);
	//SceneSelectLevel* startScene = new SceneSelectLevel(*sceneManager, *camera, *reuseData);
	//SceneGame* startScene = new SceneGame(*sceneManager, *camera, *reuseData);
	//SceneResult* startScene = new SceneResult(*sceneManager, *camera, *reuseData);

	camera->Initialize();
	reuseData->Initialize();

	sceneManager->PushScene(startScene);
	sceneManager->Initialize();

	while (ProcessMessage() == 0)
	{
		const LONGLONG  time = GetNowHiPerformanceCount();

		//画面のクリア
		ClearDrawScreen();

		inputState->Update();

		sceneManager->Update(*inputState);
		
		sceneManager->Draw();

		//裏画面を表画面を入れ替える
		ScreenFlip();

		//escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE) || sceneManager->IsSceneEnd()) break;

		//fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667) {}
	}

	camera->Finalaize();
	reuseData->Finalize();
	sceneManager->Finalize();

	delete inputState;
	delete camera;
	delete reuseData;
	delete sceneManager;

	DxLib_End();
	return 0;
}