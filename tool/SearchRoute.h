#pragma once
#include "../PosData.h"
#include <vector>

class InputState;

class SearchRoute
{
public:
	SearchRoute();
	~SearchRoute() {}

	void Initialize(std::vector<PosData> posData, const int charaNum);
	void Finalize();

	/// <summary>
	/// �L�����N�^�[�̕������擾����
	/// </summary>
	/// <returns>�L�����N�^�[�̕���</returns>
	const Direct GetDirect() { return direct; }

private:
	/// <summary>
	/// ���g����ł��߂������ɂ���Ώۂ̔z��ԍ����擾����
	/// </summary>
	/// <param name="charaPos">�����ȊO�̃L�����N�^�[�̔z��ԍ�</param>
	/// <returns>�Ώۂ̔z��ԍ�</returns>
	const PosData GetNearTargetPos(std::vector<PosData> charaPos);

	/// <summary>
	/// ���g����ł��߂������ɂ���Ώۂւ̕������擾����
	/// </summary>
	/// <returns>�Ώۂւ̕���</returns>
	const Direct GetNearTargetDirect();

	/// <summary>
	/// �Ώۂ܂ł̋������擾����
	/// </summary>
	/// <param name="target">�Ώۂ̔z��ԍ�</param>
	/// <param name="source">���g�̔z��ԍ�</param>
	/// <returns>�Ώۂ܂ł̋���</returns>
	const int GetHeuristic(const PosData target, const PosData source);

	PosData searchCharaPos;
	PosData nearChara;

	Direct direct;
};

