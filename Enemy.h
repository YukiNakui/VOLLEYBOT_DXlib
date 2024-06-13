#pragma once
#include "Engine/GameObject.h"

const int WALK_MAXFRAME{ 11 };

class Enemy :
    public GameObject
{
	int hImage;
	int hWalkImage[WALK_MAXFRAME];
	int animFrame;
	int frameCounter;
public:
	Enemy(GameObject* scene);
	~Enemy();
	void Update() override;
	void Draw() override;
	void SetPosition(int x, int y);
	bool CollideRectToCircle(float x, float y, float r);
};

