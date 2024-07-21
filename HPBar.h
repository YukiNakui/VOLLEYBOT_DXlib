#pragma once
#include "Engine/GameObject.h"

class HPBar :
    public GameObject
{
	int hBarImage;
	int hHartImage;
	int hBlackHartImage;
public:
	HPBar(GameObject* parent);
	~HPBar();
	void Update() override;
	void Draw() override;
	void GetHP(int hp);
private:
	float hpBarPosX;
	float hpBarPosY;
	float hartPosX;
	float hartPosY;
};

