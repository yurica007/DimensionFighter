#pragma once
#include "../StageData.h"
#include "../character/CharaData.h"
#include "../PosData.h"
#include <DxLib.h>
#include <vector>
#include <string>

class Collision3D;
class InputState;
class Stage;
class Easing;

class CharaBase
{
public:
	CharaBase(Stage& stage);
	virtual ~CharaBase() {}

	virtual void Initialize(const std::vector<std::string> charaData, const int number, std::vector<PosData> currentIndex);
	virtual void Update(const InputState& inputState, const int time, const std::vector<PosData> currentIndex) = 0;
	virtual void Draw();
	virtual void Finalize();

	/// <summary>
	/// 現在いる配列番号を取得する
	/// </summary>
	/// <returns>配列番号</returns>
	const PosData GetCurrentIndex() { return ConvertPosToIndex(); }

	/// <summary>
	/// キャラクターの情報を取得する
	/// </summary>
	/// <returns>キャラクター情報</returns>
	const CharaData GetData() { return chara; }

	/// <summary>
	/// 他キャラクターの配列番号を受取る
	/// </summary>
	/// <param name="atherPos">全キャラクターの配列番号</param>
	/// <param name="mineNum">自分の番号</param>
	const void SetIndexAll(const std::vector<PosData> atherPos) { indexAll = atherPos; }

	/// <summary>
	/// 他キャラクターの3次元座標を受取る
	/// </summary>
	/// <param name="pos">全キャラクターの座標</param>
	/// <param name="rad">全キャラクターの半径</param>
	const void SetPosAll(const std::vector<VECTOR> pos) { posAll = pos; }

protected:
	/// <summary>
	/// キャラクターがステージ上か判定する
	/// </summary>
	/// <param name="fallData">ブロックの落下データ</param>
	/// <returns>true：ステージ上にいる false：ステージ上にいない</returns>
	const bool IsOnStage();

	/// <summary>
	/// キャラクターの落下処理
	/// </summary>
	/// <param name="currentIndex">現在の配列座標</param>
	const void UpdateFall(const std::vector<PosData> currentIndex);

	/// <summary>
	/// キャラクターが移動できるか判定する
	/// </summary>
	/// <param name="immotileTime">硬直時間</param>
	/// <returns>true：移動できる false：移動できない</returns>
	const bool IsMove(const int immotileTime);

	Stage& stage;
	CharaData chara;
	Collision3D* collision;

	std::vector<PosData> indexAll;
	int charaNum;

	std::vector<VECTOR> posAll;

	std::vector<bool> stageCollision;

	int immotileTime;

private:
	/// <summary>
	/// キャラクターをスポーンする
	/// </summary>
	/// <param name="spawnIndexAll">スポーン前の配列座標</param>
	const void SpawnRandom(const std::vector<PosData> spawnIndexAll);

	/// <summary>
	/// 3次元座標から配列番号へ変換する
	/// </summary>
	/// <returns>変換後の配列番号</returns>
	const PosData ConvertPosToIndex();
	
	Easing* easing;
	float time;

	int stageMaxWidth;
};

