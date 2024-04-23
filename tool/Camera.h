#pragma once
#include "../SceneData.h"
#include <DxLib.h>

class Camera
{
public:
	Camera();
	~Camera() {}

	void Initialize();
	void SetPosAndVec(const SceneType scene);
	void Update(const int deadNum);
	void Finalaize();

private:
	int deadCharaNum;
	bool isZoomin;

	VECTOR pos;
};
