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
	/// シーンを切替える
	/// </summary>
	/// <param name="scene">切替えるシーン</param>
	void ChangeScene(SceneBase* scene);

	/// <summary>
	/// シーンを上に重ねる
	/// </summary>
	/// <param name="scene">重ねるシーン</param>
	void PushScene(SceneBase* scene);

	/// <summary>
	/// シーンを上から削除する
	/// </summary>
	void PopScene();

	void ChangePushScene(SceneBase* scene);

	/// <summary>
	/// シーン終了の準備をする
	/// </summary>
	void EndScene();

	/// <summary>
	/// シーンを終了するか否か取得する
	/// </summary>
	/// <returns>true：シーンを終了する false：シーンを終了しない</returns>
	const bool IsSceneEnd() { return isGameEnd; }

private:
	std::deque<SceneBase*> scenes_;
	bool isGameEnd;

	LONGLONG updateTime;
	LONGLONG drawTime;
};

