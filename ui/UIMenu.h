#pragma once
#include "../scene/SceneBase.h"
#include "../SceneData.h"
#include <vector>

struct MenuHandle
{
	int posX = 0;
	int posY = 0;
	double extRate = 1.0;
	double angle = 0.0;
	int handle = -1;
};

enum FrameType
{
	backFrame,
	selectFrame,
	notselectFrame
};

enum CursorType
{
	left,
	right
};

class Camera;
class Easing;
class ReuseData;

class UIMenu
{
public:
	UIMenu(ReuseData& reuse, Camera& camera, const SceneType scene);
	~UIMenu() {}

	void Initialize(const std::vector<std::vector<MenuHandle>> handle, const int initIndex);
	void Update(const int newMenuIndex);
	void Draw(const SceneState sceneState);
	void Finalize();

private:
	void DecisionCursor();
	
	void DrawSelectMenuGraph();

	void DrawNotSelectMenuAndBackGound();

	ReuseData& reuseData;
	Camera& camera;
	SceneType sceneType;

	Easing* easing;

	std::vector<std::vector<MenuHandle>> handleData;
	int prevMenuIndex;

	std::vector<MenuHandle> cursorData;

	float zoomAnimTime;
	float turnAnimTime;

	bool isZoom;

	int screenGraph;
	int menuGraph;

	int sizeX;
	int sizeY;
};

