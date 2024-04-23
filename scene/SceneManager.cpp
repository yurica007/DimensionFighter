#include "SceneManager.h"
#include "SceneBase.h"
#include "../GameData.h"

SceneManager::SceneManager() :
	isGameEnd (false), updateTime(0), drawTime(0)
{
}

void SceneManager::Initialize()
{
	scenes_.front()->Initialize();
}

void SceneManager::Update(const InputState& input)
{
	updateTime = GetNowHiPerformanceCount();

	scenes_.front()->Update(input);
	
	updateTime = GetNowHiPerformanceCount() - updateTime;
}

void SceneManager::Draw()
{
	drawTime = GetNowHiPerformanceCount();

	for (int i = static_cast<int>(scenes_.size()) - 1; 0 <= i; i--)
	{
		scenes_[i]->Draw();
	}

	drawTime = GetNowHiPerformanceCount() - drawTime;

	//処理バーの表示
	/*{
		float rate = static_cast<float>(updateTime + drawTime) / 16666.6f;
		int width = static_cast<int>(screenWidth * rate);

		DrawString(0, screenHeight - 70, "Update", 0xff00000);
		DrawBox(0, screenHeight - 50, width, screenHeight - 30, 0xff0000, true);

		rate = static_cast<float>(updateTime) / 16666.6f;
		width = static_cast<int>(screenWidth * rate);

		DrawString(100, screenHeight - 70, "Draw", 0x00000ff);
		DrawBox(0, screenHeight - 50, width, screenHeight - 30, 0x0000aa, true);
	}*/
}

void SceneManager::Finalize()
{
	scenes_.front()->Finalize();
}

void SceneManager::ChangeScene(SceneBase* scene)
{
	//すべてのシーンをチェックする
	while (0 < scenes_.size())
	{
		//シーンスタックが空ではなかったら
		if (scenes_.empty()) continue;

		delete scenes_.front();	//topはてっぺんの要素を返してる
		scenes_.pop_front();		//1個減る→ふつうは0になる		
	}

	PushScene(scene);	//1個増える
}

void SceneManager::PushScene(SceneBase* scene)
{
	scenes_.push_front(scene);
}

void SceneManager::PopScene()
{
	//ポップの結果、シーンが0にならないようにする
	if (scenes_.size() <= 1) return;

	delete scenes_.front();
	scenes_.pop_front();
}

void SceneManager::ChangePushScene(SceneBase* scene)
{
	if (scenes_.size() == 0) return;

	scenes_.pop_front();
	scenes_.push_front(scene);
}

void SceneManager::EndScene()
{ 
	isGameEnd = true;
	PopScene();
}