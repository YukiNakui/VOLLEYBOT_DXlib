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
	DrawRotaGraph(x, y,1.0f, 0, hWalkImage[animFrame], TRUE);
}

void Enemy::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Enemy::CollideRectToCircle(float x, float y, float r)
{
	bool result = false;
	float myRectRight = transform_.position_.x + 64.0f;
	float myRectLeft = transform_.position_.x - 64.0f;
	float myRectTop = transform_.position_.y + 64.0f;
	float myRectBottom = transform_.position_.y - 64.0f;
	// 矩形の四辺に対して円の半径分だけ足したとき円が重なっていたら
	if ((x < myRectRight + r) && (x > myRectLeft - r) && (y > myRectTop - r) && (y < myRectBottom + r)) {
		result = true;
		if (x < myRectLeft) {//矩形の左
			float dx = myRectLeft - x;
			if (y < myRectTop) {//矩形の左上
				float dy = myRectTop - y;
				if ( sqrt((dx*dx)+(dy+dy)) >= r)
				{
					result = false;
				}
			}
			else {
				if (y > myRectBottom)//矩形の左下
				{
					float dy = myRectBottom - y;
					if (sqrt((dx * dx) + (dy + dy)) >= r)
					{
						result = false;
					}
				}
			}
		}
		else {
			if (x > myRectRight) {//矩形の右
				float dx = myRectRight - x;
				if (y < myRectTop) {//矩形の右上
					float dy = myRectTop - y;
					if (sqrt((dx * dx) + (dy + dy)) >= r)
					{
						result = false;
					}
				}
				else {
					if (y > myRectBottom)//矩形の右下
					{
						float dy = myRectBottom - y;
						if (sqrt((dx * dx) + (dy + dy)) >= r)
						{
							result = false;
						}
					}
				}
			}
		}
	}
	return result;
}
