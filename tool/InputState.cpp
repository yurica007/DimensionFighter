#include "InputState.h"
#include <DxLib.h>
#include <cassert>

InputState::InputState()
{
	inputMapTable[InputType::select] = { {InputCategory::keybd, KEY_INPUT_Z},
										{InputCategory::pad, PAD_INPUT_2} };

	inputMapTable[InputType::cancel] = { {InputCategory::keybd, KEY_INPUT_X},
										{InputCategory::pad, PAD_INPUT_1} };

	inputMapTable[InputType::up] = { {InputCategory::keybd, KEY_INPUT_UP},
									{InputCategory::pad, PAD_INPUT_UP} };

	inputMapTable[InputType::down] = { {InputCategory::keybd, KEY_INPUT_DOWN},
									{InputCategory::pad, PAD_INPUT_DOWN} };

	inputMapTable[InputType::left] = { {InputCategory::keybd, KEY_INPUT_LEFT},
									{InputCategory::pad, PAD_INPUT_LEFT} };

	inputMapTable[InputType::right] = { {InputCategory::keybd, KEY_INPUT_RIGHT},
									{InputCategory::pad, PAD_INPUT_RIGHT} };
	
	inputMapTable[InputType::menu] = { {InputCategory::keybd, KEY_INPUT_Q},
									{InputCategory::pad, PAD_INPUT_8} };

	currentInput.resize(static_cast<int>(InputType::max));
	lastInput.resize(static_cast<int>(InputType::max));
}

void InputState::Update()
{
	lastInput = currentInput;	//���O�̓��͏����L�����Ă���

	char keyState[256];
	GetHitKeyStateAll(keyState);	//�S�L�[���擾

	int padState = GetJoypadInputState(DX_INPUT_PAD1);	//�p�b�h1�R���̏����擾
	int mouseState = GetMouseInput();

	for (const auto& keymap : inputMapTable)	//�}�b�v�̑S�������[�v����
	{
		for (const auto& input : keymap.second)	//���͏��z������[�v����
		{
			//����input�̒��g�́Akeybd,KEY_INPUT_RETURN�Ȃǂ̃Z�b�g(InputInfo)�������Ă���
			//keymap.second�ɂ́A���̓��͏��Z�b�gInputInfo�Z�b�g�������Ă���
			//keymap.first�ɂ́A�Ή�����Q�[������"InputType::next"�Ȃǂ������Ă���
			if (input.cat == InputCategory::keybd)
			{
				currentInput[static_cast<int>(keymap.first)] = keyState[input.id];
			}
			else if (input.cat == InputCategory::pad)
			{
				currentInput[static_cast<int>(keymap.first)] = padState & input.id;
			}
			//2�̓��͂̂��������ꂩ��true������������u���͂���Ă���v
			//�Ƃ݂Ȃ��āAbreak����
			if (currentInput[static_cast<int>(keymap.first)]) break;
		}
	}
}

const bool InputState::IsPressed(const InputType type) const
{
	return currentInput[static_cast<int>(type)];
}

const bool InputState::IsTriggered(const InputType type) const
{
	return IsPressed(type) && !lastInput[static_cast<int>(type)];
}
