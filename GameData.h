#pragma once

namespace
{
	// ウインドウモード設定
	constexpr bool windowMode = true;
	// ウインドウ名
	const char* const titleText = "ディメンションファイター";
	// ウインドウサイズ
	constexpr int screenWidth = 1280;
	constexpr int screenHeight = 720;
	// カラーモード
	constexpr int colorDepth = 32;		// 32 or 16
};

namespace
{
	constexpr int halfScreenWidth = screenWidth / 2;
	constexpr int halfScreenHeight = screenHeight / 2;
}