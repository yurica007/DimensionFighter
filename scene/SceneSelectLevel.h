#pragma once
#include "SceneMenuBase.h"
#include <fstream>
#include <vector>

class BackGround;
class UIMenu;

class SceneSelectLevel : public SceneMenuBase
{
public:
	SceneSelectLevel(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneSelectLevel() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

	void SetBackGroundData(BackGround* ground) { backGround = ground; }
private:	
	BackGround* backGround;
	UIMenu* UI;

	std::vector<int> frameHandle;
	std::vector<int> textHandle;

	int halfIndex;
};

