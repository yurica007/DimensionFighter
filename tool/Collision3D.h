#pragma once
#include "../StageData.h"
#include "../character/CharaData.h"
#include "../PosData.h"
#include <DxLib.h>
#include <vector>

struct SphereData
{
	VECTOR pos = { 0.0f, 0.0f, 0.0f };
	double radius = 0.0;
};

class InputState;
class Stage;

class Collision3D
{
public:
	Collision3D(Stage& stage);
	~Collision3D() {}

	void Initialize();
	void Update(const VECTOR pos);
	void Draw();
	void Finalize();

	/// <summary>
	/// ステージの当たり判定範囲を更新する
	/// </summary>
	void UpdateCollRange();

	/// <summary>
	/// ステージとの当たり判定
	/// </summary>
	/// <returns>true：当たっている false：当たっていない</returns>
	const std::vector<bool> IsCollideStage();

private:
	/// <summary>
	/// ステージ端の当たり判定範囲を更新する
	/// </summary>
	/// <param name="deadNum">死んだキャラクター数</param>
	void UpdateOutsideVerPos(const int decreaseWidth);

	/// <summary>
	/// 当たり判定との最短距離とキャラクターの半径を比較する
	/// </summary>
	/// <param name="vertexPos">当たり判定の頂点情報</param>
	/// <param name="direct">当たっている方向</param>
	/// <param name="collide">当たり判定</param>
	void ComparisonDistanceAndRadius(const std::vector<VECTOR> vertexPos, Direct& direct, std::vector<bool>& collide);

	/// <summary>
	/// 最短距離を取得する
	/// </summary>
	/// <param name="vertexPos">当たり判定の頂点情報</param>
	/// <param name="direct">当たっている方向</param>
	/// <returns>最短距離</returns>
	const float GetShortDistance(const std::vector<VECTOR> vertexPos, Direct& direct);

	/// <summary>
	/// 3次元ベクトルを累乗した合計を取得する
	/// </summary>
	/// <param name="dist">3次元ベクトル</param>
	/// <returns>ベクトルの合計</returns>
	const float GetVector(const VECTOR dist);

	/// <summary>
	/// 球と球との当たり判定
	/// </summary>
	/// <param name="targetPos">対象の球のデータ</param>
	/// <param name="sourcePos">自身の球のデータ</param>
	/// <returns>true：当たっている false：当たっていない</returns>
	const bool CollSphereAndSpher(const SphereData targeSphere, const SphereData sourceSphere);

	SphereData sphere;
	Stage& stage;

	int prevStageSize;

	std::vector<std::vector<VECTOR>> insideVerPos;
	std::vector<std::vector<VECTOR>> outsideVerPos;
};

