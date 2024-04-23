#pragma once
#include "SceneAlarum.h"

class InputState;

class SceneAlarumRetryGame : public SceneAlarum
{
public:
	SceneAlarumRetryGame(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneAlarumRetryGame() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;
};

