#pragma once
#include "SceneBase.h"
#include "../ui/UIMenu.h"
#include <DxLib.h>
#include <vector>
#include <string>

class InputState;
class BackGround;

class SceneSelectChara : public SceneBase
{
public:
	SceneSelectChara(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneSelectChara() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	void SetBackGroundData(BackGround* ground) { backGround = ground; }

private:
	BackGround* backGround;

	int model1;
	int model2;
	std::vector<int> texture;

	std::vector<MenuHandle> textData;
	std::vector<MenuHandle> gaugeData;
	std::vector<MenuHandle> gaugeTextData;
	std::vector<std::vector<MenuHandle>> imageData;

	int frame;
	int arrow;

	int index;
	int indexMax;

	float time;
	float timer;
	float scrollOut;
	float scrollIn;

	bool isScroll;
	bool isScrollLeft;

	VECTOR pos;

	int screenGraph;
};

