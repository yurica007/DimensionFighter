#include "Camera.h"

Camera::Camera() :
	pos({ 0.0f, 0.0f, 0.0f }), target{ 0.0f, 0.0f, 0.0f }
{
}
Camera::~Camera()
{
}

void Camera::Initialize()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 200.0f);

	pos = { 0.0f, 8.0f, -5.0f };
	target = { 0.0f, 0.0f, 0.0f };

	VECTOR lightPos = { 0.0, 8.0f, -5.0f };
	VECTOR lightVector = { 0.0f, -1.0f, 0.3f };
	//lightVector = VNorm(VSub(target, lightPos));
	SetLightPosition(lightPos);
	SetLightDirection(lightVector);

}

void Camera::Update()
{
	SetCameraPositionAndTarget_UpVecY(pos, target);
}

void Camera::Finalaize()
{
}
