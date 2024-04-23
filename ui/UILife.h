#pragma once
#include "../SceneData.h"
#include "../HandleData.h"
#include <vector>

class UILife
{
public:
	UILife();
	~UILife() {}

	void Initialize(const std::vector<HandleData> handleData);
	void Draw(const bool isValid);
	void Finalize();

private:
	std::vector<HandleData> lifeHandleData;
};

