#pragma once
#include "SceneBase.h"
#include <vector>

class BackGround;

class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneTitle() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	BackGround* backGround;

	int titleHandle;
	int textHandle;

	float time;
	float exRate;
	float swayRate;

	bool isZoom;
	bool isFade;
};

