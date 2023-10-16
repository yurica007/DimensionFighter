#pragma once
#include "SceneBase.h"
#include <string>
#include <vector>
#include <DxLib.h>

using namespace std;

struct Menu
{
	string name = "";
	VECTOR pos;
};

class SceneMenu : public SceneBase
{
public:
	SceneMenu(SceneManager& manager);
	virtual ~SceneMenu();

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	virtual bool IsTransScene(const InputState& inputState) override;
	vector<string> split(const string& input, const char delimiter);

	vector<Menu> menu;
	int currentIndex;
};

