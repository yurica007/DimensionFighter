#include "SceneBase.h"

namespace
{
	const int fadeInterval = 60;
}

SceneBase::SceneBase(SceneManager& manager) :
	manager_(manager), fadeTimer(fadeInterval), fadeValue(255)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Initialize()
{
}

void SceneBase::Update(const InputState& input)
{
}

void SceneBase::Draw()
{
}

void SceneBase::Finalize()
{
}

void SceneBase::FadeinUpdate()
{
	fadeValue = static_cast<int>(255 * static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	fadeTimer--;
	if (fadeTimer < 0) fadeTimer = 0;
}

void SceneBase::FadeoutUpdate()
{
	fadeValue = static_cast<int>(255 * static_cast<float>(fadeTimer) / static_cast<float>(fadeInterval));
	fadeTimer++;
	if (fadeInterval < fadeTimer) fadeTimer = fadeInterval;
}
