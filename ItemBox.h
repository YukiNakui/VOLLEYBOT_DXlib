#pragma once
#include "Engine/GameObject.h"

class ItemBox :
    public GameObject
{
	int hImage;
	bool isAfterHit;
	int itemInTheBox;
public:
	ItemBox(GameObject* scene);
	~ItemBox();
	void Update() override;
	void Draw() override;

	void SetPosition(int x,int y);
	bool CollideRectToRect(float x, float y, float w, float h);
	//csvのマップデータでアイテムボックスの上の数字によって箱の中身を決める
	int GetItemInTheBox(int x, int y);

	float CollisionRight(float x, float y);
	float CollisionDown(float x, float y);
	float CollisionLeft(float x, float y);
	float CollisionUp(float x, float y);
};

