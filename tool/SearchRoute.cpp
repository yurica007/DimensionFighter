#include "SearchRoute.h"
#include "InputState.h"
#include <DxLib.h>
#include <algorithm>

namespace
{
	using namespace std;
}

SearchRoute::SearchRoute() :
	direct(Direct::left)
{
}

void SearchRoute::Initialize(vector<PosData> posData, const int charaNum)
{
	searchCharaPos = { posData[charaNum].x, posData[charaNum].y };

	vector<PosData> charaPos;

	for (int i = 0; i < posData.size(); i++)
	{
		if (i != charaNum) charaPos.push_back(posData[i]);
	}

	nearChara = GetNearTargetPos(charaPos);
	direct = GetNearTargetDirect();
}

void SearchRoute::Finalize()
{
}

const PosData SearchRoute::GetNearTargetPos(vector<PosData> charaPos)
{
	vector<PosData> enemyPos;
	vector<int> heuristic;

	//�q���[���X�e�B�b�N�����𑪒�
	for (int i = 0; i < charaPos.size(); i++)
	{
		const PosData index = { charaPos[i].x, charaPos[i].y };
		heuristic.push_back(GetHeuristic(index, searchCharaPos));

		enemyPos.push_back(index);
	}

	//���肵���q���[���X�e�B�b�N�������r���A�ł��������l�̑Ώۂ��ŒZ�����ɂ���
	vector<int> sarch = heuristic;
	sort(sarch.begin(), sarch.end());

	for (int i = 0; i < heuristic.size(); i++)
	{
		if (sarch.front() == heuristic[i]) return enemyPos[i];
	}

	return enemyPos.front();
	//if (heuristic[1] < heuristic[0] && heuristic[1] <= heuristic[2]) nearTarget = enemyPos[1];
	//else if (heuristic[2] < heuristic[0] && heuristic[2] <= heuristic[1]) nearTarget = enemyPos[2];
}

const Direct SearchRoute::GetNearTargetDirect()
{
	const int distX = abs(nearChara.x - searchCharaPos.x);
	const int distY = abs(nearChara.y - searchCharaPos.y);

	//�c���̂��߂���������������
	//�X�e�[�W�̔z��ƃ`�F�b�N����z���Y���t�̂��ߓn�������͋t����
	if (distY < distX)
	{
		if (nearChara.x - searchCharaPos.x < 0) return Direct::left;
		if (0 < nearChara.x - searchCharaPos.x) return Direct::right;
	}
	else
	{
		if (0 < nearChara.y - searchCharaPos.y) return Direct::back;
		if (nearChara.y - searchCharaPos.y < 0) return Direct::front;
	}

	return Direct::front;
}

const int SearchRoute::GetHeuristic(const PosData target, const PosData source)
{
	const int distX = abs(target.x - source.x);
	const int distY = abs(target.y - source.y);

	return distX + distY;
}