#include "UIClearTime.h"
#include "../tool/Load.h"
#include "../ReuseData.h"
#include "../Easing.h"
#include <DxLib.h>

namespace
{
	using namespace std;

	const char* const uIClearTimeFilePath = "data/UIClearTime.csv";

	//ゲームシーン時UIのX座標
	constexpr int uIGameFramePosX = 100;
	constexpr int uIGameTextPosX = 60;
	constexpr int uiGameTimePosX = 130;

	//リザルトシーン時UIのX座標
	constexpr int uIResultFramePosX = 640;
	constexpr int uIResultltLeftStarPosX = 490;
	constexpr int uIResultRightStarPosX = 790;
	constexpr int uIResultUpdatePosX = 530;
	constexpr int uIResultTextPosX = 590;
	constexpr int uiResultTimePosX = 660;

	//ゲームシーン時UIのY座標
	constexpr int uIGamePosY = 650;

	//リザルトシーン時UIのY座標
	constexpr int uIResultPosY = 650;
	constexpr int uIResultUpdatePosY = 635;

	constexpr double radian = DX_PI_F / 180.0f;
	constexpr double initAngle = -20.0;
	constexpr double turnMaxRate = 10.0;

	constexpr int numSpace = 13;
	constexpr int signSpace = 10;

	constexpr int timeAllRank = 8;
	constexpr int upperNum = 10;

	constexpr int deleteHiscoreTimeInGame = 300;

	struct TimeData
	{
		int handle = -1;
		int num = 0;
	};

	enum UIType
	{
		frame,
		update,
		text
	};
}

UIClearTime::UIClearTime(ReuseData& reuse, const SceneType scene) :
	reuseData(reuse), sceneType(scene),
	time(0.0f), angle(0.0f),
	isNewRecord(false), isDraw(true)
{
}

void UIClearTime::Initialize()
{
	reuseData.RewriteTimeData(timeHandle);
	{
		vector<TimeData> highScoreData;

		if (sceneType == SceneType::game)
		{
			Time highScore;
			reuseData.RewriteScoreData(highScore);

			if (highScore.minutes == highScore.seconds &&
				highScore.seconds == highScore.miliSeconds &&
				highScore.miliSeconds < 0) isDraw = false;

			highScoreData.resize(timeAllRank);

			highScoreData[0].num = highScore.minutes / upperNum;
			highScoreData[1].num = highScore.minutes % upperNum;
			highScoreData[2].num = upperNum;
			highScoreData[3].num = highScore.seconds / upperNum;
			highScoreData[4].num = highScore.seconds % upperNum;
			highScoreData[5].num = upperNum + 1;
			highScoreData[6].num = highScore.miliSeconds / upperNum;
			highScoreData[7].num = highScore.miliSeconds % upperNum;

			for (int i = 0; i < timeHandle.size(); i++)
			{
				for (int j = 0; j < highScoreData.size(); j++)
				{
					if (highScoreData[j].num != i) continue;
					highScoreData[j].handle = timeHandle[i];
				}
			}
		}
		if (sceneType == SceneType::result)
		{
			Time highScore;
			reuseData.RewriteScoreData(highScore);
			Time time;
			reuseData.RewriteTimeData(time);

			if (time.minutes < highScore.minutes) isNewRecord = true;
			else if (time.minutes == highScore.minutes)
			{
				if (time.seconds < highScore.seconds) isNewRecord = true;
				if (time.seconds == highScore.seconds && time.miliSeconds < highScore.miliSeconds) isNewRecord = true;
			}
			else if (highScore.minutes < 0) isNewRecord = true;

			if (isNewRecord)
			{
				highScoreData.resize(timeAllRank);

				highScoreData[0].num = time.minutes / upperNum;
				highScoreData[1].num = time.minutes % upperNum;
				highScoreData[2].num = upperNum;
				highScoreData[3].num = time.seconds / upperNum;
				highScoreData[4].num = time.seconds % upperNum;
				highScoreData[5].num = upperNum + 1;
				highScoreData[6].num = time.miliSeconds / upperNum;
				highScoreData[7].num = time.miliSeconds % upperNum;

				for (int i = 0; i < timeHandle.size(); i++)
				{
					for (int j = 0; j < highScoreData.size(); j++)
					{
						if (highScoreData[j].num != i) continue;
						highScoreData[j].handle = timeHandle[i];
					}
				}
			}
			else
			{
				highScoreData.resize(timeAllRank);

				highScoreData[0].num = highScore.minutes / upperNum;
				highScoreData[1].num = highScore.minutes % upperNum;
				highScoreData[2].num = upperNum;
				highScoreData[3].num = highScore.seconds / upperNum;
				highScoreData[4].num = highScore.seconds % upperNum;
				highScoreData[5].num = upperNum + 1;
				highScoreData[6].num = highScore.miliSeconds / upperNum;
				highScoreData[7].num = highScore.miliSeconds % upperNum;

				for (int i = 0; i < timeHandle.size(); i++)
				{
					for (int j = 0; j < highScoreData.size(); j++)
					{
						if (highScoreData[j].num != i) continue;
						highScoreData[j].handle = timeHandle[i];
					}
				}
			}
		}

		int posX = 0, posY = 0;
		if (sceneType == SceneType::game) posX = uiGameTimePosX, posY = uIGamePosY;
		if (sceneType == SceneType::result) posX = uiResultTimePosX, posY = uIResultPosY;

		int scoreRank = 0;
		for (int i = 0; i < highScoreData.size(); i++)
		{
			scoreRank++;
			scoreHandleData.push_back({ posX, posY, 1.0, 0.0, highScoreData[i].handle });

			if (scoreRank % 3 == 1) posX += numSpace;
			else posX += signSpace;
		}
	}

	if (true)
	{
		Load* load = new Load();
		vector<int> handle;
		load->LoadGraphFile(uIClearTimeFilePath, handle);
		delete load;

		for (int i = 0; i < handle.size(); i++)
		{
			int posX = 0, posY = 0;

			if (sceneType == SceneType::game)
			{
				if (i == UIType::frame) posX = uIGameFramePosX, posY = uIGamePosY;
				if (i == UIType::text) posX = uIGameTextPosX, posY = uIGamePosY;
			}
			if (sceneType == SceneType::result)
			{
				if (i == UIType::frame) posX = uIResultFramePosX, posY = uIResultPosY;
				if (i == UIType::update && isNewRecord) posX = uIResultUpdatePosX, posY = uIResultUpdatePosY;
				if (i == UIType::text) posX = uIResultTextPosX, posY = uIResultPosY;
			}

			uIHandleData.push_back({ posX, posY, 1.0, angle, handle[i] });
		}
	}
}

void UIClearTime::Update()
{
	if (sceneType == SceneType::game)
	{
		if (deleteHiscoreTimeInGame < time++) isDraw = false;
	}

	if (sceneType != SceneType::result || !isNewRecord) return;

	if (80.0f < time++) time = 0.0f;

	Easing* easing = new Easing();
	const float sineRate = easing->InSine(time, 10.0f, 10.0f, 0.0f);
	angle = easing->ToRadianF(sineRate + 10.0f);
	delete easing;
}

void UIClearTime::Draw()
{
	if (!isDraw) return;

	//画像の描画
	for (int i = 0; i < uIHandleData.size(); i++)
	{
		HandleData uI = uIHandleData[i];
		if (uI.posX == 0 && uI.posY == 0) continue;

		if (i == UIType::update) uI.angle = static_cast<double>(angle);

		DrawRotaGraph(uI.posX, uI.posY, uI.extRate, uI.angle, uI.handle, true);
	}

	//クリア時間の描画
	for (int i = 0; i < scoreHandleData.size(); i++)
	{
		const HandleData score = scoreHandleData[i];

		DrawRotaGraph(score.posX, score.posY, score.extRate, score.angle, score.handle, true);
	}
}

void UIClearTime::Finalize()
{
}
