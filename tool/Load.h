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
	/// �o�C�i���t�@�C���ɏ�����
	/// </summary>
	/// <param name="time">�ۑ����鎞�ԃf�[�^</param>
	void SaveScore(const Time time);

	/// <summary>
	/// �o�C�i���t�@�C����Ǎ���
	/// </summary>
	/// <returns>�Ǎ��񂾎��ԃf�[�^</returns>
	const Time LoadHighScore();

	/// <summary>
	/// CSV�t�@�C����Ǎ���
	/// </summary>
	/// <param name="inputStr">�t�@�C����</param>
	/// <param name="delimiter">�Ǎ��ޔ͈�</param>
	/// <returns>�Ǎ��񂾃f�[�^</returns>
	const std::string LoadCSV(const std::string& inputStr, const char delimiter);

	const std::vector<std::string> LoadCSVData(const std::string& inputStr, const char delimiter);

	/// <summary>
	/// �Ǎ��񂾂̗񐔂��擾����
	/// </summary>
	/// <returns>�Ǎ��񂾂̗�</returns>
	const int GetLoadLine() { return loadLine; }

	void LoadGraphFile(const std::string filePath, std::vector<int>& graphHandle);

	void LoadGraphAndTexGraphtFile(const std::string filePath, std::vector<int>& graphHandle, std::vector<int>& textHandle);

private:
	const bool IsNewRecord(const Time targetTime, const Time sorceTime);

	int loadLine;
};