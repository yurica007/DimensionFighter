#include "InputState.h"
#include <DxLib.h>
#include <cassert>

InputState::InputState()
{
	inputMapTable_[InputType::select] = { {InputCategory::keybd, KEY_INPUT_Z},
										{InputCategory::pad, PAD_INPUT_2} };//決定

	inputMapTable_[InputType::cancel] = { {InputCategory::keybd, KEY_INPUT_X},
										{InputCategory::pad, PAD_INPUT_1} };//取り消し

	inputMapTable_[InputType::up] = { {InputCategory::keybd, KEY_INPUT_UP},
									{InputCategory::pad, PAD_INPUT_UP} };//上

	inputMapTable_[InputType::down] = { {InputCategory::keybd, KEY_INPUT_DOWN},
									{InputCategory::pad, PAD_INPUT_DOWN} };//下

	inputMapTable_[InputType::left] = { {InputCategory::keybd, KEY_INPUT_LEFT},
									{InputCategory::pad, PAD_INPUT_LEFT} };//左

	inputMapTable_[InputType::right] = { {InputCategory::keybd, KEY_INPUT_RIGHT},
									{InputCategory::pad, PAD_INPUT_RIGHT} };//右
	
	inputMapTable_[InputType::debug] = { {InputCategory::keybd, KEY_INPUT_RETURN} };//右

	currentInput_.resize(static_cast<int>(InputType::max));
	lastInput_.resize(static_cast<int>(InputType::max));
}

void InputState::Update()
{
	lastInput_ = currentInput_;	// 直前の入力情報を記憶しておく

	char keyState[256];
	GetHitKeyStateAll(keyState);	// 全キー情報取得

	int padState = GetJoypadInputState(DX_INPUT_PAD1);	// パッド1コンの情報を取得
	int mouseState = GetMouseInput();

	for (const auto& keymap : inputMapTable_)	// マップの全情報をループする
	{
		for (const auto& input : keymap.second)	// 入力情報配列をループする
		{
			// このinputの中身は、keybd,KEY_INPUT_RETURNなどのセット(InputInfo)が入っている
			// keymap.secondには、この入力情報セットInputInfoセットが入っている
			// keymap.firstには、対応するゲーム名の"InputType::next"などが入っている
			if (input.cat == InputCategory::keybd)
			{
				currentInput_[static_cast<int>(keymap.first)] = keyState[input.id];
			}
			else if (input.cat == InputCategory::pad)
			{
				currentInput_[static_cast<int>(keymap.first)] = padState & input.id;
			}
			//2つの入力のうちいずれかがtrueだったらもう「入力されている」
			//とみなして、breakする
			if (currentInput_[static_cast<int>(keymap.first)])
			{
				break;
			}
		}
	}
}

bool InputState::IsPressed(InputType type) const
{
	return currentInput_[static_cast<int>(type)];
}

bool InputState::IsTriggered(InputType type) const
{
	return IsPressed(type) && !lastInput_[static_cast<int>(type)];
}
