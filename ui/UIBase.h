#pragma once
#include <string>
#include <vector>
#include <DxLib.h>

using namespace std;

class UIBase
{
public:
	UIBase();
	virtual ~UIBase() {}

	virtual void Initialize(const VECTOR pos);
	virtual void Update();
	virtual void Draw();
	virtual void Finalize();

protected:
	vector<string> split(const string& input, const char delimiter);

	VECTOR uiPos;
};

