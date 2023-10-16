#pragma once
#include "SceneBase.h"

class Player;
class Stage;

struct Time
{
	int minutes = 0;
	int seconds = 0;
};

class SceneGame : public SceneBase
{
public:
	SceneGame(SceneManager& manager);
	virtual ~SceneGame();

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	virtual bool IsTransScene(const InputState& inputState) override;

	Stage* stage;
	Player* player;

	Time currentTime;
	int prevTime;
};

