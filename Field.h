#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Wolf.h"
#include"Skeleton.h"
#include"Bird.h"
#include"ItemBox.h"
#include"GoalObj.h"

class Field :
    public GameObject
{
public:
	Field(GameObject* scene);
	~Field();
	void Reset();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 右側の点が当たっているか調べる
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <returns>めり込んだ量（ドット）</returns>
	float CollisionRight(float x, float y);

	/// <summary>
	/// 下の点が当たっているか調べる
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <returns>めり込んだ量（ドット）</returns>
	float CollisionDown(float x, float y);

	/// <summary>
	/// 左の点が当たっているか調べる
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <returns>めり込んだ量（ドット）</returns>
	float CollisionLeft(float x, float y);

	/// <summary>
	/// 上の点が当たっているか調べる
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <returns>めり込んだ量（ドット）</returns>
	float CollisionUp(float x, float y);

	void SetNextStage();//次のステージをセットする
	bool CanNextStageChange();//次のステージに変更できるか
	void GameObjectsReset();
	bool IsFall(float x, float y);
private:
	int hImage;

	bool IsWallBlock(int x, int y);
	int* Map;
	int width;
	int height;
	int stageNum;

	GoalObj* pGoal;
	ItemBox* pIBox;
	Player* pPlayer;
	Wolf* pWolf;
	Skeleton* pSkeleton;
	Bird* pBird;
};

