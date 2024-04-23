#pragma once
#include "../SceneData.h"

enum class SceneState
{
	pushScne,
	popScene,
	popPushScene,
	donot
};

class SceneManager;
class Camera;
class ReuseData;
class InputState;

class SceneBase
{
public:
	SceneBase(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneBase() {}

	virtual void Initialize() = 0;
	virtual void Update(const InputState& input) = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;

	const void SetPrevScene(SceneBase* scene) { prevScene = scene; }
	void ResetSceneState() { sceneState = SceneState::donot; }
protected:
	void UpdateFadein(bool& isFade);
	void UpdateFadeout(bool& isFade);

	void DrawFadeGraph();

	SceneManager& manager_;
	Camera& camera;
	ReuseData& reuseData;
	SceneBase* prevScene;

	SceneState sceneState;

	int fadeTimer;
	int fadeValue;

private:
	int graph;
};


