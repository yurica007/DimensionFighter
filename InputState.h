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
	debug,	//�f�o�b�O
	max		//�ő���̓C���f�b�N�X
};

/// <summary>
/// ���͑��u�J�e�S��
/// </summary>
enum class InputCategory
{
	keybd,	// �L�[�{�[�h
	pad		// �Q�[���p�b�h
};

/// <summary>
/// ���͏��
/// </summary>
struct InputInfo
{
	InputCategory cat;	// ���͑��u�J�e�S��
	int id;	// ����ID(KEY_INPUT_�`,PAD_INPUT_�`,MOUSE_INPUT_�`)
};

// �t�����h�p�Ƀv���g�^�C�v�錾
class KeyConfigScene;

/// <summary>
/// ���͏�Ԃ��Ǘ�����
/// </summary>
class InputState
{
	friend KeyConfigScene;	// KeyConfigScene�ɂ����A���ׂĂ�������B
public:
	InputState();

	/// <summary>
	/// �������u�Ԃ�true�ɂȂ�
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns></returns>
	bool IsTriggered(InputType type) const;

	/// <summary>
	/// ������Ă�����true�ɂȂ�
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	bool IsPressed(InputType type) const;

	/// <summary>
	/// ���͏����X�V����
	/// </summary>
	/// (����)���t���[��
	void Update();

private:
	// ���ۂ̓��͂ƃQ�[���{�^���̑Ή��e�[�u��
	// �L�[(first)=InputType
	// �l(second)=std::vector<InputInfo>
	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;// ���ۂ̓��͂ƃQ�[���{�^���̑Ή��e�[�u��

	std::vector<bool> currentInput_;	// ���݂̓��͏��(�����Ă��邩�����Ă��Ȃ���)
	std::vector<bool> lastInput_;	// ���O�̓��͏��(���O�ɉ����Ă��邩�����Ă��Ȃ���)
};


