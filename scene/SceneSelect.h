#pragma once
#include "SceneBase.h"
#include "../ui/UIMenu.h"
#include <vector>

class BackGround;
class InputState;

class SceneSelect : public SceneBase
{
public:
	SceneSelect(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneSelect() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	void SetBackGroundData(BackGround* ground) { backGround = ground; }

private:
	void ChangeScene();

	BackGround* backGround;
	std::vector<std::vector<MenuHandle>> handleData;
	std::vector<int> cursor;

	int sizeWidth;
	int sizeHeight;

	int keepOut;

	int prevIndex;
	int currentIndex;

	int fps;

	float time;
	float quad;

	bool isFade;
};

