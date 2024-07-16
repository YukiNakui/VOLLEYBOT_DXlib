#pragma once
#include "Engine/GameObject.h"

class BlackLoad :
    public GameObject
{
    int hImage;
public:
	BlackLoad(GameObject* parent);
	~BlackLoad();
	void Update() override;
	void Draw() override;
};

