#pragma once
#include "SceneData.h"
#include <vector>

class BackGround
{
public:
	BackGround();
	~BackGround() {}

	void Initialize(const SceneType type);
	void Update();
	void Draw();
	void Finalize();

private:
	enum class Scroll
	{
		left,
		right,
		up,
		down,
		all
	};

	SceneType sceneType;
	Scroll scrollType;

	std::vector<int> backGroundData;
	std::vector<int> colorData;

	int backGroundNum;
	int colorNum;

	int sizeX;
	int sizeY;

	float scrollX;
	float scrollY;
	float scroll;
};

