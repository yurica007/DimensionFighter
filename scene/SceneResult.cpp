#include "SceneResult.h"
#include "SceneManager.h"
#include "SceneMenuEndGame.h"
#include "../BackGround.h"
#include "../character/CharaData.h"
#include "../tool/InputState.h"
#include "../tool/Camera.h"
#include "../ui/UIUpdateTime.h"
#include "../ui/UIClearTime.h"
#include "../tool/Load.h"
#include "../GameData.h"
#include "../ReuseData.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	const char* const uIResultFilePath = "data/UIResult.csv";

	constexpr VECTOR charaPos = { -6.0f, 2.2f, -2.0f };
	constexpr VECTOR cushionPos = { -6.0f, 0.7f, -1.5f };
	constexpr VECTOR scale = { 3.0f, 3.0f, 3.0f };

	//Xç¿ïW
	constexpr int titlePosX = halfScreenWidth;
	constexpr int rankPosX = 160;
	constexpr float posDistX = 4.0f;

	//Yç¿ïW
	constexpr int titlePosY = 100;
	constexpr int rankPosY = 250;

	//ëOÇ∆ÇÃãóó£
	constexpr int distX = 320;
	constexpr float distY = 0.2f;

	constexpr int gaussPixelSize = 16;
	constexpr int gaussParamMax = 1400;
}

SceneResult::SceneResult(SceneManager& manager, Camera& camera, ReuseData& reuse) :
	SceneBase(manager, camera, reuse),
	backGround(new BackGround()),
	updateTime(new UIUpdateTime(reuse, SceneType::result)),
	clearTime(new UIClearTime(reuse, SceneType::result)),
	stage(-1), screenGraph(-1), time(0), isFade(false)
{
}

void SceneResult::Initialize()
{
	backGround->Initialize(SceneType::result);

	updateTime->Initialize();
	clearTime->Initialize();

	Time time;
	reuseData.RewriteTimeData(time);
	reuseData.SetHighscore(time);

	vector<int> colorNum;
	reuseData.RewriteColorData(colorNum);

	{
		VECTOR pos = charaPos;
		for (int i = 0; i < colorNum.size(); i++)
		{
			pos.y = charaPos.y;
			pos.y -= (ranking[i] - 1) * distY;
			
			vector<string> data;
			reuseData.RewriteCharaData(data, i);

			charaModelHandle.push_back(MV1LoadModel(data[0].c_str()));
			MV1SetTextureGraphHandle(charaModelHandle[i], 0, LoadGraph(data[1].c_str()), false);

			MV1SetPosition(charaModelHandle[i], pos);
			pos.x += posDistX;
		}
	}

	{
		const int handle = MV1LoadModel("data/model/cushion.mv1");

		VECTOR pos = cushionPos;
		for (int i = 0; i < ranking.size(); i++)
		{
			for (int j = ranking[i]; j < colorNum.size(); j++)
			{
				const int copyModel = MV1DuplicateModel(handle);

				cushionModelHandle.push_back(copyModel);
				MV1SetScale(copyModel, scale);
				MV1SetPosition(cushionModelHandle.back(), pos);
				pos.y += distY;
			}

			pos.y = cushionPos.y;
			pos.x += posDistX;
		}
	}

	//èáà âÊëúÇÃì«çûÇ›
	{
		ifstream ifs(uIResultFilePath);
		vector<int> resultHandle;
		LoadFile(ifs, resultHandle);

		int posX = titlePosX, posY = titlePosY;
		
		resultHandleData.push_back({ posX, posY, 1.0, 0.0, resultHandle.front()});
				
		posX = rankPosX, posY = rankPosY;
		for (int i = 0; i < ranking.size(); i++)
		{
			int handle = resultHandle[ranking[i]];
			if (ranking[i] == 0) handle = -1;

			resultHandleData.push_back({ posX, posY, 1.0, 0.0, handle });

			posX += distX;
		}
	}

	stage = MV1LoadModel("data/model/stage.mv1");
	MV1SetPosition(stage, { 0.0f, 0.0f, 0.0f });

	screenGraph = MakeScreen(screenWidth, screenHeight, true);
}

void SceneResult::Update(const InputState& inputState)
{
	backGround->Update();
	const bool isPushScene = sceneState == SceneState::pushScne;
	if (!isPushScene) UpdateFadein(isFade);

	if (isFade) return;

	if (isPushScene)
	{
		SceneMenuEndGame* menuScene = new SceneMenuEndGame(manager_, camera, reuseData);
		manager_.PushScene(menuScene);
		menuScene->Initialize();
		return;
	}

	if (inputState.IsTriggered(InputType::select) && 60 <= time)
	{
		sceneState = SceneState::pushScne;
	}
	else sceneState = SceneState::donot;

	camera.SetPosAndVec(SceneType::result);
	
	if (time < 60) time++;

	Time time;
	reuseData.RewriteTimeData(time);
	updateTime->Update(time, 0);
	clearTime->Update();
}

void SceneResult::Draw()
{
	SetDrawScreen(screenGraph);
	ClearDrawScreen();

	camera.SetPosAndVec(SceneType::result);

	MV1DrawModel(stage);

	for (int i = 0; i < resultHandleData.size(); i++)
	{
		const HandleData result = resultHandleData[i];

		DrawRotaGraph(result.posX, result.posY, result.extRate, result.angle, result.handle, true);
	}

	for (int i = 0; i < cushionModelHandle.size(); i++)
	{
		MV1DrawModel(cushionModelHandle[i]);
	}

	for (int i = 0; i < charaModelHandle.size(); i++)
	{
		MV1DrawModel(charaModelHandle[i]);
	}

	updateTime->Draw();
	clearTime->Draw();
	if (sceneState == SceneState::pushScne) GraphFilter(screenGraph, DX_GRAPH_FILTER_GAUSS, gaussPixelSize, gaussParamMax);
	SetDrawScreen(DX_SCREEN_BACK);

	backGround->Draw();
	DrawRotaGraph(halfScreenWidth, halfScreenHeight, 1.0, 0.0, screenGraph, true);
	DrawFadeGraph();
}

void SceneResult::Finalize()
{
	backGround->Finalize();
	delete backGround;

	updateTime->Finalize();
	delete updateTime;
	
	clearTime->Finalize();
	delete clearTime;
}

void SceneResult::LoadFile(std::ifstream& ifs, std::vector<int>& handle)
{
	std::string line;

	Load* load = new Load();
	while (getline(ifs, line))
	{
		const std::string strvec = load->LoadCSV(line, ',');
		const int loadLine = load->GetLoadLine();

		//ê‡ñæï∂ÇÕè»Ç≠
		if (loadLine == commentLine) continue;

		handle.push_back(LoadGraph(strvec.c_str()));
	}
	delete load;
}