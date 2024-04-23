#include "Collision3D.h"
#include "../Stage.h"
#include "InputState.h"

namespace
{
	constexpr VECTOR cubeScale = { 1.0f, 1.0f, 1.0f };

	constexpr VECTOR collInitPos = { -4.0f + 640.0f, 0.5f + 360.0f, -4.0f };
	constexpr VECTOR errorPos = { 4.0f + 640.0f, 0.5f + 360.0f, 4.0f };

	constexpr VECTOR collInitLeftPos = { -5.0f, 0.5f, -4.0f };
	constexpr VECTOR collInitRightPos = { 4.0f, 0.5f, -4.0f };
	constexpr VECTOR collInitBackPos = { -4.0f, 0.5f, 4.0f };
	constexpr VECTOR collInitFrontPos = { -4.0f, 0.5f, -5.0f };

	constexpr int stageSize = 8;
	constexpr int collStageSize = stageSize + 2;

	constexpr int powerNum = 2;

	using namespace std;

	enum Vertex
	{
		frontLeftDown,
		backRightUp,
		all
	};

	enum Vector
	{
		width,
		height,
		depth
	};

	struct Fall
	{
		vector<float> pos;
		int fallNum = -1;
	};
}

Collision3D::Collision3D(Stage& stage) :
	stage(stage), prevStageSize(0)
{
}

void Collision3D::Initialize()
{
	sphere.pos = { 0.0f, 0.0f, 0.0f };
	sphere.radius = modelScale;
		
	outsideVerPos.resize(DirectNum);

	for (auto& verPos : outsideVerPos)
	{
		verPos.resize(Vertex::all);
	}

	UpdateOutsideVerPos(0);

	prevStageSize = stage.GetStageSize();
}

void Collision3D::Update(const VECTOR pos)
{
	sphere.pos = pos;
}

void Collision3D::Draw()
{
	for (int i = 0; i < insideVerPos.size(); i++)
	{
		DrawCube3D(insideVerPos[i][Vertex::frontLeftDown], insideVerPos[i][Vertex::backRightUp], 0xffffff, 0xffffff, false);
	}

	for (int i = 0; i < outsideVerPos.size(); i++)
	{
		DrawCube3D(outsideVerPos[i][Vertex::frontLeftDown], outsideVerPos[i][Vertex::backRightUp], 0xffffff, 0xffffff, false);
	}
}

void Collision3D::Finalize()
{
}

void Collision3D::UpdateCollRange()
{
	vector<BlockIndexData> fallData = stage.GetBlockData();
	insideVerPos.clear();

	vector<vector<VECTOR>> vertexData;
	VECTOR frontPos = { 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < fallData.size(); i++)
	{
		if (fallData[i].isNone) continue;

		const float posX = fallData[i].index.x + collInitPos.x;
		const float posY = collInitPos.y;
		const float posZ = fallData[i].index.y + collInitPos.z;

		const VECTOR pos = { posX, posY, posZ };
		vector<VECTOR> vertexPos;

		//��x�����蔻����Ŏ��ēx��Ŏ��Ƃ������蔻�肪�o�O��
		if (false)
		{
			if (fallData[i].fallNum == static_cast<int>(CharaPower::line) ||
				fallData[i].fallNum == static_cast<int>(CharaPower::row))
			{
				if (fallData.size() < Vertex::all)
				{
					vertexPos.push_back(pos);
					vertexPos.push_back(VAdd(pos, { 1.0f, 1.0f, 1.0f }));
					vertexData.push_back(vertexPos);
				}
				else
				{
					if (i == 0) frontPos = pos;
					vertexPos.push_back(frontPos);
					vertexPos.push_back(VAdd(pos, { 1.0f, 1.0f, 1.0f }));

					if (i == fallData.size() - 1) vertexData.push_back(vertexPos);
				}
			}
			else
			{
				vertexPos.push_back(pos);
				vertexPos.push_back(VAdd(pos, { 1.0f, 1.0f, 1.0f }));
				vertexData.push_back(vertexPos);
			}
		}

		vertexPos.push_back(pos);
		vertexPos.push_back(VAdd(pos, { 1.0f, 1.0f, 1.0f }));
		vertexData.push_back(vertexPos);
	}	
	insideVerPos = vertexData;

	//�X�e�[�W�g�̓����蔻��
	const int currentStageSize = stage.GetStageSize();
	if (prevStageSize != currentStageSize)
	{
		const int decreaseWidth = (prevStageSize - currentStageSize) / 2;
	
		UpdateOutsideVerPos(decreaseWidth);
	}
}

const vector<bool> Collision3D::IsCollideStage()
{
	vector<bool> collide;
	collide.resize(4);

	Direct direct = Direct::left;
	
	//�X�e�[�W�O���̓����蔻��
	for (int i = 0; i < outsideVerPos.size(); i++)
	{
		vector<VECTOR> vertexPos;

		//���ʍ������W
		vertexPos.push_back(outsideVerPos[i][Vertex::frontLeftDown]);
		//���ʉE����W
		vertexPos.push_back(outsideVerPos[i][Vertex::backRightUp]);

		ComparisonDistanceAndRadius(vertexPos, direct, collide);
	}

	//�X�e�[�W���̓����蔻��
	for (int i = 0; i < insideVerPos.size(); i++)
	{
		vector<VECTOR> vertexPos;

		//���ʍ������W
		vertexPos.push_back(insideVerPos[i][Vertex::frontLeftDown]);
		//���ʉE����W
		vertexPos.push_back(insideVerPos[i][Vertex::backRightUp]);

		ComparisonDistanceAndRadius(vertexPos, direct, collide);
	}
	return collide;
}

void Collision3D::UpdateOutsideVerPos(const int decreaseWidth)
{
	const float size = static_cast<float>(decreaseWidth);

	//�c�����ꂼ�ꗼ�����猸�炷
	const float stageWidth = static_cast<float>(stageSize - decreaseWidth * 2);

	//���炵��������͈͂��X�V
	for (int i = 0; i < outsideVerPos.size(); i++)
	{
		VECTOR frontPos = { 0.0f, 0.0f, 0.0f };
		VECTOR backPos = { 0.0f, 0.0f, 0.0f };
		VECTOR verSize = cubeScale;
		VECTOR frontShrink = { 0.0f, 0.0f, 0.0f };

		if (i == leftDirect)
		{
			frontPos = VAdd(collInitLeftPos, { 640.0f, 360.0f, 0.0f });
			verSize.z = stageWidth;
			frontShrink = { size, 0.0f, size };
		}
		if (i == rightDirect)
		{
			frontPos = VAdd(collInitRightPos, { 640.0f, 360.0f, 0.0f });
			verSize.z = stageWidth;
			frontShrink = { -size, 0.0f, size };
		}
		if (i == backDirect)
		{
			frontPos = VAdd(collInitBackPos, { 640.0f, 360.0f, 0.0f });
			verSize.x = stageWidth;
			frontShrink = { size, 0.0f, -size };
		}
		if (i == frontDirect)
		{
			frontPos = VAdd(collInitFrontPos, { 640.0f, 360.0f, 0.0f });
			verSize.x = stageWidth;
			frontShrink = { size, 0.0f, size };
		}

		frontPos = VAdd(frontPos, frontShrink);
		backPos = VAdd(frontPos, verSize);

		outsideVerPos[i][Vertex::frontLeftDown] = frontPos;
		outsideVerPos[i][Vertex::backRightUp] = backPos;
	}
}

void Collision3D::ComparisonDistanceAndRadius(const vector<VECTOR> vertexPos, Direct& direct, vector<bool>& collide)
{
	//�擾���������̐�Βl�����߂�
	float distance = GetShortDistance(vertexPos, direct);
	distance = fabsf(distance);

	const float rad = static_cast<float>(sphere.radius * sphere.radius);

	//�ŒZ�����Ƌ��̔��a���r���A�������Ă��邩����
	const int dir = static_cast<int>(direct);
	if (distance < rad) collide[dir] = true;
}

const float Collision3D::GetShortDistance(const vector<VECTOR> vertexPos, Direct& direct)
{
	VECTOR dist = { 0.0f, 0.0f, 0.0f };

	//������
	if (sphere.pos.x <= vertexPos[Vertex::frontLeftDown].x)
	{
		dist.x = (vertexPos[Vertex::frontLeftDown].x - sphere.pos.x);
		direct = Direct::right;
	}
	//�����E			
	else if (vertexPos[Vertex::backRightUp].x <= sphere.pos.x)
	{
		dist.x = (vertexPos[Vertex::backRightUp].x - sphere.pos.x);
		direct = Direct::left;
	}
	else dist.x = 0.0f;

	//������
	if (sphere.pos.y <= vertexPos[Vertex::frontLeftDown].y)
	{
		dist.y = (vertexPos[Vertex::frontLeftDown].y - sphere.pos.y);
	}
	//������
	else if (vertexPos[Vertex::backRightUp].y <= sphere.pos.y)
	{
		dist.y = (vertexPos[Vertex::backRightUp].y - sphere.pos.y);
	}
	else dist.y = 0.0f;

	//�����O
	if (sphere.pos.z <= vertexPos[Vertex::frontLeftDown].z)
	{
		dist.z = (vertexPos[Vertex::frontLeftDown].z - sphere.pos.z);
		direct = Direct::back;
	}
	//������
	else if (vertexPos[Vertex::backRightUp].z <= sphere.pos.z)
	{
		dist.z = (vertexPos[Vertex::backRightUp].z - sphere.pos.z);
		direct = Direct::front;
	}
	else dist.z = 0.0f;

	return GetVector(dist);
}

const float Collision3D::GetVector(const VECTOR dist)
{
	const float vecX = powf(dist.x, powerNum);
	const float vecY = powf(dist.y, powerNum);
	const float vecZ = powf(dist.z, powerNum);

	return vecX + vecY + vecZ;
}

const bool Collision3D::CollSphereAndSpher(const SphereData targeSphere, const SphereData sourceSphere)
{
	VECTOR dist = { 0.0f, 0.0f , 0.0f };
	dist.x = targeSphere.pos.x - sourceSphere.pos.x;
	dist.y = targeSphere.pos.y - sourceSphere.pos.y;
	dist.z = targeSphere.pos.z - sourceSphere.pos.z;

	const float radDist = static_cast<float>(pow((targeSphere.radius + sourceSphere.radius), powerNum));

	//��_�Ԃ̃x�N�g���Ɠ�̋��̔��a�̘a���r
	if (fabsf(GetVector(dist)) < fabsf(radDist)) return true;
	return false;
}
