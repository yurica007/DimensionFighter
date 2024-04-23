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

		//一度当たり判定を列で取り再度列で取るとき当たり判定がバグる
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

	//ステージ枠の当たり判定
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
	
	//ステージ外側の当たり判定
	for (int i = 0; i < outsideVerPos.size(); i++)
	{
		vector<VECTOR> vertexPos;

		//正面左下座標
		vertexPos.push_back(outsideVerPos[i][Vertex::frontLeftDown]);
		//裏面右上座標
		vertexPos.push_back(outsideVerPos[i][Vertex::backRightUp]);

		ComparisonDistanceAndRadius(vertexPos, direct, collide);
	}

	//ステージ内の当たり判定
	for (int i = 0; i < insideVerPos.size(); i++)
	{
		vector<VECTOR> vertexPos;

		//正面左下座標
		vertexPos.push_back(insideVerPos[i][Vertex::frontLeftDown]);
		//裏面右上座標
		vertexPos.push_back(insideVerPos[i][Vertex::backRightUp]);

		ComparisonDistanceAndRadius(vertexPos, direct, collide);
	}
	return collide;
}

void Collision3D::UpdateOutsideVerPos(const int decreaseWidth)
{
	const float size = static_cast<float>(decreaseWidth);

	//縦横それぞれ両側から減らす
	const float stageWidth = static_cast<float>(stageSize - decreaseWidth * 2);

	//減らした分判定範囲を更新
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
	//取得した距離の絶対値を求める
	float distance = GetShortDistance(vertexPos, direct);
	distance = fabsf(distance);

	const float rad = static_cast<float>(sphere.radius * sphere.radius);

	//最短距離と球の半径を比較し、当たっているか判定
	const int dir = static_cast<int>(direct);
	if (distance < rad) collide[dir] = true;
}

const float Collision3D::GetShortDistance(const vector<VECTOR> vertexPos, Direct& direct)
{
	VECTOR dist = { 0.0f, 0.0f, 0.0f };

	//球が左
	if (sphere.pos.x <= vertexPos[Vertex::frontLeftDown].x)
	{
		dist.x = (vertexPos[Vertex::frontLeftDown].x - sphere.pos.x);
		direct = Direct::right;
	}
	//球が右			
	else if (vertexPos[Vertex::backRightUp].x <= sphere.pos.x)
	{
		dist.x = (vertexPos[Vertex::backRightUp].x - sphere.pos.x);
		direct = Direct::left;
	}
	else dist.x = 0.0f;

	//球が下
	if (sphere.pos.y <= vertexPos[Vertex::frontLeftDown].y)
	{
		dist.y = (vertexPos[Vertex::frontLeftDown].y - sphere.pos.y);
	}
	//球が上
	else if (vertexPos[Vertex::backRightUp].y <= sphere.pos.y)
	{
		dist.y = (vertexPos[Vertex::backRightUp].y - sphere.pos.y);
	}
	else dist.y = 0.0f;

	//球が前
	if (sphere.pos.z <= vertexPos[Vertex::frontLeftDown].z)
	{
		dist.z = (vertexPos[Vertex::frontLeftDown].z - sphere.pos.z);
		direct = Direct::back;
	}
	//球が奥
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

	//二点間のベクトルと二つの球の半径の和を比較
	if (fabsf(GetVector(dist)) < fabsf(radDist)) return true;
	return false;
}
