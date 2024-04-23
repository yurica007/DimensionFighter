#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <vector>
#include <string>

class SceneMenuBase : public SceneBase
{
public:
	SceneMenuBase(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneMenuBase() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

protected:
	int maxIndex;
	int currentIndex;
};

