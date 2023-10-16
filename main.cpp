#include "scene/SceneManager.h"
#include "scene/SceneTitle.h"
#include "scene/SceneGame.h"
#include "scene/SceneResult.h"
#include "InputState.h"
#include "Camera.h"
#include "GameData.h"
#include <DxLib.h>

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Data::kWindowMode);
	// �E�C���h�E���ݒ�
	SetMainWindowText(Data::kTitleText);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Data::kScreenWidth, Data::kScreenHeight, Data::kColorDepth);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �_�u���o�b�t�@���[�h
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
		// ��ʂ̃N���A
		ClearDrawScreen();

		camera->Update();

		inputState->Update();
		sceneManager->Update(*inputState);
				
		sceneManager->Draw();

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE) || sceneManager->IsSceneEnd())	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	camera->Finalaize();
	sceneManager->Finalize();

	delete inputState;
	delete camera;
	delete sceneManager;

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}