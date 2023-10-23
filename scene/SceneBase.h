#pragma once

class SceneManager;
class InputState;

class SceneBase
{
public:
	SceneBase(SceneManager& manager);
	virtual ~SceneBase();

	virtual void Initialize();
	virtual void Update(const InputState& input);
	virtual void Draw();
	virtual void Finalize();

protected:
	void FadeinUpdate();
	void FadeoutUpdate();

	virtual bool IsTransScene(const InputState& inputState) { return false; }

	SceneManager& manager_;
	int fadeTimer;
	int fadeValue;
};


