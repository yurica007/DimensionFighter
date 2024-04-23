#include "NPC.h"
#include "../Stage.h"
#include "../tool/InputState.h"
#include "../tool/AI.h"
#include "../tool/Collision3D.h"

namespace
{
	using namespace std;
}

NPC::NPC(Stage& stage) :
	CharaBase(stage), ai(new AI())
{
}

void NPC::Initialize(const vector<string> charaData, const int number, vector<PosData> currentIndex)
{
	CharaBase::Initialize(charaData, number, currentIndex);
	ai->Initialize(stage.GetStageSize(), chara.pos);
}

void NPC::Update(const InputState& inputState, const int time, const vector<PosData> currentIndex)
{
	collision->UpdateCollRange();
	if (chara.isDead) return;

	//�����㏈��
	chara.isOnStage = CharaBase::IsOnStage();
	if (!chara.isOnStage) CharaBase::UpdateFall(currentIndex);

	collision->Update(chara.pos);
	//�X�e�[�W��̓����蔻��
	stageCollision = collision->IsCollideStage();

	if (chara.isOnStage)
	{
		const bool isMove = CharaBase::IsMove(immotileTime);

		//�ړ�����
		if (isMove) ai->Update(stageCollision, chara.pos, chara.moveRotate, indexAll, charaNum);
		VECTOR moveVec = ai->GetMoveVec();

		//�ړ��ʂ𐳋K����A�ړ����x����Z
		if (VSize(moveVec) != 0) moveVec = VScale(moveVec, chara.moveSpeed);
		chara.pos = VAdd(chara.pos, moveVec);

		if (time < 0) chara.dropRotateY = ai->GetDropRotateY();
	}
	else chara.dropRotateY = initDropRotateY;

	stage.PrevBlock(chara.dropRotateY, GetCurrentIndex(), chara.power, charaNum);

	MV1SetPosition(chara.model, chara.pos);
	MV1SetRotationXYZ(chara.model, chara.moveRotate);
}

void NPC::Draw()
{
	CharaBase::Draw();
}

void NPC::Finalize()
{
	CharaBase::Finalize();

	ai->Finalize();
	delete ai;
}