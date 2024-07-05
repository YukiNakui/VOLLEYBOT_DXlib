#pragma once
#include "Enemy.h"

class Bird :
    public Enemy
{
public:
	Bird(GameObject* parent);
	~Bird();
	void Update() override;
	void Draw() override;
	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h) override;
};

