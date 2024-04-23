#include "CharaBase.h"
#include "../Stage.h"
#include "../tool/InputState.h"
#include "../tool/SearchRoute.h"
#include "../tool/Collision3D.h"
#include "../tool/Load.h"
#include "../Easing.h"

#include <fstream>

namespace
{
	using namespace std;

	constexpr VECTOR initStagePos = { -3.5f + 640.0f, 0.8f + 360.0f, -3.5f };
	
	constexpr float initCharaPosY = 0.8f + 360.0f;
	constexpr float fallTime = 120.0f;
}

CharaBase::CharaBase(Stage& stage) :
	stage(stage), collision(new Collision3D(stage)),
	charaNum(0), immotileTime(0),
	easing(new Easing()), time(0.0f), stageMaxWidth(0)
{
}

void CharaBase::Initialize(const vector<string> charaData, const int number, vector<PosData> currentIndex)
{
	charaNum = number;

	chara.model = MV1LoadModel(charaData[0].c_str());
	MV1SetTextureGraphHandle(chara.model, 0, LoadGraph(charaData[1].c_str()), false);

	stageMaxWidth = stage.GetStageSize();
	SpawnRandom(currentIndex);

	chara.power = static_cast<CharaPower>(charaNum);
	chara.moveSpeed = stof(charaData[2]);
	chara.stiffenTime = stoi(charaData[3]);

	//モデルのサイズ変更
	const float radius = static_cast<float>(modelScale);
	const VECTOR scale = { radius, radius, radius };
	MV1SetScale(chara.model, scale);

	collision->Initialize();
}

void CharaBase::Draw()
{
	if (!chara.isDead) MV1DrawModel(chara.model);
	//collision->Draw();
}

void CharaBase::Finalize()
{
	MV1DeleteModel(chara.model);
	collision->Finalize();

	delete collision;
}

const bool CharaBase::IsOnStage()
{
	if (chara.pos.y < initCharaPosY) return false;

	const PosData currentIndex = ConvertPosToIndex();
	const vector<BlockIndexData> fallData = stage.GetBlockData();

	//変換したキャラクターの配列座標とステージの配列データを比較
	for (int i = 0; i < fallData.size(); i++)
	{
		if (!fallData[i].isFall) continue;

		const bool widthCheck = (fallData[i].index.x == currentIndex.x);
		const bool heightCheck = (fallData[i].index.y == currentIndex.y);

		if (widthCheck && heightCheck) return false;
	}

	return true;
}

const void CharaBase::UpdateFall(const vector<PosData> currentIndex)
{
	//キャラクターの落下処理
	if(time < fallTime) time++;
	else
	{
		chara.hp--;
		time = 0.0f;

		//ライフがなくなれば死亡、ある場合はリスポーンする
		if (chara.hp == 0) chara.isDead = true;
		else SpawnRandom(currentIndex);
	}

	const float addGravityPos = easing->InSine(time, fallTime, charaDeadLine, initCharaPosY);
	chara.pos.y = addGravityPos;
}

const bool CharaBase::IsMove(const int immotileTime)
{
	if (immotileTime == 0) return true;
	return false;
}

const void CharaBase::SpawnRandom(const vector<PosData> spawnIndexAll)
{
	PosData spawnIndex;

	//二次元配列上でスポーン地点を決める
	bool isEnd = false;
	const int stageSize = stage.GetStageSize();
	while (!isEnd)
	{
		const int space = 4 - stageSize / 2;

		spawnIndex.x = GetRand(stageSize - 1) + space;
		spawnIndex.y = GetRand(stageSize - 1) + space;

		if (spawnIndexAll.size() == 0) break;
		//他のキャラクタースポーンが被らないようにする
		for (int i = 0; i < spawnIndexAll.size(); i++)
		{
			if (spawnIndex.x != spawnIndexAll[i].x || spawnIndex.y != spawnIndexAll[i].y) isEnd = true;
			else
			{
				isEnd = false;
				break;
			}
		}
	}

	//決定した二次元配列の縦、横から三次元座標に変換する
	const float spawnWidthPos = static_cast<float>(spawnIndex.x) + initStagePos.x;
	const float spawnDepthPos = static_cast<float>(spawnIndex.y) + initStagePos.z;

	chara.pos = { spawnWidthPos, initCharaPosY, spawnDepthPos };
}

const PosData CharaBase::ConvertPosToIndex()
{
	PosData currentIndex;

	currentIndex.x = static_cast<int>(chara.pos.x - 640.0f + stageMaxWidth / 2);
	currentIndex.y = static_cast<int>(chara.pos.z + stageMaxWidth / 2);

	return currentIndex;
}
