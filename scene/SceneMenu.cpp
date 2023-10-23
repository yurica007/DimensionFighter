#include "SceneMenu.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "../InputState.h"
#include <sstream>
#include <fstream>

namespace
{
	const VECTOR initMenuPos = { 500.0f, 360.0f, 0.0f };

	const char* const menuFilePath = "data/MenuData.csv";

	enum
	{
		menuName
	};
}

SceneMenu::SceneMenu(SceneManager& manager) :
	SceneBase(manager), currentIndex(0)
{
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Initialize()
{
	ifstream ifs(menuFilePath);
	string line;
	vector<Menu> data;

	while (getline(ifs, line))
	{
		if (line == "ƒƒjƒ…[–¼") continue;
		vector<string> strvec = split(line, ',');
		Menu menuData;
		menuData.name = strvec[menuName];
		data.push_back(menuData);
	}

	menu.resize(data.size());

	for (int i = 0; i < menu.size(); i++)
	{
		menu[i].name = data[i].name;
		if (i == 0) menu[i].pos = initMenuPos;
		else menu[i].pos = VAdd(menu[i - 1].pos, { 280.0f, 0.0f, 0.0f });
	}
}

void SceneMenu::Update(const InputState& inputState)
{
	if (inputState.IsTriggered(InputType::left) && 0 < currentIndex)
	{
		currentIndex--;
	}
	else if (inputState.IsTriggered(InputType::right) && currentIndex < menu.size() - 1)
	{
		currentIndex++;
	}

	if (IsTransScene(inputState)) return;
}

void SceneMenu::Draw()
{
	for (int i = 0; i < menu.size(); i++)
	{
		DrawStringF(menu[i].pos.x, menu[i].pos.y, menu[i].name.c_str(), 0xffffff);
		
		if (currentIndex == i)
		{
			DrawStringF(menu[i].pos.x - 30, menu[i].pos.y, "Z", 0xffffff);
		}
	}
	DrawBox(0, 1280, 0, 720, 0xaaaaaa, true);
}

void SceneMenu::Finalize()
{
}

bool SceneMenu::IsTransScene(const InputState& inputState)
{
	if (inputState.IsTriggered(InputType::select))
	{
		switch (currentIndex)
		{
		case 0:
		{
			manager_.PopScene();
			SceneGame* prevScene = new SceneGame(manager_);
			const int currentTime = GetNowCount();
			prevScene->SetTranceTime(currentTime);
			break;
		}
		case 1:
		{
			SceneTitle* nextScene = new SceneTitle(manager_);
			manager_.ChangeScene(nextScene);
			nextScene->Initialize();
			break;
		}			
		case 2:
			manager_.EndScene();
			break;
		default:
			return false;
		}
		return true;
	}

	return false;
}

vector<string> SceneMenu::split(const string& input, const char delimiter)
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
