#include "UIBase.h"
#include <sstream>
#include <fstream>

UIBase::UIBase() :
	uiPos({ 0.0f, 0.0f, 0.0f })
{
}

void UIBase::Initialize(const VECTOR pos)
{
}

void UIBase::Update()
{
}

void UIBase::Draw()
{
}

void UIBase::Finalize()
{
}

vector<string> UIBase::split(const string& input, const char delimiter)
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
