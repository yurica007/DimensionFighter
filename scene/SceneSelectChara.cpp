#include "SceneSelectChara.h"
#include "SceneManager.h"
#include "../tool/InputState.h"
#include "../tool/Camera.h"
#include "../GameData.h"
#include "../ReuseData.h"
#include "../BackGround.h"
#include "../Easing.h"
#include "../character/CharaData.h"

namespace
{
	using namespace std;

	enum StateType
	{
		speed,
		power
	};
}

SceneSelectChara::SceneSelectChara(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneBase(manager, camera, reuse),
	backGround(nullptr),
	model1(-1), model2(-1), frame(-1), arrow(-1), index(0), indexMax(0), time(0.0f), timer(0.0f), scrollIn(0.0f), scrollOut(0.0f),
	isScroll(false), isScrollLeft(false), pos({ 0.0f, 0.0f, 0.0f }), screenGraph(-1)
{
}

void SceneSelectChara::Initialize()
{
	indexMax = charaNumAll;
	
	vector<vector<string>> charaData;
	for (int i = 0; i < indexMax; i++)
	{
		vector<string> data;
		reuseData.RewriteCharaData(data, i);
		charaData.push_back(data);
		texture.push_back(LoadGraph(data[1].c_str()));
	}

	model1 = MV1LoadModel(charaData[index][0].c_str());
	model2 = MV1DuplicateModel(model1);

	vector<int> colorNum;
	reuseData.RewriteColorData(colorNum);

	if (colorNum.size() != 0) index = colorNum.front();

	MV1SetTextureGraphHandle(model1, 0, LoadGraph(charaData[index][1].c_str()), false);

	screenGraph = MakeScreen(screenWidth, screenHeight, true);

	{
		frame = LoadGraph("data/ui/charaState/backFrame.png");
		arrow = LoadGraph("data/ui/arrow.png");

		const int speedText = LoadGraph("data/ui/charaState/speed.png");
		const int stiffenText = LoadGraph("data/ui/charaState/stiffen.png");
		const int powerText = LoadGraph("data/ui/charaState/power.png");
		textData.push_back({ 950, 80, 1.0, 0.0, speedText });
		textData.push_back({ 950, 180, 1.0, 0.0, stiffenText });
		textData.push_back({ 950, 300, 1.0, 0.0, powerText });

		const int onequarter = LoadGraph("data/ui/charaState/onequarter.png");
		const int oneHalf = LoadGraph("data/ui/charaState/oneHalf.png");
		const int threequarter = LoadGraph("data/ui/charaState/threequarter.png");
		const int max = LoadGraph("data/ui/charaState/max.png");

		gaugeData.push_back({ 100, 35, 1.0, 0.0, onequarter });
		gaugeData.push_back({ 100, 35, 1.0, 0.0, oneHalf });
		gaugeData.push_back({ 100, 35, 1.0, 0.0, threequarter });
		gaugeData.push_back({ 100, 35, 1.0, 0.0, max });
	}

	{
		const int slowText = LoadGraph("data/ui/charaState/slow.png");
		const int quickText = LoadGraph("data/ui/charaState/quick.png");
		const int shortText = LoadGraph("data/ui/charaState/short.png");
		const int longText = LoadGraph("data/ui/charaState/long.png");

		gaugeTextData.push_back({ -50, 60, 1.0, 0.0, slowText });
		gaugeTextData.push_back({ 250, 60, 1.0, 0.0, quickText });

		gaugeTextData.push_back({ -50, 60, 1.0, 0.0, shortText });
		gaugeTextData.push_back({ 250, 60, 1.0, 0.0, longText });
	}

	{
		const int offRed = LoadGraph("data/ui/image/offRed.png");
		const int onRed = LoadGraph("data/ui/image/onRed.png");
		std::vector<MenuHandle> red;
		red.push_back({ 1050, 420, 1.0, 0.0, offRed });
		red.push_back({ 1050, 420, 1.0, 0.0, onRed });

		const int offGreen = LoadGraph("data/ui/image/offGreen.png");
		const int onGreen = LoadGraph("data/ui/image/onGreen.png");
		std::vector<MenuHandle> green;
		green.push_back({ 1050, 420, 1.0, 0.0, offGreen });
		green.push_back({ 1050, 420, 1.0, 0.0, onGreen });

		const int offBlue = LoadGraph("data/ui/image/offBlue.png");
		const int onBlue = LoadGraph("data/ui/image/onBlue.png");
		std::vector<MenuHandle> blue;
		blue.push_back({ 1050, 420, 1.0, 0.0, offBlue });
		blue.push_back({ 1050, 420, 1.0, 0.0, onBlue });

		const int offYellow = LoadGraph("data/ui/image/offYellow.png");
		const int onYellow = LoadGraph("data/ui/image/onYellow.png");
		std::vector<MenuHandle> yellow;
		yellow.push_back({ 1050, 420, 1.0, 0.0, offYellow });
		yellow.push_back({ 1050, 420, 1.0, 0.0, onYellow });

		imageData.push_back(red);
		imageData.push_back(green);
		imageData.push_back(blue);
		imageData.push_back(yellow);
	}
}

void SceneSelectChara::Update(const InputState& inputState)
{
	backGround->Update();
	const bool isPopScene = sceneState == SceneState::popScene;

	if (isPopScene)
	{
		std::vector<int> charaNum;
		charaNum.push_back(index);
		for (int i = 0; i < indexMax; i++)
		{
			if (i == index) continue;
			charaNum.push_back(i);
		}

		reuseData.SetCharaColor(charaNum);

		prevScene->ResetSceneState();
		manager_.PopScene();
		return;
	}

	if (inputState.IsTriggered(InputType::select))
	{
		sceneState = SceneState::popScene;
	}
	else sceneState = SceneState::donot;

	camera.SetPosAndVec(SceneType::selectChrara);

	if (inputState.IsTriggered(InputType::left) && !isScroll)
	{
		index = ((index + 1) % indexMax);
		isScroll = true;
		isScrollLeft = true;
	}
	if (inputState.IsTriggered(InputType::right) && !isScroll)
	{
		index = (((index - 1) + indexMax) % indexMax);
		isScroll = true;
		isScrollLeft = false;
	}

	if (isScroll)
	{
		if (60 < time++)
		{
			time = 0.0f;
			isScroll = false;

			MV1SetTextureGraphHandle(model1, 0, texture[index], false);
		}

		Easing* easing = new Easing();
		const float inQuad = easing->InQuad(time, 60.0f, 20.0f, 0.0f);
		const float outQuad = easing->OutQuad(time, 60.0f, 20.0f, 0.0f);
		const float rate = easing->InSine(time, 60.0f, 1.0f, 0.1f);
		delete easing;

		if (isScrollLeft)
		{
			scrollIn = -outQuad * rate;
			scrollOut = -inQuad * rate;
		}
		else
		{
			scrollIn = outQuad * rate;
			scrollOut = inQuad * rate;
		}
		MV1SetTextureGraphHandle(model2, 0, texture[index], false);


		float posX2 = 0.0f;
		if (isScrollLeft) posX2 = 661.0f + scrollIn;
		else posX2 = 619.0f + scrollIn;
		MV1SetPosition(model2, { posX2, 360.0f, 0.0f });
	}
	else
	{
		scrollIn = 0.0f;
		scrollOut = 0.0f;
	}

	const float posX1 = 640.0f + scrollOut;
	MV1SetPosition(model1, { posX1, 360.0f, 0.0f });

	pos = ConvWorldPosToScreenPos({ posX1, 360.0f, 0.0f });
}

void SceneSelectChara::Draw()
{
	SetDrawScreen(screenGraph);
	ClearDrawScreen();
	
	camera.SetPosAndVec(SceneType::selectChrara);

	MV1DrawModel(model1);
	MV1DrawModel(model2);

	if (!isScroll && VSize(pos) != 0.0f)
	{
		DrawRotaGraph(1050, 300, 2.0, 0.0, frame, true);

		for (int i = 0; i < textData.size(); i++)
		{
			const MenuHandle text = textData[i];
			DrawRotaGraph(text.posX, text.posY, text.extRate, text.angle, text.handle, true);
		}

		{
			int posX = textData[StateType::speed].posX +gaugeTextData[0].posX;
			int posY = textData[StateType::speed].posY + gaugeTextData[0].posY;
			DrawRotaGraph(posX, posY, gaugeTextData[0].extRate, gaugeTextData[0].angle, gaugeTextData[0].handle, true);

			posX = textData[StateType::speed].posX +gaugeTextData[1].posX;
			posY = textData[StateType::speed].posY + gaugeTextData[1].posY;
			DrawRotaGraph(posX, posY, gaugeTextData[1].extRate, gaugeTextData[1].angle, gaugeTextData[1].handle, true);
		}

		{
			int posX = textData[StateType::power].posX +gaugeTextData[2].posX;
			int posY = textData[StateType::power].posY + gaugeTextData[2].posY;
			DrawRotaGraph(posX, posY, gaugeTextData[2].extRate, gaugeTextData[2].angle, gaugeTextData[2].handle, true);

			posX = textData[StateType::power].posX + gaugeTextData[3].posX;
			posY = textData[StateType::power].posY + gaugeTextData[3].posY;
			DrawRotaGraph(posX, posY, gaugeTextData[3].extRate, gaugeTextData[3].angle, gaugeTextData[3].handle, true);
		}

		const int speedGauge = (((index - 1) + indexMax) % indexMax);
		//‚Í‚â‚³
		{
			const MenuHandle gauge = gaugeData[speedGauge];

			const int posX = gauge.posX + textData[StateType::speed].posX;
			const int posY = gauge.posY + textData[StateType::speed].posY;
			DrawRotaGraph(posX, posY, gauge.extRate, gauge.angle, gauge.handle, true);
		}

		//‚¿‚©‚ç
		{
			const int powerGauge = (indexMax - 1) - speedGauge;
			const MenuHandle gauge = gaugeData[powerGauge];

			const int posX = gauge.posX + textData[StateType::power].posX;
			const int posY = gauge.posY + textData[StateType::power].posY;
			DrawRotaGraph(posX, posY, gauge.extRate, gauge.angle, gauge.handle, true);
		}

		int num = 0;
		if (30 < timer) num = 1;
		const MenuHandle image = imageData[index][num];
		DrawRotaGraph(image.posX, image.posY, image.extRate, image.angle, image.handle, true);
	}

	if (60 < timer++) timer = 0.0f;
	Easing* easing = new Easing();
	const float shake = easing->InSine(timer, 15.0f, 10.0f, -10.0f);
	delete easing;

	DrawRotaGraph(50 + static_cast<int>(shake), halfScreenHeight, 1.0, 0.0, arrow, true, true);
	DrawRotaGraph(screenWidth - 50 - static_cast<int>(shake), halfScreenHeight, 1.0, 0.0, arrow, true);

	SetDrawScreen(DX_SCREEN_BACK);
	backGround->Draw();
	DrawRotaGraph(halfScreenWidth, halfScreenHeight, 1.0, 0.0, screenGraph, true);
//	DrawRotaGraph(320, halfScreenHeight, 0.5, 0.0, screenGraph, true);
}

void SceneSelectChara::Finalize()
{
}
