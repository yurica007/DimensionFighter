#include "Stage.h"
#include <sstream>
#include <fstream>

namespace
{
	const char* const stageFilePath = "data/StageData.csv";
	const char* const textureDataFilePath = "data/TextureData.csv";
	const char* const cubeModelPath = "data/model/Cube.mv1";

	const char* const textureFilePath = "data/model/texture/stage/";
	const char* const dataExtension = ".png";

	const char* const lightTexturePath = "data/model/texture/StageLight.png";
	const char* const darkTexturePath = "data/model/texture/StageDark.png";

	enum
	{
		stageWidthSize
	};
}

Stage::Stage() :
	stageSize(0), stageMaxSize(0),
	modelSize({ 0.0f, 0.0f, 0.0f }),
	fallPlayer(-1),
	darkTexture(0)
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
	LoadTexture();
}

void Stage::Update()
{
	for (int i = 0; i < stageMaxSize; i++)
	{
		if (cubeModel[i].isFall)
		{
			int index = fallPlayer;
			if (modelInitTexture[i] == darkTexture) index = fallPlayer;
			else index = fallPlayer + playerNum;

			if (!cubeModel[i].isFalling) MV1SetTextureGraphHandle(cubeModel[i].handle, 0, modelTranceTexture[index], false);
			cubeModel[i].isFall = false;
			cubeModel[i].isFalling = true;
		}

		if (cubeModel[i].isFalling)
		{
			cubeModel[i].pos = VAdd(cubeModel[i].pos, gravity);
			MV1SetPosition(cubeModel[i].handle, cubeModel[i].pos);
		}

		if (cubeModel[i].pos.y < deadLine)
		{
			cubeModel[i].isFalling = false;
			cubeModel[i].pos.y = 0.0f;
			MV1SetPosition(cubeModel[i].handle, cubeModel[i].pos);
			MV1SetTextureGraphHandle(cubeModel[i].handle, 0, modelInitTexture[i], false);
		}
	}
}

void Stage::Draw()
{
	for (int i = 0; i < stageMaxSize; i++)
	{
		MV1DrawModel(cubeModel[i].handle);
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
		MV1DeleteModel(cubeModel[i].handle);
	}
}

void Stage::SelectFallCube(const vector<VECTOR> direction, const vector<vector<int>> index)
{
	for (int i = 0; i < direction.size(); i++)
	{
		const float rotateY = direction[i].y;
		if (rotateY < 0.0f) continue;		

		const int currentWidthNum = index[i][0];
		const int currentHeightNum = index[i][1];

		const int currentIndex = currentHeightNum * stageSize + currentWidthNum;

		//ステージの端からステージ外方向には処理しない
		if (currentHeightNum == 0 && rotateY == downVec) continue;
		if (currentWidthNum == 0 && rotateY == leftVec) continue;
		if (currentHeightNum == stageSize - 1 && rotateY == upVec) continue;
		if (currentWidthNum == stageSize - 1 && rotateY == rightVec) continue;

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
		fallPlayer = i;
	}	
}

const RECT Stage::GetStageSize()
{
	RECT stageMaxRange = { 0, 0, 0, 0 };
	
	stageMaxRange.left = static_cast<long>(cubeModel[0].pos.x - modelSize.x / 2);
	stageMaxRange.top = static_cast<long>(cubeModel[stageMaxSize - 1].pos.z + modelSize.z / 2);
	stageMaxRange.right = static_cast<long>(cubeModel[stageMaxSize - 1].pos.x + modelSize.x / 2);
	stageMaxRange.bottom = static_cast<long>(cubeModel[0].pos.z - modelSize.z / 2);

	return stageMaxRange;
}

const vector<vector<int>> Stage::GetStageFallData()
{
	vector<vector<int>> fallPos;
	fallPos.resize(stageMaxSize);

	for (int i = 0; i < stageMaxSize; i++)
	{
		if (cubeModel[i].isFalling)
		{
			const int posWidth = cubeModel[i].pos.x - modelSize.x / 2 + stageSize / 2;
			const int posHeight = cubeModel[i].pos.z - modelSize.z / 2 + stageSize / 2;

			fallPos[i].push_back(posWidth);
			fallPos[i].push_back(posHeight);
			continue;
		}

		fallPos[i].push_back(-1);
		fallPos[i].push_back(-1);		
	}

	return fallPos;
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
	cubeModel.resize(stageMaxSize);

	//モデルの読み込み
	const int cubeHandle = MV1LoadModel(cubeModelPath);

	const int colorLight = LoadGraph(lightTexturePath);
	const int colorDark = LoadGraph(darkTexturePath);
	darkTexture = colorDark;

	for (int i = 0; i < stageMaxSize; i++)
	{
		//2色のモデルを交互に配置する
		const int duplicateModel = MV1DuplicateModel(cubeHandle);
		const bool isColorLight = (i / stageSize % 2 == i % stageSize % 2);
		int texture = 0;

		if (isColorLight) texture = colorLight;
		else texture = colorDark;

		MV1SetTextureGraphHandle(duplicateModel, 0, texture, false);
		modelInitTexture.push_back(texture);

		cubeModel[i].handle = duplicateModel;
		cubeModel[i].isFall = false;
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
	const VECTOR initPos = { size, 0.0f, size };

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
		cubeModel[i].pos = pos;
		MV1SetPosition(cubeModel[i].handle, cubeModel[i].pos);
		pos = VAdd(pos, widthSpace);
	}
}

void Stage::LoadTexture()
{
	ifstream ifs(textureDataFilePath);
	string line;
	vector<string> textureName;

	while (getline(ifs, line))
	{
		vector<string> strvec = split(line, ',');
		textureName.push_back(strvec[0]);
	}

	for (auto& texName : textureName)
	{
		const string texturePath = textureFilePath + texName + dataExtension;
		const int texture = LoadGraph(texturePath.c_str());
		modelTranceTexture.push_back(texture);
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
			const bool isnotFall = !cubeModel[i].isFall && !cubeModel[i].isFalling;

			if (widthLineCheck || heightLineCheck && isnotFall)
			{
				cubeModel[i].isFall = true;
			}
		}
	}
	//下方向、左方向
	else
	{
		for (int i = beginIndex; endIndex <= i; i--)
		{
			const bool widthLineCheck = i / stageSize == currentIndex / stageSize;
			const bool heightLineCheck = i % stageSize == currentIndex % stageSize;
			const bool isnotFall = !cubeModel[i].isFall && !cubeModel[i].isFalling;

			if (widthLineCheck || heightLineCheck && isnotFall)
			{
				cubeModel[i].isFall = true;
			}
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
