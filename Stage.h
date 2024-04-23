#pragma once
#include "StageData.h"
#include "PosData.h"
#include "character/CharaData.h"
#include <vector>
#include <DxLib.h>

struct BlockData
{
	int handle = -1;
	VECTOR pos = { 0.0f, 0.0f, 0.0f };

	int texture = -1;

	bool isPrepareFall = false;
	bool isFall = false;
	bool isFalling = false;
	bool isNone = false;
	bool isDelete = false;

	int charaNum = -1;
	int changeTexTimer = 0;
	int fallDelayTimer = 0;
};

class ReuseData;

class Stage
{
public:
	Stage();
	~Stage() {}

	void Initialize(ReuseData& reuse);
	void Update();
	void Draw();
	void Finalize();

	const int GetStageSize() { return widthSize; }

	void PrevBlock(const float rotateY, const PosData posIndex, const CharaPower charaType, const int charaNum);

	const std::vector<BlockIndexData> GetBlockData();

	/// <summary>
	/// ���񂾃L�����N�^�[��������
	/// </summary>
	/// <param name="deadCharaNum">���񂾃L�����N�^�[��</param>
	const void SetDeadCharaNum(const int deadCharaNum) { deadNum = deadCharaNum; }

private:
	/// <summary>
	/// �K�v�ȃu���b�N�f�[�^��Ǎ���
	/// </summary>
	void LoadBlockData(ReuseData& reuseData);

	/// <summary>
	/// �u���b�N�f�[�^������������
	/// </summary>
	void InitializeBlock();

	/// <summary>
	/// ���񂾃L�����N�^�[���ɉ����ău���b�N�����߂�
	/// </summary>
	const void NarrowBlock();

	/// <summary>
	/// ���߂�ׂ��u���b�N�����肷��
	/// </summary>
	/// <param name="index">�z��ԍ�</param>
	/// <returns>true�F���߂� false�F���߂Ȃ�</returns>
	const bool IsNarrowBlock(const int index);

	/// <summary>
	/// �u���b�N�̗������Ƀe�N�X�`����ς���
	/// </summary>
	const void ChangeBlockTexture();

	/// <summary>
	/// �ς���e�N�X�`�������肷��
	/// </summary>
	/// <param name="isNone">���߂�u���b�N���ۂ�</param>
	/// <param name="index">�z��ԍ�</param>
	void DecideTexture(const bool isNone, const int index);

	/// <summary>
	/// �u���b�N�𗎉�������
	/// </summary>
	const void FallBlock();
		
	/// <summary>
	/// �u���b�N�̗������I�������̏���
	/// </summary>
	const void UpdateBlock();

	std::vector<BlockData> block;
	std::vector<int> baseTextureHandle;

	std::vector<int> colorTextureHandle;

	int deadNum;
	int massNum;

	int narrowIndex;
	int widthSize;

	int modelHandle;
	VECTOR modelScale;
};

