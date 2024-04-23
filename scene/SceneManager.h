#pragma once
#include "../SceneData.h"
#include <deque>
#include <DxLib.h>

class SceneBase;
class InputState;

class SceneManager
{
public:
	SceneManager();
	~SceneManager() {}

	void Initialize();
	void Update(const InputState& input);
	void Draw();
	void Finalize();

	/// <summary>
	/// �V�[����ؑւ���
	/// </summary>
	/// <param name="scene">�ؑւ���V�[��</param>
	void ChangeScene(SceneBase* scene);

	/// <summary>
	/// �V�[������ɏd�˂�
	/// </summary>
	/// <param name="scene">�d�˂�V�[��</param>
	void PushScene(SceneBase* scene);

	/// <summary>
	/// �V�[�����ォ��폜����
	/// </summary>
	void PopScene();

	void ChangePushScene(SceneBase* scene);

	/// <summary>
	/// �V�[���I���̏���������
	/// </summary>
	void EndScene();

	/// <summary>
	/// �V�[�����I�����邩�ۂ��擾����
	/// </summary>
	/// <returns>true�F�V�[�����I������ false�F�V�[�����I�����Ȃ�</returns>
	const bool IsSceneEnd() { return isGameEnd; }

private:
	std::deque<SceneBase*> scenes_;
	bool isGameEnd;

	LONGLONG updateTime;
	LONGLONG drawTime;
};

