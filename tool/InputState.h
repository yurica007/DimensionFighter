#pragma once
#include <map>
#include <vector>
#include <string>

enum class InputType
{
	select,	//決定
	cancel,	//取り消し
	up,		//↑
	down,	//↓
	left,	//←
	right,	//→
	menu,	//メニュー
	max		//最大入力インデックス
};

enum class InputCategory
{
	keybd,	// キーボード
	pad		// ゲームパッド
};

struct InputInfo
{
	InputCategory cat;	// 入力装置カテゴリ
	int id;	// 入力ID(KEY_INPUT_～,PAD_INPUT_～,MOUSE_INPUT_～)
};

class InputState
{
public:
	InputState();
	void Update();

	/// <summary>
	/// 押された瞬間に取得する
	/// </summary>
	/// <param name="type">入力情報</param>
	/// <returns>true：押した false：押していない</returns>
	const bool IsTriggered(const InputType type) const;

	/// <summary>
	/// 押されている間取得する
	/// </summary>
	/// <param name="type">入力情報</param>
	/// <returns>true：押されている false：押されていない</returns>
	const bool IsPressed(const InputType type) const;

private:
	// 実際の入力とゲームボタンの対応テーブル
	// キー(first)=InputType
	// 値(second)=std::vector<InputInfo>
	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable;// 実際の入力とゲームボタンの対応テーブル

	std::vector<bool> currentInput;	// 現在の入力情報(押しているか押していないか)
	std::vector<bool> lastInput;	// 直前の入力情報(直前に押しているか押していないか)
};


