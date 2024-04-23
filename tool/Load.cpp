#include "Load.h"
#include <DxLib.h>
#include <sstream>
#include <fstream>

namespace
{
    using namespace std;

    const char* const binFile = "data/record/highScore.bin";

    constexpr int timeRank = 6;
    constexpr int upperNum = 10;

    constexpr int mojiCodeZero = '0';
}

Load::Load() :
    loadLine(0)
{
}

void Load::SaveScore(const Time time)
{
    //保存されている時間を読み込む
    const Time highScore = LoadHighScore();

    //保存されていない場合…現在の記録を保存
    //保存されていた場合…クリア時間と比較しクリア時間が短ければ保存
    if (!IsNewRecord(highScore, time)) return;

    //クリア時間をchar型の6桁の配列に整える
    vector<char> saveScore;
    saveScore.push_back(time.minutes / upperNum);
    saveScore.push_back(time.minutes % upperNum);
    saveScore.push_back(time.seconds / upperNum);
    saveScore.push_back(time.seconds % upperNum);
    saveScore.push_back(time.miliSeconds / upperNum);
    saveScore.push_back(time.miliSeconds % upperNum);

    //バイナリファイルに文字コード'0'との和を保存
    fstream fs;
    fs.open(binFile, ios_base::out | ios_base::trunc | ios_base::binary);
    for (int i = 0; i < saveScore.size(); i++)
    {
        saveScore[i] += mojiCodeZero;
        fs.write(&saveScore[i], sizeof(char));
    }

    fs.close();
}

const Time Load::LoadHighScore()
{
    fstream fs;

    //ファイルの読み込み
    char loadScore[timeRank];
    fs.open(binFile);
    fs.read(loadScore, sizeof(char) * timeRank);
    fs.close();

    //読み込んだchara型の配列をint型の配列に整える
    vector<int> score;
    for (int i = 0; i < timeRank; i++)
    {
        score.push_back(loadScore[i] - mojiCodeZero);
    }

    //6桁の配列を使いやすい形に直す
    Time highScore;
    highScore.minutes = score[0] * upperNum + score[1];
    highScore.seconds = score[2] * upperNum + score[3];
    highScore.miliSeconds = score[4] * upperNum + score[5];

    return highScore;
}

const string Load::LoadCSV(const string& inputStr, const char delimiter)
{
    loadLine++;

	istringstream stream(inputStr);
	string field;
	vector<string> data;
	while (getline(stream, field, delimiter))
	{
		data.push_back(field);
	}

    string result = "";
    for (int i = 0; i < data.size(); i++)
    {
        result += data[i];
    }

	return result;
}

const vector<string> Load::LoadCSVData(const string& inputStr, const char delimiter)
{
    loadLine++;

    istringstream stream(inputStr);
    string field;
    vector<string> data;
    while (getline(stream, field, delimiter))
    {
        data.push_back(field);
    }

    vector<string> result = data;
    return result;
}

void Load::LoadGraphFile(const string filePath, vector<int>& graphHandle)
{
    ifstream ifs(filePath);
    string line;

    while (getline(ifs, line))
    {
        const string strvec = LoadCSV(line, ',');
        const int loadLine = GetLoadLine();

        //説明文は省く
        if (loadLine == commentLine) continue;

        graphHandle.push_back(LoadGraph(strvec.c_str()));
    }
}

void Load::LoadGraphAndTexGraphtFile(const std::string filePath, vector<int>& graphHandle, vector<int>& textHandle)
{
    ifstream ifs(filePath);
    string line;

    while (getline(ifs, line))
    {
        const string strvec = LoadCSV(line, ',');
        const int loadLine = GetLoadLine();

        //説明文は省く
        if (loadLine == commentLine) continue;

        if (loadLine <= 3 + commentLine) graphHandle.push_back(LoadGraph(strvec.c_str()));
        else textHandle.push_back(LoadGraph(strvec.c_str()));
    }
}

const bool Load::IsNewRecord(const Time targetTime, const Time sorceTime)
{
    if (0 <= targetTime.minutes && 0 <= targetTime.seconds && 0 <= targetTime.miliSeconds)
    {
        if (targetTime.minutes < sorceTime.minutes) return false;
        if (targetTime.minutes <= sorceTime.minutes && targetTime.seconds < sorceTime.seconds) return false;
        if (targetTime.minutes <= sorceTime.minutes && targetTime.seconds <= sorceTime.seconds && targetTime.miliSeconds < sorceTime.miliSeconds) return false;
    }

    return true;
}
