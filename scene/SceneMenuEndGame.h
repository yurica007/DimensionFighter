#pragma once
#include "SceneMenuBase.h"

class UIMenu;

class SceneMenuEndGame : public SceneMenuBase
{
public:
	SceneMenuEndGame(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneMenuEndGame() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

protected:
	UIMenu* UI;
};

