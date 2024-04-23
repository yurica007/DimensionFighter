#pragma once
#include "SceneAlarum.h"

class InputState;

class SceneAlarumFinishGame : public SceneAlarum
{
public:
	SceneAlarumFinishGame(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneAlarumFinishGame() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;
};

