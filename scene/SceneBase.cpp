#include "SceneBase.h"

SceneBase::SceneBase(SceneManager& manager) :
	manager_(manager)
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
