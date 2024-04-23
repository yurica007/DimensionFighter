#pragma once
#include <map>
#include <vector>
#include <string>

enum class InputType
{
	select,	//����
	cancel,	//������
	up,		//��
	down,	//��
	left,	//��
	right,	//��
	menu,	//���j���[
	max		//�ő���̓C���f�b�N�X
};

enum class InputCategory
{
	keybd,	// �L�[�{�[�h
	pad		// �Q�[���p�b�h
};

struct InputInfo
{
	InputCategory cat;	// ���͑��u�J�e�S��
	int id;	// ����ID(KEY_INPUT_�`,PAD_INPUT_�`,MOUSE_INPUT_�`)
};

class InputState
{
public:
	InputState();
	void Update();

	/// <summary>
	/// �����ꂽ�u�ԂɎ擾����
	/// </summary>
	/// <param name="type">���͏��</param>
	/// <returns>true�F������ false�F�����Ă��Ȃ�</returns>
	const bool IsTriggered(const InputType type) const;

	/// <summary>
	/// ������Ă���Ԏ擾����
	/// </summary>
	/// <param name="type">���͏��</param>
	/// <returns>true�F������Ă��� false�F������Ă��Ȃ�</returns>
	const bool IsPressed(const InputType type) const;

private:
	// ���ۂ̓��͂ƃQ�[���{�^���̑Ή��e�[�u��
	// �L�[(first)=InputType
	// �l(second)=std::vector<InputInfo>
	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable;// ���ۂ̓��͂ƃQ�[���{�^���̑Ή��e�[�u��

	std::vector<bool> currentInput;	// ���݂̓��͏��(�����Ă��邩�����Ă��Ȃ���)
	std::vector<bool> lastInput;	// ���O�̓��͏��(���O�ɉ����Ă��邩�����Ă��Ȃ���)
};


