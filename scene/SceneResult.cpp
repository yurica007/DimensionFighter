#include "SceneResult.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "../PlayerData.h"
#include "../InputState.h"
#include "../ui/UITime.h"
#include <DxLib.h>

namespace
{
	VECTOR uiPos = { 580.0f, 600.0f, 0.0f };
}

SceneResult::SceneResult(SceneManager& manager) :
	SceneBase(manager), currentTime(0), ui(new UITime())
{
}

SceneResult::~SceneResult()
{
}

void SceneResult::Initialize()
{
	currentTime = GetNowCount();
	ranking.resize(playerNum);
	ui->Initialize(uiPos);
}

void SceneResult::Update(const InputState& inputState)
{
	if (IsTransScene(inputState)) return;
	ui->SetCurrentTime(clearTime);
	ui->Update();
}

void SceneResult::Draw()
{
	DrawString(0, 0, "result", 0xfffff);

	for (int i = 0; i < playerNum; i++)
	{
		DrawFormatString(i * 320 + 153, 300, 0xffffff, "%dˆÊ", ranking[i]);
		DrawFormatString(i * 320 + 153, 475, 0xffffff, "%dP", i + 1);
	}
	ui->Draw();

	//for (int i = 0; i < 9; i++)
	//{
	//	if (i % 2 == 1) DrawLine(160 * i, 0, 160 * i, 740, 0xffffff);
	//}
	//DrawFormatString(640, 500, 0xffffff, "%dF%dF%d", clearTime.minutes, clearTime.seconds, clearTime.miliSeconds);
}

void SceneResult::Finalize()
{
	ui->Finalize();
}

void SceneResult::SetRanking(const vector<int> rank)
{
	if (rank.size() != playerNum) return;
	ranking = rank;
}

void SceneResult::SetClearTime(const Time time)
{
	clearTime = time;
}

bool SceneResult::IsTransScene(const InputState& inputState)
{
	if (inputState.IsTriggered(InputType::debug))
	{
		SceneTitle* nextScene = new SceneTitle(manager_);
		manager_.ChangeScene(nextScene);
		nextScene->Initialize();
		return true;
	}

	return false;
}
