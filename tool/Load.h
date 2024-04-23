#pragma once
#include <string>
#include <vector>
#include "../TimeData.h"

namespace
{
	constexpr int commentLine = 1;
}

class Load
{
public:
	Load();

	/// <summary>
	/// バイナルファイルに書込む
	/// </summary>
	/// <param name="time">保存する時間データ</param>
	void SaveScore(const Time time);

	/// <summary>
	/// バイナリファイルを読込む
	/// </summary>
	/// <returns>読込んだ時間データ</returns>
	const Time LoadHighScore();

	/// <summary>
	/// CSVファイルを読込む
	/// </summary>
	/// <param name="inputStr">ファイル名</param>
	/// <param name="delimiter">読込む範囲</param>
	/// <returns>読込んだデータ</returns>
	const std::string LoadCSV(const std::string& inputStr, const char delimiter);

	const std::vector<std::string> LoadCSVData(const std::string& inputStr, const char delimiter);

	/// <summary>
	/// 読込んだの列数を取得する
	/// </summary>
	/// <returns>読込んだの列数</returns>
	const int GetLoadLine() { return loadLine; }

	void LoadGraphFile(const std::string filePath, std::vector<int>& graphHandle);

	void LoadGraphAndTexGraphtFile(const std::string filePath, std::vector<int>& graphHandle, std::vector<int>& textHandle);

private:
	const bool IsNewRecord(const Time targetTime, const Time sorceTime);

	int loadLine;
};