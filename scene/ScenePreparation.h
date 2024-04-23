#pragma once
#include "SceneBase.h"
#include "../ui/UIMenu.h"

class BackGround;

class ScenePreparation : public SceneBase
{
public:
	ScenePreparation(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~ScenePreparation() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	BackGround* backGround;
	std::vector<std::vector<MenuHandle>> handleData;

	int sizeWidth;
	int sizeHeight;

	int currentIndex;
	bool isFade;
};

