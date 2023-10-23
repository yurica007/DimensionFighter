#pragma once
#include "CharacterBase.h"

class Character3 : public CharacterBase
{
public:
	Character3();
	virtual ~Character3() {}

	virtual void Initialize(const RECT stageMaxSize, const int stageWidthSize) override;
	virtual void Update(const InputState& inputState, const vector<vector<int>> fallData, const int time) override;
	virtual void Draw() override;
	virtual void Finalize() override;
};

