#pragma once
#include "../character/CharaData.h"
#include "../StageData.h"
#include "../PosData.h"
#include <vector>
#include <DxLib.h>

class CharaBase;
class ReuseData;
class InputState;
class Stage;

class CharaManager
{
public:
	CharaManager(Stage& stage);
	~CharaManager() {}

	void Initialize(ReuseData& reuseData);
	void Update(const InputState& inputState, const int time);
	void Draw();
	void Finalize();

	/// <summary>
	/// 全キャラクターの配列番号を取得する
	/// </summary>
	/// <returns>全キャラクターの配列番号</returns>
	const std::vector<PosData> GetCharaIndex();

	/// <summary>
	/// 残りキャラクター数を取得する
	/// </summary>
	/// <returns>残りキャラクター数</returns>
	const int GetRemainCharaNum();

	/// <summary>
	/// 全キャラクターの体力を取得する
	/// </summary>
	/// <returns>全キャラクターの体力</returns>
	const std::vector<int> GetCharaHP();

	const std::vector<bool> GetIsOnStage();

	/// <summary>
	/// 全キャラクターの3次元座標を取得する
	/// </summary>
	/// <returns>全キャラクターの3次元座標</returns>
	const std::vector<VECTOR> GetCharaPos();
protected:
	Stage& stage;

private:
	/// <summary>
	/// キャラクターのデータのを保存する
	/// </summary>
	/// <param name="data">保存するデータ</param>
	/// <param name="num">キャラクターの番号</param>
	void KeepCharaData(const CharaData data, const int num) { characterData[num] = data; }

	std::vector<CharaBase*> chara;

	std::vector<CharaData> characterData;

	int deadCharaNum;
};

