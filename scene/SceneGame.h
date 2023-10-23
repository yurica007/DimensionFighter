#pragma once
#include "SceneBase.h"
#include "../TimeDatah.h"
#include <vector>

class Stage;
class CharacterManager;
class UITime;
class UILife;

using namespace std;

class SceneGame : public SceneBase
{
public:
	SceneGame(SceneManager& manager);
	virtual ~SceneGame();

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	void SetTranceTime(const int currentTime);

private:
	virtual bool IsTransScene(const InputState& inputState) override;

	Stage* stage;
	CharacterManager* character;
	UITime* time;
	vector<UILife*> life;

	int prevTime;
	Time currentTime;

	int startCount;
	int countDown;
};

