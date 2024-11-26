#pragma once
#include "../Engine/GameObject.h"
#include"Enemy.h"

//namespace WOLF_ANIMFRAME{
//	const int WALK_MAXFRAME{ 11 };
//	const int DEAD_MAXFRAME{ 2 };
//}
//namespace WAF = WOLF_ANIMFRAME;

class Wolf :
    public Enemy
{
	/*int hWalkImage[WAF::WALK_MAXFRAME];
	int hDeadImage[WAF::DEAD_MAXFRAME];*/
	int hAnimImg;
	int animFrame;
	int frameCounter;
	int animType;
	bool isRight;
	bool onGround;

	enum State {
		NORMAL,
		ATTACK,
		DEAD,
	};
	State state;

	float jumpSpeed;

	float cdTimer;
public:
	Wolf(GameObject* parent);
	~Wolf();
	void Update() override;
	void Draw() override;
	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h) override;
	void KillEnemy() override;
	bool IsSteppedOnHead(float x, float y, float w, float h) override;
private:
	bool ViewInPlayer(XMFLOAT3 pos);
	XMFLOAT3 targetPos;
	XMVECTOR viewVec;
	float accel;
	bool attackRight;
	bool attackAfter;
	float attackAfterTimer;

	int roarSound;
	int deathSound;
};

