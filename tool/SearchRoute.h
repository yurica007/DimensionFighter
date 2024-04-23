#pragma once
#include "../PosData.h"
#include <vector>

class InputState;

class SearchRoute
{
public:
	SearchRoute();
	~SearchRoute() {}

	void Initialize(std::vector<PosData> posData, const int charaNum);
	void Finalize();

	/// <summary>
	/// キャラクターの方向を取得する
	/// </summary>
	/// <returns>キャラクターの方向</returns>
	const Direct GetDirect() { return direct; }

private:
	/// <summary>
	/// 自身から最も近い距離にある対象の配列番号を取得する
	/// </summary>
	/// <param name="charaPos">自分以外のキャラクターの配列番号</param>
	/// <returns>対象の配列番号</returns>
	const PosData GetNearTargetPos(std::vector<PosData> charaPos);

	/// <summary>
	/// 自身から最も近い距離にある対象への方向を取得する
	/// </summary>
	/// <returns>対象への方向</returns>
	const Direct GetNearTargetDirect();

	/// <summary>
	/// 対象までの距離を取得する
	/// </summary>
	/// <param name="target">対象の配列番号</param>
	/// <param name="source">自身の配列番号</param>
	/// <returns>対象までの距離</returns>
	const int GetHeuristic(const PosData target, const PosData source);

	PosData searchCharaPos;
	PosData nearChara;

	Direct direct;
};

