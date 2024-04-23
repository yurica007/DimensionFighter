#pragma once
#include "../character/CharaData.h"
#include "../StageData.h"
#include "../PosData.h"
#include <vector>
#include <DxLib.h>

class CharaBase;
class ReuseData;
class InputState;
class Stage;

class CharaManager
{
public:
	CharaManager(Stage& stage);
	~CharaManager() {}

	void Initialize(ReuseData& reuseData);
	void Update(const InputState& inputState, const int time);
	void Draw();
	void Finalize();

	/// <summary>
	/// �S�L�����N�^�[�̔z��ԍ����擾����
	/// </summary>
	/// <returns>�S�L�����N�^�[�̔z��ԍ�</returns>
	const std::vector<PosData> GetCharaIndex();

	/// <summary>
	/// �c��L�����N�^�[�����擾����
	/// </summary>
	/// <returns>�c��L�����N�^�[��</returns>
	const int GetRemainCharaNum();

	/// <summary>
	/// �S�L�����N�^�[�̗̑͂��擾����
	/// </summary>
	/// <returns>�S�L�����N�^�[�̗̑�</returns>
	const std::vector<int> GetCharaHP();

	const std::vector<bool> GetIsOnStage();

	/// <summary>
	/// �S�L�����N�^�[��3�������W���擾����
	/// </summary>
	/// <returns>�S�L�����N�^�[��3�������W</returns>
	const std::vector<VECTOR> GetCharaPos();
protected:
	Stage& stage;

private:
	/// <summary>
	/// �L�����N�^�[�̃f�[�^�̂�ۑ�����
	/// </summary>
	/// <param name="data">�ۑ�����f�[�^</param>
	/// <param name="num">�L�����N�^�[�̔ԍ�</param>
	void KeepCharaData(const CharaData data, const int num) { characterData[num] = data; }

	std::vector<CharaBase*> chara;

	std::vector<CharaData> characterData;

	int deadCharaNum;
};

