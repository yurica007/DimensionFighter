#pragma once
#include "CharaBase.h"

class PC : public CharaBase
{
public:
	PC(Stage& stage);
	virtual ~PC() {}

	virtual void Initialize(const std::vector<std::string> charaData, const int number, std::vector<PosData> currentIndex) override;
	virtual void Update(const InputState& inputState, const int time, const std::vector<PosData> currentIndex) override;
	virtual void Draw() override;
	virtual void Finalize() override;
};

