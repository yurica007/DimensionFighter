#pragma once
#include "CharacterBase.h"

class Character1 : public CharacterBase
{
public:
	Character1();
	virtual ~Character1() {}

	virtual void Initialize(const RECT stageMaxSize, const int stageWidthSize) override;
	virtual void Update(const InputState& inputState, const vector<vector<int>> fallData, const int time) override;
	virtual void Draw() override;
	virtual void Finalize() override;
};

