#include "UILife.h"
#include <DxLib.h>

namespace
{
	enum LifeType
	{
		valid,
		invalid
	};
}

UILife::UILife()
{
}

void UILife::Initialize(const std::vector<HandleData> handleData)
{
	lifeHandleData = handleData;
}

void UILife::Draw(const bool isValid)
{
	LifeType lifeType = LifeType::valid;
	if (!isValid) lifeType = LifeType::invalid;

	const HandleData lifeData = lifeHandleData[lifeType];
	DrawRotaGraph(lifeData.posX, lifeData.posY, lifeData.extRate, lifeData.angle, lifeData.handle, true);
}

void UILife::Finalize()
{
}
