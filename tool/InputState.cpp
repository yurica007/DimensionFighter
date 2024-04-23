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
	lastInput = currentInput;	//直前の入力情報を記憶しておく

	char keyState[256];
	GetHitKeyStateAll(keyState);	//全キー情報取得

	int padState = GetJoypadInputState(DX_INPUT_PAD1);	//パッド1コンの情報を取得
	int mouseState = GetMouseInput();

	for (const auto& keymap : inputMapTable)	//マップの全情報をループする
	{
		for (const auto& input : keymap.second)	//入力情報配列をループする
		{
			//このinputの中身は、keybd,KEY_INPUT_RETURNなどのセット(InputInfo)が入っている
			//keymap.secondには、この入力情報セットInputInfoセットが入っている
			//keymap.firstには、対応するゲーム名の"InputType::next"などが入っている
			if (input.cat == InputCategory::keybd)
			{
				currentInput[static_cast<int>(keymap.first)] = keyState[input.id];
			}
			else if (input.cat == InputCategory::pad)
			{
				currentInput[static_cast<int>(keymap.first)] = padState & input.id;
			}
			//2つの入力のうちいずれかがtrueだったらもう「入力されている」
			//とみなして、breakする
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
