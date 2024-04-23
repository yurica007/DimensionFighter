#pragma once
#include "../StageData.h"
#include "../character/CharaData.h"
#include "../PosData.h"
#include <DxLib.h>
#include <vector>
#include <string>

class Collision3D;
class InputState;
class Stage;
class Easing;

class CharaBase
{
public:
	CharaBase(Stage& stage);
	virtual ~CharaBase() {}

	virtual void Initialize(const std::vector<std::string> charaData, const int number, std::vector<PosData> currentIndex);
	virtual void Update(const InputState& inputState, const int time, const std::vector<PosData> currentIndex) = 0;
	virtual void Draw();
	virtual void Finalize();

	/// <summary>
	/// ���݂���z��ԍ����擾����
	/// </summary>
	/// <returns>�z��ԍ�</returns>
	const PosData GetCurrentIndex() { return ConvertPosToIndex(); }

	/// <summary>
	/// �L�����N�^�[�̏����擾����
	/// </summary>
	/// <returns>�L�����N�^�[���</returns>
	const CharaData GetData() { return chara; }

	/// <summary>
	/// ���L�����N�^�[�̔z��ԍ�������
	/// </summary>
	/// <param name="atherPos">�S�L�����N�^�[�̔z��ԍ�</param>
	/// <param name="mineNum">�����̔ԍ�</param>
	const void SetIndexAll(const std::vector<PosData> atherPos) { indexAll = atherPos; }

	/// <summary>
	/// ���L�����N�^�[��3�������W������
	/// </summary>
	/// <param name="pos">�S�L�����N�^�[�̍��W</param>
	/// <param name="rad">�S�L�����N�^�[�̔��a</param>
	const void SetPosAll(const std::vector<VECTOR> pos) { posAll = pos; }

protected:
	/// <summary>
	/// �L�����N�^�[���X�e�[�W�ォ���肷��
	/// </summary>
	/// <param name="fallData">�u���b�N�̗����f�[�^</param>
	/// <returns>true�F�X�e�[�W��ɂ��� false�F�X�e�[�W��ɂ��Ȃ�</returns>
	const bool IsOnStage();

	/// <summary>
	/// �L�����N�^�[�̗�������
	/// </summary>
	/// <param name="currentIndex">���݂̔z����W</param>
	const void UpdateFall(const std::vector<PosData> currentIndex);

	/// <summary>
	/// �L�����N�^�[���ړ��ł��邩���肷��
	/// </summary>
	/// <param name="immotileTime">�d������</param>
	/// <returns>true�F�ړ��ł��� false�F�ړ��ł��Ȃ�</returns>
	const bool IsMove(const int immotileTime);

	Stage& stage;
	CharaData chara;
	Collision3D* collision;

	std::vector<PosData> indexAll;
	int charaNum;

	std::vector<VECTOR> posAll;

	std::vector<bool> stageCollision;

	int immotileTime;

private:
	/// <summary>
	/// �L�����N�^�[���X�|�[������
	/// </summary>
	/// <param name="spawnIndexAll">�X�|�[���O�̔z����W</param>
	const void SpawnRandom(const std::vector<PosData> spawnIndexAll);

	/// <summary>
	/// 3�������W����z��ԍ��֕ϊ�����
	/// </summary>
	/// <returns>�ϊ���̔z��ԍ�</returns>
	const PosData ConvertPosToIndex();
	
	Easing* easing;
	float time;

	int stageMaxWidth;
};

