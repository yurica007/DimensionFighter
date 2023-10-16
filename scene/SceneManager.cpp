#include "SceneManager.h"
#include "SceneBase.h"

void SceneManager::ChangeScene(SceneBase* scene)
{
	//���ׂẴV�[�����`�F�b�N����
	while (scenes_.size() > 0)
	{
		//�V�[���X�^�b�N����ł͂Ȃ�������
		if (!scenes_.empty())
		{
			delete scenes_.front();	//top�͂Ă��؂�̗v�f��Ԃ��Ă�
			scenes_.pop_front();		//1���遨�ӂ���0�ɂȂ�
		}
	}

	scenes_.push_front(scene);	//1������
}

void SceneManager::PushuScene(SceneBase* scene)
{
	scenes_.push_front(scene);
}

void SceneManager::PopScene()
{
	//�|�b�v�̌��ʁA�V�[����0�ɂȂ�Ȃ��悤�ɂ���
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
