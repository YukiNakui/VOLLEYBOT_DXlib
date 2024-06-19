#pragma once
#include "Engine/GameObject.h"

class GoalObj :
    public GameObject
{
	int hImage;
public:
	GoalObj(GameObject* scene);
	~GoalObj();
	void Update() override;
	void Draw() override;
};

