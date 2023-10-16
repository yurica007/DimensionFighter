#include "scene/SceneManager.h"
#include "scene/SceneTitle.h"
#include "scene/SceneGame.h"
#include "scene/SceneResult.h"
#include "InputState.h"
#include "Camera.h"
#include "GameData.h"
#include <DxLib.h>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(Data::kWindowMode);
	// ウインドウ名設定
	SetMainWindowText(Data::kTitleText);
	// 画面サイズの設定
	SetGraphMode(Data::kScreenWidth, Data::kScreenHeight, Data::kColorDepth);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
	SetUseBackCulling(TRUE);
	
	InputState* inputState = new InputState();
	Camera* camera = new Camera();
	SceneManager* sceneManager = new SceneManager();

//	SceneTitle* startScene = new SceneTitle(*sceneManager);
	SceneGame* startScene = new SceneGame(*sceneManager);
//	SceneResult* startScene = new SceneResult(*sceneManager);
		
	camera->Initialize();
	sceneManager->PushuScene(startScene);
	sceneManager->Initialize();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		camera->Update();

		inputState->Update();
		sceneManager->Update(*inputState);
				
		sceneManager->Draw();

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE) || sceneManager->IsSceneEnd())	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	camera->Finalaize();
	sceneManager->Finalize();

	delete inputState;
	delete camera;
	delete sceneManager;

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}