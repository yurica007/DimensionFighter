#include "Stage.h"
#include "tool/Load.h"
#include "character/CharaData.h"
#include "ReuseData.h"
#include <fstream>

namespace
{
	using namespace std;

	const char* const blockDataFilePath = "data/BlockData.csv";
	const char* const blockTextureFilePath = "data/BlockTexture.csv";

	constexpr int blockWidthNum = 8;
	constexpr int blockAllNum = blockWidthNum * blockWidthNum;

	constexpr int delayTime = 30;

	constexpr int intervalFrame = 5;

	constexpr float stageShrinkLine = -5.0f;

	enum TextureChroma
	{
		dark,
		light,
		all
	};
}

Stage::Stage() :
	deadNum(0), massNum(0), modelHandle(-1), modelScale({ 0.0f, 0.0f, 0.0f }), widthSize(blockWidthNum), narrowIndex(-1)
{
}

void Stage::Initialize(ReuseData& reuse)
{
	LoadBlockData(reuse);
	InitializeBlock();
}

void Stage::Update()
{
	//�X�e�[�W�����߂�
	if (deadNum != 0) NarrowBlock();
	else narrowIndex = -1;

	//�u���b�N�̃e�N�X�`����ύX����
	ChangeBlockTexture();

	//�X�e�[�W�𗎉�����
	FallBlock();

	//������
	UpdateBlock();

	if (narrowIndex < 0) return;
	//�k�����u���b�N�����ׂė������I�������X�e�[�W�T�C�Y���X�V����
	if (block[narrowIndex].isNone && block[narrowIndex].isFalling) widthSize = blockWidthNum - deadNum * 2;
}

void Stage::Draw()
{
	for (int i = 0; i < blockAllNum; i++)
	{
		if (!block[i].isDelete) MV1DrawModel(block[i].handle);
	}
}

void Stage::Finalize()
{
	for (int i = 0; i < blockAllNum; i++)
	{
		MV1DeleteModel(block[i].handle);
	}
}

void Stage::PrevBlock(const float rotateY, const PosData posIndex, const CharaPower charaType, const int charaNum)
{
	if (rotateY < 0.0f) return;
	
	const int indexX = posIndex.x, indexY = posIndex.y;
	const int currentIndex = posIndex.y * blockWidthNum + indexX;

	if (charaType == CharaPower::one)
	{
		if (indexX == 0 && rotateY == leftVec) return;
		if (indexX == blockWidthNum - 1 && rotateY == rightVec) return;
		if (indexY == 0 && rotateY == frontVec) return;
		if (indexY == blockWidthNum - 1 && rotateY == backVec) return;

		int fallIndex = currentIndex;

		if (rotateY == leftVec)	//������
		{
			fallIndex -= 1;
		}
		if (rotateY == rightVec)//�E����
		{
			fallIndex += 1;
		}
		if (rotateY == backVec)	//������
		{
			fallIndex += blockWidthNum;
		}
		if (rotateY == frontVec)//��O����
		{
			fallIndex -= blockWidthNum;
		}

		if (block[fallIndex].isPrepareFall || block[fallIndex].isFalling) return;
		if (block[fallIndex].isNone || block[fallIndex].isDelete) return;

		block[fallIndex].isPrepareFall = true;
		block[fallIndex].charaNum = charaNum;
		block[fallIndex].changeTexTimer = 5;
		block[fallIndex].fallDelayTimer = 15;
	}

	if (charaType == CharaPower::line)
	{
		if (indexX == 0 && rotateY == leftVec) return;
		if (indexX == blockWidthNum - 1 && rotateY == rightVec) return;
		if (indexY == 0 && rotateY == frontVec) return;
		if (indexY == blockWidthNum - 1 && rotateY == backVec) return;

		vector<int> fallIndex;

		if (rotateY == leftVec)	//������
		{
			for (int i = 0; i < indexX; i++)
			{
				int index = currentIndex - (1 + i);
				fallIndex.push_back(index);
			}
		}
		if (rotateY == rightVec)//�E����
		{
			for (int i = 0; i < blockWidthNum - (indexX + 1); i++)
			{
				int index = currentIndex + (1 + i);
				fallIndex.push_back(index);
			}
		}
		if (rotateY == backVec)	//������
		{
			for (int i = 0; i < blockWidthNum - (indexY + 1); i++)
			{
				int index = currentIndex + (1 + i) * blockWidthNum;
				fallIndex.push_back(index);
			}
		}
		if (rotateY == frontVec)//��O����
		{
			for (int i = 0; i < indexY; i++)
			{
				int index = currentIndex - (1 + i) * blockWidthNum;
				fallIndex.push_back(index);
			}
		}

		for (int i = 0; i < fallIndex.size(); i++)
		{
			const int index = fallIndex[i];
			if (index < 0) continue;
			if (blockAllNum <= index) continue;
			if (block[index].isPrepareFall || block[index].isFalling) continue;
			if (block[index].isNone || block[index].isDelete) continue;

			block[index].isPrepareFall = true;
			block[index].charaNum = charaNum;
			block[index].changeTexTimer = i * intervalFrame;
			block[index].fallDelayTimer = static_cast<int>(fallIndex.size()) * intervalFrame;
		}
	}

	if (charaType == CharaPower::row)
	{
		if (indexX == 0 && rotateY == leftVec) return;
		if (indexX == blockWidthNum - 1 && rotateY == rightVec) return;
		if (indexY == 0 && rotateY == frontVec) return;
		if (indexY == blockWidthNum - 1 && rotateY == backVec) return;

		vector<int> fallIndex;

		if (rotateY == leftVec)	//������
		{
			if (currentIndex / blockWidthNum != 0) fallIndex.push_back(currentIndex - 1 - blockWidthNum);
			fallIndex.push_back(currentIndex - 1);
			if (currentIndex / blockWidthNum != blockWidthNum - 1) fallIndex.push_back(currentIndex - 1 + blockWidthNum);
		}
		if (rotateY == rightVec)//�E����
		{
			if (currentIndex / blockWidthNum != blockWidthNum - 1) fallIndex.push_back(currentIndex + 1 + blockWidthNum);
			fallIndex.push_back(currentIndex + 1);
			if (currentIndex / blockWidthNum != 0) fallIndex.push_back(currentIndex + 1 - blockWidthNum);
		}
		if (rotateY == backVec)	//������
		{
			if (currentIndex % blockWidthNum != 0) fallIndex.push_back(currentIndex + blockWidthNum - 1 );
			fallIndex.push_back(currentIndex + blockWidthNum);
			if (currentIndex % blockWidthNum != blockWidthNum - 1) fallIndex.push_back(currentIndex + blockWidthNum + 1 );
			if (currentIndex % blockWidthNum != blockWidthNum - 1) fallIndex.push_back(currentIndex + blockWidthNum + 1 );
		}
		if (rotateY == frontVec)//��O����
		{
			if (currentIndex % blockWidthNum != blockWidthNum - 1) fallIndex.push_back(currentIndex - blockWidthNum + 1);
			fallIndex.push_back(currentIndex - blockWidthNum);
			if (currentIndex % blockWidthNum != 0) fallIndex.push_back(currentIndex - blockWidthNum - 1);
		}

		for (int i = 0; i < fallIndex.size(); i++)
		{
			const int index = fallIndex[i];
			if (index < 0) continue;
			if (blockAllNum <= index) continue;
			if (block[index].isPrepareFall || block[index].isFalling) continue;
			if (block[index].isNone || block[index].isDelete) continue;

			block[index].isPrepareFall = true;
			block[index].charaNum = charaNum;
			block[index].changeTexTimer = i * intervalFrame;
			block[index].fallDelayTimer = static_cast<int>(fallIndex.size()) * intervalFrame;
		}
	}

	if (charaType == CharaPower::round)
	{
		vector<int> fallIndex;

		if (currentIndex % blockWidthNum != 0)
		{
			if (currentIndex / blockWidthNum != blockWidthNum - 1) fallIndex.push_back(currentIndex - 1 + blockWidthNum);
			if (currentIndex / blockWidthNum != 0) fallIndex.push_back(currentIndex - 1 - blockWidthNum);
		}
		if (currentIndex % blockWidthNum != blockWidthNum - 1)
		{
			if (currentIndex / blockWidthNum != blockWidthNum - 1) fallIndex.push_back(currentIndex + 1 + blockWidthNum);
			if (currentIndex / blockWidthNum != 0) fallIndex.push_back(currentIndex + 1 - blockWidthNum);
		}

		for (int i = 0; i < fallIndex.size(); i++)
		{
			const int index = fallIndex[i];
			if (block[index].isPrepareFall || block[index].isFalling) continue;
			if (block[index].isNone || block[index].isDelete) continue;

			block[index].isPrepareFall = true;
			block[index].charaNum = charaNum;
			block[index].changeTexTimer = i * intervalFrame;
			block[index].fallDelayTimer = static_cast<int>(fallIndex.size()) * intervalFrame;
		}
	
	}
}

const vector<BlockIndexData> Stage::GetBlockData()
{
	vector<BlockIndexData> blockData;

	for (int i = 0; i < block.size(); i++)
	{
		BlockIndexData indexData;

		if (block[i].isFalling || block[i].isDelete)
		{
			const int indexX = static_cast<int>(block[i].pos.x - 640.0f - modelScale.x / 2) + blockWidthNum / 2;
			const int indexY = static_cast<int>(block[i].pos.z - modelScale.z / 2) + blockWidthNum / 2;

			indexData.index = { indexX, indexY };
			indexData.fallNum = block[i].charaNum;
			indexData.isFall = block[i].isFall;
			indexData.isNone = block[i].isNone;

			blockData.push_back(indexData);
		}
	}

	return blockData;
}

void Stage::LoadBlockData(ReuseData& reuseData)
{
	//���ʂ̃f�[�^�̓Ǎ�
	{
		Load* load = new Load();
		ifstream ifs(blockDataFilePath);
		string line;

		vector<vector<string>> texture;
		while (getline(ifs, line))
		{
			const vector<string> strvec = load->LoadCSVData(line, ',');
			const int loadLine = load->GetLoadLine();

			//�������͏Ȃ�
			if (loadLine == commentLine) continue;
			texture.push_back(strvec);
		}
		delete load;

		modelHandle = MV1LoadModel(texture.front().front().c_str());
		
		for (int i = 0; i < texture.size(); i++)
		{
			baseTextureHandle.push_back(LoadGraph(texture[i].back().c_str()));
		}
	}

	{
		vector<int> handle;
		Load* load = new Load();
		load->LoadGraphFile(blockTextureFilePath, handle);
		delete load;

		for (int i = 0; i < charaNumAll; i++)
		{
			const int darkTex = i * 2;
			colorTextureHandle.push_back(handle[darkTex]);

			const int lightTex = darkTex + 1;
			colorTextureHandle.push_back(handle[lightTex]);
		}
	}
}

void Stage::InitializeBlock()
{
	modelScale = MV1GetScale(modelHandle);

	//�X�e�[�W�̒��S����ʒ����ɗ���悤�ɒ���
	const float size = modelScale.x / 2.0f - static_cast<float>(blockWidthNum / 2);
	const VECTOR initPos = { size + 640.0f, 360.0f, size + 0.0f };

	//���W���Z�b�g
	VECTOR pos = initPos;
	const VECTOR widthSpace = { modelScale.x, 0.0f, 0.0f };
	const VECTOR depthSpace = { 0.0f, 0.0f, modelScale.z };

	block.resize(blockAllNum);
	for (int i = 0; i < blockAllNum; i++)
	{
		//�R�s�[���f�����쐬
		const int duplicateModel = MV1DuplicateModel(modelHandle);
		block[i].handle = duplicateModel;

		if (i % blockWidthNum == 0)
		{
			//�E�[�܂ŕ~���l�߂��獶�[�ɖ߂�AZ+�����Ƀ��f��1�����炷
			pos = initPos;
			const float depthNum = static_cast<float>(i / blockWidthNum);
			pos = VAdd(pos, VScale(depthSpace, depthNum));
		}
		//�u���b�N��~���l�߂�
		block[i].pos = pos;
		pos = VAdd(pos, widthSpace);
		MV1SetPosition(block[i].handle, block[i].pos);

		const bool isColorLight = (i / blockWidthNum % 2 == i % blockWidthNum % 2);

		//�u���b�N�̐F���s���͗l�ɂ���
		if (isColorLight) block[i].texture = baseTextureHandle[TextureChroma::dark];
		else block[i].texture = baseTextureHandle[TextureChroma::light];

		MV1SetTextureGraphHandle(duplicateModel, 0, block[i].texture, false);
	}
}

const void Stage::NarrowBlock()
{
	//���߂�u���b�N���擾
	vector<int> leftLine, rightLine, backLine, frontLine;
	for (int i = 0; i < blockAllNum; i++)
	{
		if (!IsNarrowBlock(i) || block[i].isNone) continue;

		const int indexX = i % blockWidthNum;
		const int indexY = i / blockWidthNum;

		//4�ӂ��ۂ��̃`�F�b�N
		const bool isLeftLine = indexX == deadNum - 1;
		const bool isRightLine = indexX == blockWidthNum - deadNum;
		const bool isBackLine = indexY == blockWidthNum - deadNum;
		const bool isFrontLine = indexY == deadNum - 1;
		
		if (isBackLine && !isRightLine) backLine.push_back(i);			//����
		else if (isRightLine && !isFrontLine) rightLine.push_back(i);	//�E��	
		else if (isFrontLine && !isLeftLine) frontLine.push_back(i);	//��O��		
		else if (isLeftLine && !isBackLine) leftLine.push_back(i);		//����
		else continue;
	}

	if (leftLine.size() == 0 && rightLine.size() == 0 && frontLine.size() == 0 && backLine.size() == 0) return;

	//���ォ�玞�v���̏��ɔz��ɓ���Ă���
	vector<int> fallAll;
	for (int i = 0; i < backLine.size(); i++)
	{
		fallAll.push_back(backLine[i]);
	}
	for (int i = static_cast<int>(rightLine.size()) - 1; 0 <= i ; i--)
	{
		fallAll.push_back(rightLine[i]);
	}
	for (int i = static_cast<int>(frontLine.size()) - 1; 0 <= i ; i--)
	{
		fallAll.push_back(frontLine[i]);
	}
	for (int i = 0; i < leftLine.size(); i++)
	{
		fallAll.push_back(leftLine[i]);
	}

	for (int i = 0; i < fallAll.size(); i++)
	{
		const int index = fallAll[i];

		block[index].isPrepareFall = true;
		block[index].isNone = true;
		block[index].changeTexTimer = i * intervalFrame;
		block[index].fallDelayTimer = (static_cast<int>(fallAll.size()) - i) * intervalFrame;

		narrowIndex = fallAll[i];
	}
}

const bool Stage::IsNarrowBlock(const int index)
{
	if (index % blockWidthNum < deadNum) return true;
	if (index / blockWidthNum < deadNum) return true;

	if (blockWidthNum - deadNum <= index % blockWidthNum) return true;
	if (blockWidthNum - deadNum <= index / blockWidthNum) return true;

	return false;
}

const void Stage::ChangeBlockTexture()
{
	for (int i = 0; i < blockAllNum; i++)
	{
		if (!block[i].isPrepareFall || block[i].isFalling) continue;

		if (--block[i].changeTexTimer < 0)
		{
			DecideTexture(block[i].isNone, i);
			block[i].isFalling = true;
			block[i].changeTexTimer = 0;
		}
	}
}

void Stage::DecideTexture(const bool isNone, const int index)
{
	//�X�e�[�W���k��
	if (isNone)
	{
		int num = TextureChroma::all;
		if (block[index].texture == baseTextureHandle[TextureChroma::dark]) num += TextureChroma::dark;
		else num += TextureChroma::light;

		MV1SetTextureGraphHandle(block[index].handle, 0, baseTextureHandle[num], false);
	}
	//�ʏ펞
	else
	{
		int charaNum = block[index].charaNum * 2;
		if (block[index].texture == baseTextureHandle[TextureChroma::light]) charaNum += 1;

		MV1SetTextureGraphHandle(block[index].handle, 0, colorTextureHandle[charaNum], false);
	}
}

const void Stage::FallBlock()
{
	for (int i = 0; i < blockAllNum; i++)
	{
		//������ԂɂȂ��u���b�N�͏������s��Ȃ�
		if (!block[i].isFalling) continue;
		if (0 < --block[i].fallDelayTimer) continue;

		block[i].isPrepareFall = false;
		block[i].pos = VAdd(block[i].pos, gravity);
		MV1SetPosition(block[i].handle, block[i].pos);

		block[i].fallDelayTimer = 0;
		block[i].isFall = true;

		//�������Ƀ��f�����k������
		if (stageShrinkLine < block[i].pos.y) continue;

		const float dist = block[i].pos.y - stageDeadLine;
		const VECTOR scele = VScale(modelScale, dist / -stageShrinkLine);
		MV1SetScale(block[i].handle, scele);
	}
}

const void Stage::UpdateBlock()
{
	for (int i = 0; i < blockAllNum; i++)
	{
		if (stageDeadLine < block[i].pos.y) continue;

		//�k�����ȊO�̗��������u���b�N�����ɖ߂�
		if (!block[i].isNone)
		{
			block[i].pos.y = 360.0f;
			block[i].isFall = false;
		}
		else block[i].isDelete = true;

		block[i].isFalling = false;

		MV1SetPosition(block[i].handle, block[i].pos);
		MV1SetScale(block[i].handle, modelScale);
		MV1SetTextureGraphHandle(block[i].handle, 0, block[i].texture, false);
	}
}