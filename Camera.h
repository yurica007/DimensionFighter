// 2016 Takeru Yui All Rights Reserved.
#pragma once
#include "DxLib.h"

class PlayerManager;

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:
	Camera();
	~Camera();

	void Initialize();
	void Update();
	void Finalaize();

	const VECTOR& GetPos() const { return pos; }

private:
	VECTOR pos;
	VECTOR target;
};
