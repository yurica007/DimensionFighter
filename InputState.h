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
	debug,	//デバッグ
	max		//最大入力インデックス
};

/// <summary>
/// 入力装置カテゴリ
/// </summary>
enum class InputCategory
{
	keybd,	// キーボード
	pad		// ゲームパッド
};

/// <summary>
/// 入力情報
/// </summary>
struct InputInfo
{
	InputCategory cat;	// 入力装置カテゴリ
	int id;	// 入力ID(KEY_INPUT_～,PAD_INPUT_～,MOUSE_INPUT_～)
};

// フレンド用にプロトタイプ宣言
class KeyConfigScene;

/// <summary>
/// 入力状態を管理する
/// </summary>
class InputState
{
	friend KeyConfigScene;	// KeyConfigSceneにだけ、すべてを見せる。
public:
	InputState();

	/// <summary>
	/// 押した瞬間にtrueになる
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns></returns>
	bool IsTriggered(InputType type) const;

	/// <summary>
	/// 押されていたらtrueになる
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	bool IsPressed(InputType type) const;

	/// <summary>
	/// 入力情報を更新する
	/// </summary>
	/// (注意)毎フレーム
	void Update();

private:
	// 実際の入力とゲームボタンの対応テーブル
	// キー(first)=InputType
	// 値(second)=std::vector<InputInfo>
	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;// 実際の入力とゲームボタンの対応テーブル

	std::vector<bool> currentInput_;	// 現在の入力情報(押しているか押していないか)
	std::vector<bool> lastInput_;	// 直前の入力情報(直前に押しているか押していないか)
};


