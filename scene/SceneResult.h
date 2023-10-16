#pragma once
#include "SceneBase.h"

class SceneResult :	public SceneBase
{
public:
	SceneResult(SceneManager& manager);
	virtual ~SceneResult();

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	virtual bool IsTransScene(const InputState& inputState) override;

	int currentTime;
};

