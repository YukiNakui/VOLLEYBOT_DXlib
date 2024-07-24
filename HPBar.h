#pragma once
#include "Engine/GameObject.h"

class HPBar :
    public GameObject
{
	int hBarImage;
	int hHartImage;
	int hBlackHartImage;
	int manuImage;
public:
	HPBar(GameObject* parent);
	~HPBar();
	void Update() override;
	void Draw() override;
	void SetHP(int hp);
private:
	int playerHP;
};

