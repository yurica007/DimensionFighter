#pragma once
#include "SceneBase.h"
#include "../TimeData.h"
#include <vector>

class BackGround;
class Stage;
class CharaManager;

class UIUpdateTime;
class UIClearTime;
class UILifeManager;
class UICharaType;

class SceneGame : public SceneBase
{
public:
	SceneGame(SceneManager& manager, Camera& camera, ReuseData& reuse);
	virtual ~SceneGame() {}

	virtual void Initialize() override;
	virtual void Update(const InputState& inputState) override;
	virtual void Draw() override;
	virtual void Finalize() override;

private:
	/// <summary>
	/// �o�ߎ��Ԃ��X�V����
	/// </summary>
	void UpdateTime();

	/// <summary>
	/// �L�����N�^�[�̏��ʕt��������
	/// </summary>
	/// <returns>true�F���ʕt���I�� false�F���ʕt����</returns>
	bool IsRankChara();

	BackGround* backGround;
	Stage* stage;
	CharaManager* chara;
	UIUpdateTime* updateTime;
	UIClearTime* clearTime;
	UILifeManager* lifes;
	UICharaType* charaType;

	Time currentTime;

	int startCount;
	int countDown;

	std::vector<int> rank;

	int screenGraph;
	bool isFade;
};

