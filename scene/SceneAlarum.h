#pragma once
#include "SceneMenuBase.h"
#include <vector>

class UIMenu;

class SceneAlarum : public SceneMenuBase
{
public:
	SceneAlarum(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneAlarum() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

protected:
	enum MenuAlarumType
	{
		disagree,
		agree
	};

	void LoadFile(const std::string filePath);

private:
	UIMenu* UI;

	std::vector<int> frameHandle;
	std::vector<int> textHandle;
};

