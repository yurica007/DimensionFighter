#include "UIMenu.h"
#include "../GameData.h"
#include "../ReuseData.h"
#include "../tool/Camera.h"
#include "../Easing.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	constexpr float zoomAnimTimeMax = 45.0f;
	constexpr float turnAnimTimeMax = 80.0f;
	constexpr float shakeAnimTimeMax = 20.0f;

	constexpr float turnAngleMax = 360.0f;
	constexpr float shakeAngleMax = 5.0f;

	constexpr int gaussPixelSize = 16;
	constexpr int gaussParamMax = 1400;

	enum MenuType
	{
		frame,
		text,
		cursor
	};
}

UIMenu::UIMenu(ReuseData& reuse, Camera& camera, const SceneType scene) :
	reuseData(reuse), camera(camera), sceneType(scene), easing(new Easing()),
	prevMenuIndex(0), zoomAnimTime(0.0f), turnAnimTime(0.0), isZoom(true),
	screenGraph(-1), menuGraph(-1), sizeX(0), sizeY(0)
{	
}

void UIMenu::Initialize(const vector<vector<MenuHandle>> handle, const int initIndex)
{
	handleData = handle;
	prevMenuIndex = initIndex;

	screenGraph = MakeScreen(screenWidth, screenHeight, true);

	const int graph = handleData[MenuType::frame][FrameType::selectFrame].handle;
	GetGraphSize(graph, &sizeX, &sizeY);
	sizeX += handleData[MenuType::cursor][CursorType::left].posX;

	menuGraph = MakeScreen(sizeX, sizeY, true);
}

void UIMenu::Update(const int newMenuIndex)
{
	camera.SetPosAndVec(sceneType);

	if (zoomAnimTime < zoomAnimTimeMax) zoomAnimTime++;
	else
	{
		isZoom = false;

		if (prevMenuIndex != newMenuIndex)
		{
			DecisionCursor();
			prevMenuIndex = newMenuIndex;
		}

		if (turnAnimTimeMax < turnAnimTime++) turnAnimTime = 0.0f;
	}
}

void UIMenu::Draw(const SceneState sceneState)
{
	if (!isZoom) DrawSelectMenuGraph();

	SetDrawScreen(screenGraph);
	ClearDrawScreen();

	camera.SetPosAndVec(sceneType);
	DrawNotSelectMenuAndBackGound();

	if (!isZoom)
	{
		const MenuHandle selectFrame = handleData[MenuType::frame][FrameType::selectFrame];
		const MenuHandle text = handleData[MenuType::text][prevMenuIndex];
		int posX = selectFrame.posX + text.posX, posY = selectFrame.posY + text.posY;

		const float sineRate = easing->InSine(turnAnimTime, shakeAnimTimeMax, shakeAngleMax, 0.0f);
		const float angle = easing->ToRadianF(sineRate - shakeAngleMax);
		DrawRotaGraph(posX, posY, 1.0, angle, menuGraph, true);
	}

	SetDrawScreen(DX_SCREEN_BACK);
	if (sceneState == SceneState::pushScne)  GraphFilter(screenGraph, DX_GRAPH_FILTER_GAUSS, gaussPixelSize, gaussParamMax);
	const float extRate = easing->OutBounce(zoomAnimTime, zoomAnimTimeMax, 1.0f, 0.0f);
	DrawRotaGraph(halfScreenWidth, halfScreenHeight, extRate, 0.0, screenGraph, true);
}

void UIMenu::Finalize()
{
}

void UIMenu::DecisionCursor()
{
	cursorData.clear();
	const int cursorNum = static_cast<int>(handleData[MenuType::cursor].size()) - 1;
	while (cursorData.size() != 2)
	{
		const int num = GetRand(cursorNum - 1);
		cursorData.push_back(handleData[MenuType::cursor][num]);
	}

	cursorData[1].posX *= -1;
}

void UIMenu::DrawSelectMenuGraph()
{
	SetDrawScreen(menuGraph);
	ClearDrawScreen();

	const MenuHandle selectFrame = handleData[MenuType::frame][FrameType::selectFrame];
	const int framePosX = sizeX / 2, framePosY = sizeY / 2;
	DrawRotaGraph(framePosX, framePosY, selectFrame.extRate, selectFrame.angle, selectFrame.handle, true);

	float rate = easing->Linear(turnAnimTime, turnAnimTimeMax, turnAngleMax, 0.0f);

	//ÉÅÉjÉÖÅ[ÇÃè¸ÇËÇï`âÊ
	for (int j = 0; j < cursorData.size(); j++)
	{
		const MenuHandle cursor = cursorData[j];
		const int posX = cursor.posX + framePosX, posY = cursor.posY + framePosY;

		rate *= -1.0f;
		const float angle = easing->ToRadianF(rate);
		DrawRotaGraph(posX, posY, cursor.extRate, angle, cursor.handle, true);
	}
	const MenuHandle text = handleData[MenuType::text][prevMenuIndex];
	DrawRotaGraph(framePosX, framePosY, text.extRate, text.angle, text.handle, true);
}

void UIMenu::DrawNotSelectMenuAndBackGound()
{
	const vector<MenuHandle> menuFrame = handleData[MenuType::frame];
	//ÉÅÉjÉÖÅ[UIògÇÃï`âÊ
	{
		const MenuHandle backFrame = menuFrame[FrameType::backFrame];
		DrawRotaGraph(backFrame.posX, backFrame.posY, backFrame.extRate, backFrame.angle, backFrame.handle, true);
	}

	const vector<MenuHandle> menuText = handleData[MenuType::text];
	for (int i = 0; i < menuText.size(); i++)
	{
		const MenuHandle text = menuText[i];

		if (i == 0 && sceneType != SceneType::menuInGame && sceneType != SceneType::menuEndGame)
		{
			DrawRotaGraph(text.posX, text.posY, text.extRate, text.angle, text.handle, true);
			continue;
		}
		if (i == prevMenuIndex) continue;
		//ëIëÇ≥ÇÍÇƒÇ¢Ç»Ç¢ÉÅÉjÉÖÅ[ÇÃï`âÊ
		const MenuHandle notSelectFrame = menuFrame[FrameType::notselectFrame];
		const int framePosX = text.posX + notSelectFrame.posX;
		const int framePosY = text.posY + notSelectFrame.posY;
		DrawRotaGraph(framePosX, framePosY, notSelectFrame.extRate, notSelectFrame.angle, notSelectFrame.handle, true);

		DrawRotaGraph(text.posX, text.posY, text.extRate, text.angle, text.handle, true);
	}
}
