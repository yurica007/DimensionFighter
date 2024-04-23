#pragma once
#include "../PosData.h"
#include "../StageData.h"
#include "../character/CharaData.h"
#include <vector>

struct PosIndex
{
	PosData pos;
	int score = 0;
};

class AI
{
public:
	AI();
	~AI() {}

	void Initialize(const int stageWidthMax, const VECTOR charaPos);
	void Update(const std::vector<bool> collisionStage, const VECTOR charaPos, VECTOR& rotate, std::vector<PosData> indexAll, const int mine);
	void Finalize();

	const VECTOR GetMoveVec() { return moveVec; }
	const float GetDropRotateY() { return dropRotateY; }

	/*void UpdateStageCost(const std::vector<PosData> posData, const std::vector<BlockType> fallType);

	//移動後の配列座標の取得
	const std::vector<PosData> GetMovedIndex() { return movedIndex; }

	const Act GetAct() { return action; }*/

private:
	void DecisionMoveVecAndDist(VECTOR& rotate);
	void UpdateMoveAct(const std::vector<bool> collisionStage, const VECTOR charaPos);

	void UpdateWaitAct();

	void DecisionDropVec(VECTOR& rotate);
	void UpdateDropAct();

	/*//キャラ座標から全体への距離を測定
	const int GetHeuristic(const PosData target, const PosData source);
	//測定した距離をもとにキャラの座標を11とする1~10の値へ変換
	const int NormalizeHeuristic(const int heuristic);

	/// <summary>
	/// 新規の方向を取得する
	/// </summary>
	/// <param name="prevDirect">以前の方向</param>
	/// <param name="collisionStage">ステージの当たり判定</param>
	/// <returns>新規の方向</returns>
	const Direct GetNewDirect(const Direct prevDirect, const std::vector<bool> collisionStage);

	const VECTOR GetMovedPos(const VECTOR charaPos, Direct& direct);

	void UpdateNotAct();
	void UpdateMoveAct(const std::vector<bool> collisionStage, const VECTOR charaPos, VECTOR& rotate);
	void UpdateDropAct(VECTOR& rotate, std::vector<PosData> indexAll, const int mine);

	void UpdateMoveing(const int num, const PosData pos);
	void SearchSeaftyArea(const int num, const PosData pos);

	const bool GetRandom(const int probability);*/

	std::vector<PosIndex> stage;
	std::vector<PosData> player;

	std::vector<PosData> movedIndex;

	int stageMaxWidth;
	int stageWidth;

	bool isAvoid;

	ActType action;
	bool isAction;

	int timer;

	Direct direct;
	VECTOR moveVec;
	float dropRotateY;
	VECTOR movePrevPos;

	bool isAct;
	ActType act;

	float dist;
	Direct moveDir;
	Direct dropDir;
	VECTOR maxVec;

	int time;
};

