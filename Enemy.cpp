#include "Enemy.h"
#include"Field.h"
#include "Engine/time.h"

Enemy::Enemy(GameObject* scene)
{
	hWalkImage[WALK_MAXFRAME];
	/*hImage = LoadGraph("Assets/EnemyWalk.png");
	assert(hImage > 0);*/
	LoadDivGraph("Assets/EnemyWalk.png", WALK_MAXFRAME, WALK_MAXFRAME, 1, 128, 128, hWalkImage);
	for (int i = 0; i < WALK_MAXFRAME; i++)
		assert(hWalkImage[i] > 0);
	animFrame = 0;
	frameCounter = 0;
}

Enemy::~Enemy()
{
	for (int i = 0; i < WALK_MAXFRAME; i++) {
		if (hWalkImage[i] > 0)
		{
			DeleteGraph(hWalkImage[i]);
		}
	}
}

void Enemy::Update()
{
	frameCounter ++;
	if (frameCounter >= WALK_MAXFRAME) {
		animFrame = (animFrame + 1) % WALK_MAXFRAME;
		frameCounter = 0;
	}
}

void Enemy::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	DrawGraph(x, y, hWalkImage[animFrame], TRUE);
}

void Enemy::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}
