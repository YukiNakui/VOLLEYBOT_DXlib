#pragma once
#include "Engine/GameObject.h"

namespace ENEMY_ANIMFRAME{
	const int WALK_MAXFRAME{ 11 };
	const int DEAD_MAXFRAME{ 2 };
}
namespace EAF = ENEMY_ANIMFRAME;

class Enemy :
    public GameObject
{
	int hWalkImage[EAF::WALK_MAXFRAME];
	int hDeadImage[EAF::DEAD_MAXFRAME];
	int animFrame;
	int frameCounter;
	bool isRight;
	bool onGround;

	float jumpSpeed;
public:
	Enemy(GameObject* scene);
	~Enemy();
	void Update() override;
	void Draw() override;
	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h);
};

