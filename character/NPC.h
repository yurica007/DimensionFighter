#pragma once
#include "CharaBase.h"

class AI;

class NPC : public CharaBase
{
public:
	NPC(Stage& stage);
	virtual ~NPC() {}

	virtual void Initialize(const std::vector<std::string> charaData, const int number, std::vector<PosData> currentIndex) override;
	virtual void Update(const InputState& inputState, const int time, const std::vector<PosData> currentIndex) override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	AI* ai;
};

