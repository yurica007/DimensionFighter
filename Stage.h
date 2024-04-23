#pragma once
#include "StageData.h"
#include "PosData.h"
#include "character/CharaData.h"
#include <vector>
#include <DxLib.h>

struct BlockData
{
	int handle = -1;
	VECTOR pos = { 0.0f, 0.0f, 0.0f };

	int texture = -1;

	bool isPrepareFall = false;
	bool isFall = false;
	bool isFalling = false;
	bool isNone = false;
	bool isDelete = false;

	int charaNum = -1;
	int changeTexTimer = 0;
	int fallDelayTimer = 0;
};

class ReuseData;

class Stage
{
public:
	Stage();
	~Stage() {}

	void Initialize(ReuseData& reuse);
	void Update();
	void Draw();
	void Finalize();

	const int GetStageSize() { return widthSize; }

	void PrevBlock(const float rotateY, const PosData posIndex, const CharaPower charaType, const int charaNum);

	const std::vector<BlockIndexData> GetBlockData();

	/// <summary>
	/// 死んだキャラクター数を受取る
	/// </summary>
	/// <param name="deadCharaNum">死んだキャラクター数</param>
	const void SetDeadCharaNum(const int deadCharaNum) { deadNum = deadCharaNum; }

private:
	/// <summary>
	/// 必要なブロックデータを読込む
	/// </summary>
	void LoadBlockData(ReuseData& reuseData);

	/// <summary>
	/// ブロックデータを初期化する
	/// </summary>
	void InitializeBlock();

	/// <summary>
	/// 死んだキャラクター数に応じてブロックを狭める
	/// </summary>
	const void NarrowBlock();

	/// <summary>
	/// 狭めるべきブロックか判定する
	/// </summary>
	/// <param name="index">配列番号</param>
	/// <returns>true：狭める false：狭めない</returns>
	const bool IsNarrowBlock(const int index);

	/// <summary>
	/// ブロックの落下時にテクスチャを変える
	/// </summary>
	const void ChangeBlockTexture();

	/// <summary>
	/// 変えるテクスチャを決定する
	/// </summary>
	/// <param name="isNone">狭めるブロックか否か</param>
	/// <param name="index">配列番号</param>
	void DecideTexture(const bool isNone, const int index);

	/// <summary>
	/// ブロックを落下させる
	/// </summary>
	const void FallBlock();
		
	/// <summary>
	/// ブロックの落下が終わった後の処理
	/// </summary>
	const void UpdateBlock();

	std::vector<BlockData> block;
	std::vector<int> baseTextureHandle;

	std::vector<int> colorTextureHandle;

	int deadNum;
	int massNum;

	int narrowIndex;
	int widthSize;

	int modelHandle;
	VECTOR modelScale;
};

