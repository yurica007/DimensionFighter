#pragma once
#include "UIBase.h"
#include <DxLib.h>

class UILife : public UIBase
{
public:
	UILife();
	virtual ~UILife() {}

	virtual void Initialize(const VECTOR pos);
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();

	void SetCurrentLife(const int playerNum, const int life);

private:
	int currentLife;
	int player;
	int gaugeHandle;
	int lifeHandle;

	RECT gaugePos = { 0, 0, 0, 0 };
};

