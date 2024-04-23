#pragma once
#include "../StageData.h"
#include "../character/CharaData.h"
#include "../PosData.h"
#include <DxLib.h>
#include <vector>

struct SphereData
{
	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	double radius = 0.0;
};

class InputState;
class Stage;

class Collision3D
{
public:
	Collision3D(Stage& stage);
	~Collision3D() {}

	void Initialize();
	void Update(const VECTOR pos);
	void Draw();
	void Finalize();

	/// <summary>
	/// �X�e�[�W�̓����蔻��͈͂��X�V����
	/// </summary>
	void UpdateCollRange();

	/// <summary>
	/// �X�e�[�W�Ƃ̓����蔻��
	/// </summary>
	/// <returns>true�F�������Ă��� false�F�������Ă��Ȃ�</returns>
	const std::vector<bool> IsCollideStage();

private:
	/// <summary>
	/// �X�e�[�W�[�̓����蔻��͈͂��X�V����
	/// </summary>
	/// <param name="deadNum">���񂾃L�����N�^�[��</param>
	void UpdateOutsideVerPos(const int decreaseWidth);

	/// <summary>
	/// �����蔻��Ƃ̍ŒZ�����ƃL�����N�^�[�̔��a���r����
	/// </summary>
	/// <param name="vertexPos">�����蔻��̒��_���</param>
	/// <param name="direct">�������Ă������</param>
	/// <param name="collide">�����蔻��</param>
	void ComparisonDistanceAndRadius(const std::vector<VECTOR> vertexPos, Direct& direct, std::vector<bool>& collide);

	/// <summary>
	/// �ŒZ�������擾����
	/// </summary>
	/// <param name="vertexPos">�����蔻��̒��_���</param>
	/// <param name="direct">�������Ă������</param>
	/// <returns>�ŒZ����</returns>
	const float GetShortDistance(const std::vector<VECTOR> vertexPos, Direct& direct);

	/// <summary>
	/// 3�����x�N�g����ݏ悵�����v���擾����
	/// </summary>
	/// <param name="dist">3�����x�N�g��</param>
	/// <returns>�x�N�g���̍��v</returns>
	const float GetVector(const VECTOR dist);

	/// <summary>
	/// ���Ƌ��Ƃ̓����蔻��
	/// </summary>
	/// <param name="targetPos">�Ώۂ̋��̃f�[�^</param>
	/// <param name="sourcePos">���g�̋��̃f�[�^</param>
	/// <returns>true�F�������Ă��� false�F�������Ă��Ȃ�</returns>
	const bool CollSphereAndSpher(const SphereData targeSphere, const SphereData sourceSphere);

	SphereData sphere;
	Stage& stage;

	int prevStageSize;

	std::vector<std::vector<VECTOR>> insideVerPos;
	std::vector<std::vector<VECTOR>> outsideVerPos;
};

