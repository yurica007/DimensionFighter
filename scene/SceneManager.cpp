#include "SceneManager.h"
#include "SceneBase.h"

void SceneManager::ChangeScene(SceneBase* scene)
{
	//すべてのシーンをチェックする
	while (scenes_.size() > 0)
	{
		//シーンスタックが空ではなかったら
		if (!scenes_.empty())
		{
			delete scenes_.front();	//topはてっぺんの要素を返してる
			scenes_.pop_front();		//1個減る→ふつうは0になる
		}
	}

	scenes_.push_front(scene);	//1個増える
}

void SceneManager::PushuScene(SceneBase* scene)
{
	scenes_.push_front(scene);
}

void SceneManager::PopScene()
{
	//ポップの結果、シーンが0にならないようにする
//	if (scenes_.size() > 1)
	{
		delete scenes_.front();
		scenes_.pop_front();
	}
}

void SceneManager::EndScene()
{
	if (scenes_.size() > 1)
	{
		delete scenes_.front();
		scenes_.pop_front();
	}

	isGameEnd = true;
}

void SceneManager::Initialize()
{
	scenes_.front()->Initialize();
}

void SceneManager::Update(const InputState& input)
{
	scenes_.front()->Update(input);
}

void SceneManager::Draw()
{
	for (int i = static_cast<int>(scenes_.size()) - 1; i >= 0; i--)
	{
		scenes_[i]->Draw();
	}
}

void SceneManager::Finalize()
{
	scenes_.front()->Finalize();
}
