#pragma once

#include <deque>

class SceneBase;
class InputState;

class SceneManager
{
public:
	/// <summary>
	/// シーンの切り替えを行う
	/// </summary>
	/// <param name="scene">切り替えたい次シーンのアドレス</param>
	void ChangeScene(SceneBase* scene);

	/// <summary>
	/// シーンを上に積む
	/// Updateで実行されるのは上に積まれたシーンのみ
	/// </summary>
	void PushuScene(SceneBase* scene);

	/// <summary>
	/// シーンを削除する
	/// Updateで実行されるのは上に積まれたシーンのみ
	/// </summary>
	void PopScene();

	void EndScene();

	bool IsSceneEnd() { return isGameEnd; }

	/// <summary>
	/// 各シーンの初期化処理を行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// 各シーンのUpdateを行う
	/// </summary>
	/// <param name="input">入力ステート</param>
	void Update(const InputState& input);
	/// <summary>
	/// 各シーンの描画を行う
	/// </summary>
	void Draw();

	/// <summary>
	/// 各シーンの終了処理を行う
	/// </summary>
	void Finalize();

private:
	//「今実行中」のシーンを切り替えていきたいので参照ではなくポインタとして宣言
	std::deque<SceneBase*> scenes_;

	bool isGameEnd = false;
};

