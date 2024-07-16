#pragma once
#include "Engine/GameObject.h"

class HPBar :
    public GameObject
{
	int hbarImage;
	int hHartImage;
public:
	HPBar(GameObject* parent);
	~HPBar();
	void Update() override;
	void Draw() override;
};

