#pragma once
#include "Enemy.h"

class Skeleton :
    public Enemy
{
public:
	Skeleton(GameObject* parent);
	~Skeleton();
	void Update() override;
	void Draw() override;
	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h) override;
};

