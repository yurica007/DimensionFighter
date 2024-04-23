#include "SceneMenuBase.h"
#include "../ReuseData.h"

SceneMenuBase::SceneMenuBase(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneBase(manager, camera, reuse), maxIndex(0), currentIndex(0)
{	
}

void SceneMenuBase::Initialize()
{
}

void SceneMenuBase::Update(const InputState& inputState)
{
}

void SceneMenuBase::Draw()
{

}

void SceneMenuBase::Finalize()
{
}