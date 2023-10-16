#include "Stage.h"
#include <sstream>
#include <fstream>

namespace
{
	const char* const stageFilePath = "resource/StageData.csv";
	const char* const cubeModelPath = "resource/model/Cube.mv1";

	const char* const blueTexturePath = "resource/model/texture/Blue.png";
	const char* const pinkTexturePath = "resource/model/texture/Pink.png";

	const VECTOR gravity = { 0.0f, -0.1f, 0.0f };

	enum
	{
		stageWidthSize
	};
}

Stage::Stage() :
	stageSize(0), stageMaxSize(0),
	modelSize({ 0.0f, 0.0f, 0.0f }), 
	initPos({ 0.0f, 0.0f, 0.0f })
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	LoadStage();
	LoadModelHandle();
	InitializeStage();
}

void Stage::Update()
{
	for (int i = 0; i < stageMaxSize; i++)
	{
		if (model[i].isFall)
		{
			model[i].pos = VAdd(model[i].pos, gravity);
			MV1SetPosition(model[i].handle, model[i].pos);
		}

		if (model[i].pos.y < - 10.0f)
		{
			model[i].isFall = false;
			model[i].pos.y = 0.0f;
			MV1SetPosition(model[i].handle, model[i].pos);
		}
	}
}

void Stage::Draw()
{
	for (int i = 0; i < stageMaxSize; i++)
	{
		MV1DrawModel(model[i].handle);
	}

	//XYZ軸描画
	float lineSize = 300.0f;
	DrawLine3D(VGet(-lineSize, 0, 0), VGet(lineSize, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(0, -lineSize, 0), VGet(0, lineSize, 0), GetColor(0, 255, 0));
	DrawLine3D(VGet(0, 0, -lineSize), VGet(0, 0, lineSize), GetColor(0, 0, 255));
}

void Stage::Finalize()
{
	for (int i = 0; i < stageMaxSize; i++)
	{
		MV1DeleteModel(model[i].handle);
	}
}

void Stage::SelectFallCube(const PlayerData playerData)
{
	const float rotateY = playerData.rotate.y;
	if (rotateY < 0.0f) return;

	const int currentWidthNum = static_cast<int>(playerData.pos.x + stageSize / 2);
	const int currentHeightNum = static_cast<int>(playerData.pos.z + stageSize / 2);

	const int currentIndex = currentHeightNum * stageSize + currentWidthNum;

	//ステージの端からステージ外方向には処理しない
	if (currentHeightNum == 0 && rotateY == downVec) return;
	if (currentWidthNum == 0 && rotateY == leftVec) return;
	if (currentHeightNum == stageSize - 1 && rotateY == upVec) return;
	if (currentWidthNum == stageSize - 1 && rotateY == rightVec) return;

	int beginIndex = currentIndex;
	int endIndex = currentIndex;
	
	if (rotateY == leftVec)
	{
		beginIndex -= 1;
		endIndex = currentHeightNum * stageSize;
	}
	if (rotateY == rightVec)
	{
		beginIndex += 1;
		endIndex = currentHeightNum * stageSize + (stageSize - 1);
	}

	if (rotateY == upVec)
	{
		beginIndex += stageSize;
		endIndex = stageMaxSize - (stageSize - currentWidthNum);
	}
	if (rotateY == downVec)
	{
		beginIndex -= stageSize;
		endIndex = currentWidthNum;
	}

	SelectCubeLine(beginIndex, endIndex, currentIndex);
}

const RECT Stage::GetStageSize()
{
	RECT stageMaxRange = { 0, 0, 0, 0 };
	
	stageMaxRange.left = static_cast<long>(initPos.x - modelSize.x / 2);
	stageMaxRange.top = static_cast<long>(model[stageMaxSize - 1].pos.z + modelSize.z / 2);
	stageMaxRange.right = static_cast<long>(model[stageMaxSize - 1].pos.x + modelSize.x / 2);
	stageMaxRange.bottom = static_cast<long>(initPos.z - modelSize.z / 2);

	return stageMaxRange;
}

void Stage::LoadStage()
{
	//ステージデータの読み込み
	ifstream ifs(stageFilePath);
	string line;
	vector<int> stage;

	while (getline(ifs, line))
	{
		if (line == "サイズ") continue;
		vector<string> strvec = split(line, ',');
		stage.push_back(stoi(strvec[stageWidthSize]));
	}

	stageSize = stage[stageWidthSize];
	stageMaxSize = stageSize * stageSize;
}

void Stage::LoadModelHandle()
{
	model.resize(stageMaxSize);

	//モデルの読み込み
	const int cubeHandle = MV1LoadModel(cubeModelPath);

	const int colorBlue = LoadGraph(blueTexturePath);
	const int colorPink = LoadGraph(pinkTexturePath);
	for (int i = 0; i < stageMaxSize; i++)
	{
		//2色のモデルを交互に配置する
		//偶数時しかできていないため改善余地あり
		const int duplicateModel = MV1DuplicateModel(cubeHandle);

		if (i / stageSize % 2 == i % 2) MV1SetTextureGraphHandle(duplicateModel, 0, colorBlue, false);
		else MV1SetTextureGraphHandle(duplicateModel, 0, colorPink, false);

		model[i].handle = duplicateModel;
		model[i].isFall = false;
	}

	modelSize = { 1.0f, 1.0f , 1.0f };
}

void Stage::InitializeStage()
{
	//ステージの中央と画面中央が同じになるよう調整
	float size = static_cast<float>(stageSize / 2);
	if (stageSize % 2 == 0)
	{
		size -= modelSize.x / 2.0f;
	}

	size *= -modelSize.x;
	initPos = { size, 0.0f, size };

	//座標をセット
	VECTOR pos = initPos;
	VECTOR widthSpace = { modelSize.x, 0.0f, 0.0f };
	VECTOR depthSpace = { 0.0f, 0.0f, modelSize.z };

	//モデルを配置する
	for (int i = 0; i < stageMaxSize; i++)
	{
		const int widthNum = i % stageSize;
		if (widthNum == 0 && i != 0)
		{
			pos = initPos;
			const float depthNum = static_cast<float>(i / stageSize);
			pos = VAdd(pos, VScale(depthSpace, depthNum));
		}
		model[i].pos = pos;
		MV1SetPosition(model[i].handle, model[i].pos);
		pos = VAdd(pos, widthSpace);
	}
}

void Stage::SelectCubeLine(const int beginIndex, const int endIndex, const int currentIndex)
{
	//上方向、右方向
	if (beginIndex < endIndex)
	{
		for (int i = beginIndex; i <= endIndex; i++)
		{
			const bool widthLineCheck = i / stageSize == currentIndex / stageSize;
			const bool heightLineCheck = i % stageSize == currentIndex % stageSize;
			if (widthLineCheck || heightLineCheck) model[i].isFall = true;
		}
	}
	//下方向、左方向
	else
	{
		for (int i = beginIndex; endIndex <= i; i--)
		{
			const bool widthLineCheck = i / stageSize == currentIndex / stageSize;
			const bool heightLineCheck = i % stageSize == currentIndex % stageSize;
			if (widthLineCheck || heightLineCheck) model[i].isFall = true;
		}
	}
}

vector<string> Stage::split(const string& input, const char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter))
	{
		result.push_back(field);
	}
	return result;
}
