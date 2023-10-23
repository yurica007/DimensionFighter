#include "UILife.h"

UILife::UILife() :
	currentLife(0), player(0),
	gaugeHandle(-1), lifeHandle(-1)
{
}

void UILife::Initialize(const VECTOR pos)
{
	uiPos = pos;
	gaugeHandle = LoadGraph("data/square2.png");
	lifeHandle = LoadGraph("data/ellipse.png");
}

void UILife::Update()
{
}

void UILife::Draw()
{
	DrawRotaGraphF(uiPos.x, uiPos.y, 3.0, 0.0, gaugeHandle, false);
	{
		const auto iconPosWidth = uiPos.x - 80;
		const auto iconPosHeight = uiPos.y - 5;
		DrawFormatString(iconPosWidth, iconPosHeight, 0x000000, "P%d", player);
	}

	for (int i = 0; i < currentLife; i++)
	{
		const auto posWidth = uiPos.x + (i - 2) * 25 + 15;
		DrawRotaGraphF(posWidth, uiPos.y, 2.0, 0.0, lifeHandle, false);
	}

	//{
	//	DrawLine(240, 50, 410, 50, 0xffffff);
	//	DrawLine(325, 0, 325, 720, 0xffffff);
	//	DrawLine(450, 50, 620, 50, 0xffffff);
	//	DrawLine(535, 0, 535, 720, 0xffffff);
	//	DrawLine(660, 50, 830, 50, 0xffffff);
	//	DrawLine(745, 0, 745, 720, 0xffffff);
	//	DrawLine(870, 50, 1040, 50, 0xffffff);
	//	DrawLine(955, 0, 955, 720, 0xffffff);
	//}
}

void UILife::Finalize()
{
}

void UILife::SetCurrentLife(const int playerNum, const int life)
{
	player = playerNum;
	currentLife = life;
}
