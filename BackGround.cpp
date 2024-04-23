#include "BackGround.h"
#include "GameData.h"
#include <DxLib.h>

namespace
{
	constexpr float gameScrollSpeed = 0.5f;
	constexpr float nomralScrollSpeed = 1.0f;
}

BackGround::BackGround() :
	sceneType(SceneType::title), scrollType(Scroll::left),
	backGroundNum(0), colorNum(0), sizeX(0), sizeY(0), scrollX(0.0f), scrollY(0.0f),
	scroll(nomralScrollSpeed)
{
}

void BackGround::Initialize(const SceneType type)
{
	sceneType = type;

	//�摜�f�[�^
	{
		const int handle1 = LoadGraph("data/backGraph/babble.png");
		const int handle2 = LoadGraph("data/backGraph/diamond.png");
		const int handle3 = LoadGraph("data/backGraph/stripe.png");
		const int handle4 = LoadGraph("data/backGraph/star.png");
		const int handle5 = LoadGraph("data/backGraph/square.png");

		backGroundData.push_back(handle1);
		backGroundData.push_back(handle2);
		backGroundData.push_back(handle3);
		backGroundData.push_back(handle4);
		backGroundData.push_back(handle5);
	}

	//�F�f�[�^
	{
		const int handle1 = LoadGraph("data/backGraph/colorRed.png");
		const int handle2 = LoadGraph("data/backGraph/colorGreen.png");
		const int handle3 = LoadGraph("data/backGraph/colorBlue.png");

		colorData.push_back(handle1);
		colorData.push_back(handle2);
		colorData.push_back(handle3);
	}

	//�V�[�����Ƃɔw�i�摜�����肷��
	backGroundNum = GetRand(static_cast<int>(backGroundData.size()) - 1);
	
	//�w�i�F�����肷��
	colorNum = GetRand(static_cast<int>(colorData.size()) - 1);

	//�X�N���[��������������肷��
	scrollType = static_cast<Scroll>(GetRand(static_cast<int>(Scroll::all) - 1));

	//�Q�[���V�[���Ƃ��̑��̃V�[���ŃX�N���[�����x��ς���
	if (sceneType == SceneType::game) scroll = gameScrollSpeed;
	else scroll = nomralScrollSpeed;
	
	GetGraphSize(backGroundData[backGroundNum], &sizeX, &sizeY);
}

void BackGround::Update()
{
	//�X�N���[����������ɉ����ď�����ύX����
	if (scrollType == Scroll::left) scrollX += scroll;
	else if (scrollType == Scroll::right) scrollX -= scroll;
	else if (scrollType == Scroll::up) scrollY += scroll;
	else if (scrollType == Scroll::down) scrollY -= scroll;

	//�摜1�����X�N���[�����I�����猳�̏ꏊ�ɖ߂�
	if (sizeX < scrollX && scrollType == Scroll::left) scrollX = 0.0f;
	if (scrollX < -sizeX && scrollType == Scroll::right) scrollX = 0.0f;
	if (sizeY < scrollY && scrollType == Scroll::up) scrollY = 0.0f;
	if (scrollY < -sizeY && scrollType == Scroll::down) scrollY = 0.0f;
}

void BackGround::Draw()
{
	float frontPosX = static_cast<float>(halfScreenWidth) - scrollX;
	float backPosX = frontPosX;
	if (scrollType == Scroll::left) backPosX += sizeX;
	if (scrollType == Scroll::right) backPosX -= sizeX;

	float frontPosY = static_cast<float>(halfScreenHeight) - scrollY;
	float backPosY = frontPosY;
	if (scrollType == Scroll::up) backPosY += sizeY;
	if (scrollType == Scroll::down) backPosY -= sizeY;


	if (sceneType == SceneType::game)
	{
		DrawRotaGraphF(frontPosX, frontPosY, 1.0f, 0.0f, backGroundData[backGroundNum], true);
		DrawRotaGraphF(backPosX, backPosY, 1.0f, 0.0f, backGroundData[backGroundNum], true);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawRotaGraphF(frontPosX, frontPosY, 1.0f, 0.0f, colorData[colorNum], true);
		DrawRotaGraphF(backPosX, backPosY, 1.0f, 0.0f, colorData[colorNum], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		DrawRotaGraphF(frontPosX, frontPosY, 1.0f, 0.0f, backGroundData[backGroundNum], true);
		DrawRotaGraphF(backPosX, backPosY, 1.0f, 0.0f, backGroundData[backGroundNum], true);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawRotaGraphF(frontPosX, frontPosY, 1.0f, 0.0f, colorData[colorNum], true);
		DrawRotaGraphF(backPosX, backPosY, 1.0f, 0.0f, colorData[colorNum], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void BackGround::Finalize()
{
}
