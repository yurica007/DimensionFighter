#include "SceneBase.h"
#include "../GameData.h"
#include <DxLib.h>

namespace
{
	constexpr int fadeInterval = 60;
	constexpr int fadeMax = 255;
}

SceneBase::SceneBase(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	manager_(manager), camera(camera), reuseData(reuse), sceneState(SceneState::donot),
	fadeTimer(fadeInterval), fadeValue(fadeMax), graph(-1)
{
	graph = LoadGraph("data/backGraph/colorBlue.png");
}

void SceneBase::UpdateFadein(bool& isFade)
{
	isFade = true;
	fadeValue = static_cast<int>(fadeMax * static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));

	if (--fadeTimer < 0)
	{
		fadeTimer = 0;
		isFade = false;
	}
}

void SceneBase::UpdateFadeout(bool& isFade)
{
	isFade = true;
	fadeValue = static_cast<int>(fadeMax * static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));

	if (fadeInterval < ++fadeTimer)
	{
		fadeTimer = fadeInterval;
		isFade = false;
	}
}

void SceneBase::DrawFadeGraph()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue);
	DrawRotaGraph(halfScreenWidth, halfScreenHeight, 1.0, 0.0, graph, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
