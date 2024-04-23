#include "UICharaType.h"
#include "../GameData.h"
#include "../ReuseData.h"

namespace
{
	using namespace std;

	constexpr int rankPosX = 160;
	constexpr int distX = 320;
	constexpr int typePosY = 500;
}

UICharaType::UICharaType(ReuseData& reuse) :
	reuseData(reuse)
{
}

void UICharaType::Initialize()
{
}

void UICharaType::Update(const vector<VECTOR> charaPos)
{
	vector<int> colorNum;
	reuseData.RewriteColorData(colorNum);

	vector<int> typeHandle;
	reuseData.RewriteTypeData(typeHandle);

	for (int i = 0; i < colorNum.size(); i++)
	{
		const VECTOR pos = ConvWorldPosToScreenPos(charaPos[i]);

		const int posX = static_cast<int>(pos.x);
		const int posY = static_cast<int>(pos.y);

		typeHandleData.push_back({ posX, posY, 1.0, 0.0, typeHandle[colorNum[i]] });
	}
}

void UICharaType::Draw()
{
	for (int i = 0; i < typeHandleData.size(); i++)
	{
		const HandleData type = typeHandleData[i];

		DrawRotaGraph(type.posX, type.posY, type.extRate, type.angle, type.handle, true);
	}
}

void UICharaType::Finalize()
{
}
