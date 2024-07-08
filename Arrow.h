#pragma once
#include "Enemy.h"

class Arrow :
    public Enemy
{
    int hImage;
    bool isRight;
    float cdTimer;

	enum State {
		NORMAL,
		HIT,
	};
	State state;
public:
	Arrow(GameObject* parent);
	~Arrow();
	void Update() override;
	void Draw() override;
	void SetArrow(int x, int y,bool isright);
	bool CollideRectToRect(float x, float y, float w, float h) override;
	void KillEnemy() override;
	bool IsSteppedOnHead(float x, float y, float w, float h) override;
};

