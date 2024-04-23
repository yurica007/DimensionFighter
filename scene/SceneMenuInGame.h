#pragma once
#include "SceneMenuBase.h"

class UIMenu;

class SceneMenuInGame : public SceneMenuBase
{
public:
	SceneMenuInGame(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneMenuInGame() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

protected:
	UIMenu* UI;
};

