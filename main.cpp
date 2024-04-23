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
	//window���[�h�ݒ�
	ChangeWindowMode(windowMode);
	//�E�C���h�E���ݒ�
	SetMainWindowText(titleText);
	//��ʃT�C�Y�̐ݒ�
	SetGraphMode(screenWidth, screenHeight, colorDepth);

	//�G���[���N�����璼���ɏI��
	if (DxLib_Init() == -1) return -1;

	//�_�u���o�b�t�@���[�h
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

		//��ʂ̃N���A
		ClearDrawScreen();

		inputState->Update();

		sceneManager->Update(*inputState);
		
		sceneManager->Draw();

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		//esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE) || sceneManager->IsSceneEnd()) break;

		//fps��60�ɌŒ�
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