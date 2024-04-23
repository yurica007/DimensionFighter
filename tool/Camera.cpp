#include "Camera.h"

namespace
{
	constexpr float nearCamera = 0.1f;
	constexpr float farCamera = 200.0f;

	constexpr VECTOR sceneCameraPos = { 0.0f, 3.0f, -10.0f };
	constexpr VECTOR sceneTargetPos = { 0.0f, 2.0f, 0.0f };

	constexpr VECTOR sceneLightPos = sceneCameraPos;
	constexpr VECTOR sceneLightVec = { 0.0f, -0.5f, 1.0f };

	constexpr VECTOR sceneGameCameraPos = { 640.0f, 368.0f, -6.0f };
	constexpr VECTOR sceneGameTargetPos = { 640.0f, 360.0f, 0.0f };

	constexpr VECTOR sceneGameLightPos = sceneGameCameraPos;
	constexpr VECTOR sceneGameLightVec = { 0.0f, -1.0f, 0.3f };
}

Camera::Camera() :
	deadCharaNum(0), isZoomin(false), pos(sceneGameCameraPos)
{
}

void Camera::Initialize()
{
	//Z軸を使うための設定
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
	SetUseBackCulling(TRUE);
}

void Camera::SetPosAndVec(const SceneType scene)
{
	VECTOR cameraPos = {};
	VECTOR targetPos = {};
	VECTOR lightPos = {};
	VECTOR lightVector = {};

	if (scene == SceneType::game || scene == SceneType::menuInGame || scene == SceneType::alarum)
	{
		cameraPos = pos;
		targetPos = sceneGameTargetPos;
		lightPos = sceneGameLightPos;
		lightVector = sceneGameLightVec;
	}
	if (scene == SceneType::title || scene == SceneType::result || scene == SceneType::menuEndGame)
	{
		cameraPos = sceneCameraPos;
		targetPos = sceneTargetPos;
		lightPos = sceneLightPos;
		lightVector = sceneLightVec;
	}

	if (scene == SceneType::selectChrara)
	{
		cameraPos = { 640.0f, 363.0f, -5.0f };
		targetPos = { 640.0f, 360.0f, 0.0f };
		lightPos = sceneLightPos;
		lightVector = sceneLightVec;
	}

	//カメラの奥行きを設定
	SetCameraNearFar(nearCamera, farCamera);

	//ライトの設定
	SetLightPosition(lightPos);
	SetLightDirection(lightVector);

	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);
}

void Camera::Update(const int deadNum)
{
	if (deadCharaNum != deadNum)
	{
		deadCharaNum = deadNum;
		
		VECTOR targetPos = sceneGameCameraPos;
		targetPos.y -= static_cast<float>(deadCharaNum) * 0.4f;
		targetPos.z += static_cast<float>(deadCharaNum) * 0.3f;
		pos = targetPos;

		//isZoomin = true;
	}


	if (isZoomin)
	{
		VECTOR targetPos = sceneGameCameraPos;
		targetPos.y -= static_cast<float>(deadCharaNum) * 0.4f;
		targetPos.z += static_cast<float>(deadCharaNum) * 0.3f;
		pos = targetPos;
	}
}

void Camera::Finalaize()
{
}