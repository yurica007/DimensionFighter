#include "UIUpdateTime.h"
#include "../tool/Load.h"
#include "../ReuseData.h"
#include "../GameData.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	const char* const countFilePath = "data/CountData.csv";
	const char* const uILapseTimeFilePath = "data/UILapseTime.csv";

	//ゲームシーン時UIのX座標
	constexpr int uIGameFramePosX = 80;
	constexpr int uIGameClockPosX = 25;
	constexpr int uiGameTimePosX = 60;

	//リザルトシーン時UIのX座標
	constexpr int uIResultFramePosX = 640;
	constexpr int uIResultClockPosX = 580;
	constexpr int uiResultTimePosX = 620;

	//ゲームシーン時UIのY座標
	constexpr int uIGamePosY = 690;

	//リザルトシーン時UIのY座標
	constexpr int uIResultPosY = 600;

	constexpr int numSpace = 13;
	constexpr int signSpace = 10;

	constexpr int timeAllRank = 8;
	constexpr int upperNum = 10;

	struct TimeData
	{
		int handle = -1;
		int num = 0;
	};

	enum UIType
	{
		frame,
		clock
	};
}

UIUpdateTime::UIUpdateTime(ReuseData& reuse, const SceneType scene) :
	reuseData(reuse), sceneType(scene), count(4)
{
}

void UIUpdateTime::Initialize()
{
	if (sceneType == SceneType::game)
	{
		//カウントダウン素材の読込み
		Load* load = new Load();
		vector<int> handle;
		load->LoadGraphFile(countFilePath, handle);
		delete load;

		for (int i = 0; i < handle.size(); i++)
		{
			countHandle.push_back({ halfScreenWidth, halfScreenHeight, 1.0, 0.0, handle[i] });
		}
	}

	{
		Load* load = new Load();
		vector<int> handle;
		load->LoadGraphFile(uILapseTimeFilePath, handle);
		delete load;

		for (int i = 0; i < handle.size(); i++)
		{
			int posX = 0, posY = 0;
			//シーンに応じて描画する座標を決定する
			if (sceneType == SceneType::game)
			{
				if (i == UIType::frame) posX = uIGameFramePosX, posY = uIGamePosY;
				if (i == UIType::clock) posX = uIGameClockPosX, posY = uIGamePosY;
			}
			if (sceneType == SceneType::result)
			{
				if (i == UIType::frame) posX = uIResultFramePosX, posY = uIResultPosY;
				if (i == UIType::clock) posX = uIResultClockPosX, posY = uIResultPosY;
			}

			//座標、画像をセットする
			uIHandleData.push_back({ posX, posY, 1.0, 0.0, handle[i] });
		}
	}

	int posX = 0, posY = 0;
	//シーンに応じて描画する最初の座標を決定する
	if (sceneType == SceneType::game) posX = uiGameTimePosX, posY = uIGamePosY;
	if (sceneType == SceneType::result) posX = uiResultTimePosX, posY = uIResultPosY;

	int scoreRank = 0;
	for (int i = 0; i < timeAllRank; i++)
	{
		scoreRank++;
		//座標をセットする
		timeHandleData.push_back({ posX, posY });

		//数字とその他で幅を変更する
		if (scoreRank % 3 == 1) posX += numSpace;
		else posX += signSpace;
	}
	reuseData.RewriteTimeData(timeHandle);
}

void UIUpdateTime::Update(const Time currentTime, const int countDown)
{
	vector<TimeData> time;
	time.resize(timeAllRank);

	time[0].num = currentTime.minutes / upperNum;
	time[1].num = currentTime.minutes % upperNum;
	time[2].num = upperNum;
	time[3].num = currentTime.seconds / upperNum;
	time[4].num = currentTime.seconds % upperNum;
	time[5].num = upperNum + 1;
	time[6].num = currentTime.miliSeconds / upperNum;
	time[7].num = currentTime.miliSeconds % upperNum;

	for (int i = 0; i < timeHandle.size(); i++)
	{
		for (int j = 0; j < time.size(); j++)
		{
			if (time[j].num != i) continue;
			time[j].handle = timeHandle[i];
		}
	}

	//経過時間と描画する時間をそろえる
	for (int i = 0; i < timeHandleData.size(); i++)
	{
		timeHandleData[i].handle = time[i].handle;
	}

	count = countDown;
}

void UIUpdateTime::Draw()
{
	//画像の描画
	for (int i = 0; i < uIHandleData.size(); i++)
	{
		const HandleData uI = uIHandleData[i];

		DrawRotaGraph(uI.posX, uI.posY, uI.extRate, uI.angle, uI.handle, true);
	}

	//経過時間の描画
	for (int i = 0; i < timeHandleData.size(); i++)
	{
		const HandleData time = timeHandleData[i];

		DrawRotaGraph(time.posX, time.posY, time.extRate, time.angle, time.handle, true);
	}

	if (sceneType != SceneType::game) return;
	for (int i = 0; i < countHandle.size(); i++)
	{
		if (count != i) continue;

		const HandleData data = countHandle[i];

		DrawRotaGraph(data.posX, data.posY, data.extRate, data.angle, data.handle, true);
	}
}

void UIUpdateTime::Finalize()
{
}
