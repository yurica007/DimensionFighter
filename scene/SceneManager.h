#pragma once

#include <deque>

class SceneBase;
class InputState;

class SceneManager
{
public:
	/// <summary>
	/// �V�[���̐؂�ւ����s��
	/// </summary>
	/// <param name="scene">�؂�ւ��������V�[���̃A�h���X</param>
	void ChangeScene(SceneBase* scene);

	/// <summary>
	/// �V�[������ɐς�
	/// Update�Ŏ��s�����̂͏�ɐς܂ꂽ�V�[���̂�
	/// </summary>
	void PushuScene(SceneBase* scene);

	/// <summary>
	/// �V�[�����폜����
	/// Update�Ŏ��s�����̂͏�ɐς܂ꂽ�V�[���̂�
	/// </summary>
	void PopScene();

	void EndScene();

	bool IsSceneEnd() { return isGameEnd; }

	/// <summary>
	/// �e�V�[���̏������������s��
	/// </summary>
	void Initialize();

	/// <summary>
	/// �e�V�[����Update���s��
	/// </summary>
	/// <param name="input">���̓X�e�[�g</param>
	void Update(const InputState& input);
	/// <summary>
	/// �e�V�[���̕`����s��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e�V�[���̏I���������s��
	/// </summary>
	void Finalize();

private:
	//�u�����s���v�̃V�[����؂�ւ��Ă��������̂ŎQ�Ƃł͂Ȃ��|�C���^�Ƃ��Đ錾
	std::deque<SceneBase*> scenes_;

	bool isGameEnd = false;
};

