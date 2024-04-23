#include "PC.h"
#include "../Stage.h"
#include "../tool/InputState.h"
#include "../tool/Collision3D.h"

namespace
{
	using namespace std;
}

PC::PC(Stage& stage) :
	CharaBase(stage)
{
}

void PC::Initialize(const vector<string> charaData, const int number, std::vector<PosData> currentIndex)
{
	CharaBase::Initialize(charaData, number, currentIndex);
	collision->Initialize();
}

void PC::Update(const InputState& inputState, const int time, const vector<PosData> currentIndex)
{
	collision->UpdateCollRange();
	if (chara.isDead) return;

	//落下後処理
	chara.isOnStage = CharaBase::IsOnStage();
	if (!chara.isOnStage) CharaBase::UpdateFall(currentIndex);

	collision->Update(chara.pos);
	//ステージ上の当たり判定
	stageCollision = collision->IsCollideStage();

	if (chara.isOnStage)
	{
		const bool isMove = CharaBase::IsMove(immotileTime);
		if (immotileTime != 0) immotileTime--;

		//入力されたらその方向へ向かせる
		//当たり判定を確認し、当たっていない場合のみ移動させる
		VECTOR moveVec = { 0.0f, 0.0f, 0.0f };
		if (inputState.IsPressed(InputType::left) && isMove)
		{
			if (!stageCollision[leftDirect]) moveVec.x = -1.0f;
			chara.moveRotate.y = leftVec;
		}
		else if (inputState.IsPressed(InputType::right) && isMove)
		{
			if (!stageCollision[rightDirect]) moveVec.x = 1.0f;
			chara.moveRotate.y = rightVec;
		}
		else if (inputState.IsPressed(InputType::up) && isMove)
		{
			if (!stageCollision[backDirect]) moveVec.z = 1.0f;
			chara.moveRotate.y = backVec;
		}
		else if (inputState.IsPressed(InputType::down) && isMove)
		{
			if (!stageCollision[frontDirect]) moveVec.z = -1.0f;
			chara.moveRotate.y = frontVec;
		}

		//移動量を正規化後、移動速度を乗算
		if (VSize(moveVec) != 0) moveVec = VScale(moveVec, chara.moveSpeed);
		chara.pos = VAdd(chara.pos, moveVec);

		//ブロックを落下させる
		//向いている方向を渡し、それ以外はエラー数値を渡す
		if (inputState.IsTriggered(InputType::select) && isMove && time < 0)
		{
			chara.dropRotateY = chara.moveRotate.y;
			immotileTime = chara.stiffenTime;
		}
		else chara.dropRotateY = initDropRotateY;
	}
	else chara.dropRotateY = initDropRotateY;
	stage.PrevBlock(chara.dropRotateY, GetCurrentIndex(), chara.power, charaNum);

	MV1SetPosition(chara.model, chara.pos);
	MV1SetRotationXYZ(chara.model, chara.moveRotate);
}

void PC::Draw()
{
	CharaBase::Draw();
}

void PC::Finalize()
{
	CharaBase::Finalize();
}