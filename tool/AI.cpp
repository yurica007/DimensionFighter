#include "AI.h"
#include"../PosData.h"
#include "SearchRoute.h"
#include <DxLib.h>
#include <algorithm>
#include <random>

namespace
{
	using namespace std;

	constexpr int charaPosCost = 11;
}

AI::AI() :
	stageMaxWidth(0), stageWidth(0), isAvoid(false),
	action(ActType::move), isAction(true), timer(0), direct(Direct::left),
	moveVec({ 0.0f, 0.0f, 0.0f }), dropRotateY(-1.0f), movePrevPos({ 0.0f, 0.0f, 0.0f }), isAct(false), act(ActType::wait), dist(0.0f), moveDir(Direct::left), dropDir(Direct::left), maxVec({0.0f, 0.0f, 0.0f}), time(0)
{
}

void AI::Initialize(const int stageWidthMax, const VECTOR charaPos)
{
	stageWidth = stageWidthMax;
	stageMaxWidth = stageWidth * stageWidth;
	stage.resize(stageMaxWidth);

	movePrevPos = charaPos;
}

void AI::Update(const vector<bool> collisionStage, const VECTOR charaPos, VECTOR& rotate, vector<PosData> indexAll, const int mine)
{
	if (!isAct)
	{
		const int actRate = GetRand(9);

		if (actRate < 1) act = ActType::wait;
		else if (actRate < 2) act = ActType::drop;
		else if (actRate < 10) act = ActType::move;
	}

	if (act == ActType::move)
	{
		if (!isAct) DecisionMoveVecAndDist(rotate);
		UpdateMoveAct(collisionStage, charaPos);
	}
	if (act == ActType::wait)
	{
		UpdateWaitAct();
	}
	if (act == ActType::drop)
	{
		if (!isAct) DecisionDropVec(rotate);
		UpdateDropAct();
	}

	/*moveVec = { 0.0f, 0.0f, 0.0f };

	if (!isAction)
	{
		if (GetRandom(2)) action = Act::notAct;
	}
	else
	{
		action = Act::move;
		isAction = true;
	}

	if (action == Act::notAct)
	{
		UpdateNotAct();
		return;
	}

	if (action == Act::move)
	{
		UpdateMoveAct(collisionStage, charaPos, rotate);
		return;
	}

	if (action == Act::drop)
	{
		UpdateDropAct(rotate, indexAll, mine);
		return;
	}*/
}

void AI::Finalize()
{
}

void AI::DecisionMoveVecAndDist(VECTOR& rotate)
{
	//どの方向に移動するか決定する
	moveDir = static_cast<Direct>(GetRand(static_cast<int>(Direct::all) - 1));

	//移動量を決定する
	dist = (GetRand(7) + 1) * 0.2f;

	if (moveDir == Direct::left)
	{
		maxVec.x = dist;
		rotate.y = leftVec;
	}
	if (moveDir == Direct::right)
	{
		maxVec.x = dist;
		rotate.y = rightVec;
	}
	if (moveDir == Direct::back)
	{
		maxVec.z = dist;
		rotate.y = backVec;
	}
	if (moveDir == Direct::front)
	{
		maxVec.z = dist;
		rotate.y = frontVec;
	}
}

void AI::UpdateMoveAct(const vector<bool> collisionStage, const VECTOR charaPos)
{
	isAct = true;

	VECTOR moved = VSub(movePrevPos, charaPos);
	moved.x = fabsf(moved.x);
	moved.z = fabsf(moved.z);
	maxVec = VSub(maxVec, moved);

	if (!collisionStage[static_cast<int>(moveDir)])
	{
		if (moveDir == Direct::right) moveVec.x = 1.0f;
		if (moveDir == Direct::left) moveVec.x = -1.0f;
		if (moveDir == Direct::back) moveVec.z = 1.0f;
		if (moveDir == Direct::front) moveVec.z = -1.0f;
	}
	else
	{
		maxVec = { 0.0, 0.0f, 0.0f };
		moveVec = { 0.0, 0.0f, 0.0f };
	}

	if (maxVec.x <= 0.0f)
	{
		maxVec.x = 0.0f;
		moveVec.x = 0.0f;
	}

	if (maxVec.z <= 0.0f)
	{
		maxVec.z = 0.0f;
		moveVec.z = 0.0f;
	}

	movePrevPos = charaPos;

	if (VSize(maxVec) == 0.0f)
	{
		moveVec = { 0.0f, 0.0f ,0.0f };
		isAct = false;
	}
}

void AI::UpdateWaitAct()
{
	isAct = true;

	if (time++ == 20)
	{
		isAct = false;
		time = 0;
	}
}

void AI::DecisionDropVec(VECTOR& rotate)
{
	dropDir = static_cast<Direct>(GetRand(static_cast<int>(Direct::all) - 1));

	if (dropDir == Direct::left)
	{
		dropRotateY = rotate.y = leftVec;
	}
	if (dropDir == Direct::right)
	{
		dropRotateY = rotate.y = rightVec;
	}
	if (dropDir == Direct::back)
	{
		dropRotateY = rotate.y = backVec;
	}
	if (dropDir == Direct::front)
	{
		dropRotateY = rotate.y = frontVec;
	}
}

void AI::UpdateDropAct()
{
	isAct = true;

	if (time++ == 60)
	{
		isAct = false;
		time = 0;

		dropRotateY = -1.0f;
	}
}

/*
void AI::UpdateStageCost(const vector<PosData> posData, const vector<BlockType> fallType)
{
	movedIndex = posData;
	blockType = fallType;

	player.resize(movedIndex.size());

	//キャラクターの座標配列の取得
	for (int i = 0; i < movedIndex.size(); i++)
	{
		player[i] = movedIndex[i];
	}

	//脅威度マップ作成
	for (int i = 0; i < stage.size(); i++)
	{
		const PosData stagePos = { i % stageWidth, i / stageWidth };
		stage[i].pos = stagePos;
		stage[i].score = 0;

		for (int j = 0; j < player.size(); j++)
		{
			int score = 0;
			score = NormalizeHeuristic(GetHeuristic({ stage[i].pos.x, stage[i].pos.y}, player[j]));

			if (score < stage[i].score) continue;
			stage[i].score = score;
		}
	}

	//ステージの状態の取得
	for (int i = 0; i < blockType.size(); i++)
	{
		if (!blockType[i].isSelect && !blockType[i].isFall) continue;
		//ステージが狭まった場合
		if (blockType[i].isNone)
		{
			stage[i].score = charaPosCost+1;
			continue;
		}

		stage[i].score += (charaPosCost - 1);
	}
}

const int AI::GetHeuristic(const PosData target, const PosData source)
{
	const int distX = abs(target.x - source.x);
	const int distY = abs(target.y - source.y);

	return distX + distY;
}

const int AI::NormalizeHeuristic(const int heuristic)
{
	const int decuple = heuristic * 10;
	const int norm = static_cast<int>(decuple * 0.9);
	const int integer = decuple - 10;

	if (norm < integer) return 0;

	const int normalize = norm - integer + 1;
	return normalize;
}

const Direct AI::GetNewDirect(const Direct prevDirect, const std::vector<bool> collisionStage)
{
	//ステージと当たっていない場合は処理をしない
	if (!collisionStage[static_cast<int>(prevDirect)]) return prevDirect;

	//当たっていない方向を取得し配列データにする
	vector<int> moveDir;
	for (int i = 0; i < collisionStage.size(); i++)
	{
		if (!collisionStage[i]) moveDir.push_back(i);
	}

	//取得した配列をシャッフルし配列の先頭を新しい方向とする
	random_device randDev;
	mt19937_64 rand(randDev());
	shuffle(moveDir.begin(), moveDir.end(), rand);

	return static_cast<Direct>(moveDir.front());;
}

const VECTOR AI::GetMovedPos(const VECTOR charaPos, Direct& direct)
{
	VECTOR pos = charaPos;

	//移動方向を決める
	Direct dir = static_cast<Direct>(GetRand(DirectNum - 1));	
	if (direct != dir) direct = dir;

	//移動先の座標を決める
	if (direct == Direct::left) pos.x -= 1.0f;
	if (direct == Direct::right) pos.x += 1.0f;
	if (direct == Direct::back) pos.z += 1.0f;
	if (direct == Direct::front) pos.z -= 1.0f;

	return pos;
}

void AI::UpdateNotAct()
{
	if (30 < ++timer)
	{
		isAction = true;
		timer = 0;
	}
}

void AI::UpdateMoveAct(const std::vector<bool> collisionStage, const VECTOR charaPos, VECTOR& rotate)
{
	if (direct == Direct::left && movedPos.x < charaPos.x)
	{
		moveVec.x = -1.0f;
		rotate.y = leftVec;
	}
	else if (direct == Direct::right && charaPos.x < movedPos.x)
	{
		moveVec.x = 1.0f;
		rotate.y = rightVec;
	}
	else if (direct == Direct::back && charaPos.z < movedPos.z)
	{
		moveVec.z = 1.0f;
		rotate.y = backVec;
	}
	else if (direct == Direct::front && movedPos.z < charaPos.z)
	{
		moveVec.z = -1.0f;
		rotate.y = frontVec;
	}
	else
	{
		isAction = false;
		movedPos = GetMovedPos(charaPos, direct);
	}

	direct = GetNewDirect(direct, collisionStage);
}

void AI::UpdateDropAct(VECTOR& rotate, vector<PosData> indexAll, const int mine)
{
	SearchRoute* route = new SearchRoute();

	route->Initialize(indexAll, mine);

	direct = route->GetDirect();
	delete route;

	if (direct == Direct::left)
	{
		rotate.y = leftVec;
	}
	if (direct == Direct::right)
	{
		rotate.y = rightVec;
	}
	if (direct == Direct::back)
	{
		rotate.y = backVec;
	}
	if (direct == Direct::front)
	{
		rotate.y = frontVec;
	}
}

void AI::UpdateMoveing(const int num, const PosData pos)
{
	const int enemyIndex = pos.y * stageWidth + pos.x;
	if (stageWidth < stage[enemyIndex].score) SearchSeaftyArea(num, pos);
}

void AI::SearchSeaftyArea(const int num, const PosData pos)
{
	vector<PosData> movedPosData;

	//移動先の4方向の配列座標の取得
	for (int i = 0; i < 4; i++)
	{
		PosData movePos = pos;
		
		if (i== leftDirect) movePos.x -= 1;
		else if (i == rightDirect) movePos.x += 1;
		else if (i == backDirect) movePos.y -= 1;
		else if (i == frontDirect) movePos.y += 1;

		//範囲外は測定しない
		if (movePos.x < 0 || stageWidth <= movePos.x) continue;
		if (movePos.y < 0 || stageWidth <= movePos.y) continue;
		
		movedPosData.push_back(movePos);

		const int index = pos.y * stageWidth + pos.x;
		if (!blockType[index].isNone) continue;

		for (int j = 0; j < 4; j++)
		{
			PosData movedPos = movePos;

			if (j == leftDirect) movedPos.x -= 1;
			else if (j == rightDirect) movedPos.x += 1;
			else if (j == backDirect) movedPos.y -= 1;
			else if (j == frontDirect) movedPos.y += 1;

			//範囲外は測定しない
			if (movedPos.x < 0 || stageWidth <= movedPos.x) continue;
			if (movedPos.y < 0 || stageWidth <= movedPos.y) continue;

			movedPosData.push_back(movedPos);
		}
	}

	vector<int> score;

	//移動後のスコアの取得
	for (int i = 0; i < movedPosData.size(); i++)
	{
		const int index = movedPosData[i].y * stageWidth + movedPosData[i].x;
		score.push_back(stage[index].score);
	}

	vector<int> sortScore = score;
	sort(sortScore.begin(), sortScore.end());

	vector<PosData> lowScore;

	//最小スコアの場所を見つける
	for (int i = 0; i < score.size(); i++)
	{
		if (sortScore.front() < score[i]) continue;
		if (charaPosCost <= sortScore.front()) continue;

		lowScore.push_back(movedPosData[i]);
	}

	//複数見つけた場合ランダムで決定する
	const int index = GetRand(static_cast<int>(lowScore.size()) - 1);
	if (lowScore.size() != 0)
	{
		movedIndex[num] = lowScore[index];
		isAvoid = true;
	}
}

const bool AI::GetRandom(const int probability)
{
	const int rand = 100 / probability;

	if (GetRand(rand) == 0) return true;
	return false;
}
*/