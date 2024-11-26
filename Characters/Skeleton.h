#pragma once
#include "Enemy.h"

class Skeleton :
    public Enemy
{
	int hAnimImg;
	int animFrame;
	int frameCounter;
	int animType;
	bool isRight;
	bool onGround;
	bool afterShot;

	enum State {
		NORMAL,
		SHOT,
		DEAD,
	};
	State state;

	float jumpSpeed;

	float cdTimer;
public:
	Skeleton(GameObject* parent);
	~Skeleton();
	void Update() override;
	void Draw() override;
	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h) override;
	void KillEnemy() override;
	bool IsSteppedOnHead(float x, float y, float w, float h) override;
private:
	int shotSound;
	int deathSound;
};

