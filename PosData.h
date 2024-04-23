#pragma once

struct PosData
{
	int x = -1;
	int y = -1;
};

enum class Direct
{
	left,
	right,
	back,
	front,
	all
};

namespace
{
	constexpr int leftDirect = static_cast<int>(Direct::left);
	constexpr int rightDirect = static_cast<int>(Direct::right);
	constexpr int backDirect = static_cast<int>(Direct::back);
	constexpr int frontDirect = static_cast<int>(Direct::front);
	constexpr int DirectNum = static_cast<int>(Direct::all);
}
