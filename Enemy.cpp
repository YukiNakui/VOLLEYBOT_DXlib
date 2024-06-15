#include "Enemy.h"
#include"Field.h"
#include "Engine/time.h"
#include"Camera.h"

namespace {
	const float ENEMY_WIDTH = 128.0f;
	const float ENEMY_HEIGHT = 128.0f;
	const float MOVE_SPEED = 1.5f;
	const float GRAVITY = 9.8f / 60.0f;//重力加速度
	const float CORRECT_WIDTH = 35.0f;
	const float CORRECT_BOTTOM = 1.0f;
	const float CORRECT_TOP = 35.0f;
}

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
	isRight = true;
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
	Field* pField = GetParent()->FindGameObject<Field>();
	frameCounter ++;
	if (frameCounter >= WALK_MAXFRAME) {
		animFrame = (animFrame + 1) % WALK_MAXFRAME;
		frameCounter = 0;
	}
	if (isRight) {
		transform_.position_.x += MOVE_SPEED;
	}
	else {
		transform_.position_.x -= MOVE_SPEED;
	}

	

	if (pField != nullptr) {
		float cx = ENEMY_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = ENEMY_HEIGHT / 2.0f;

		float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		transform_.position_.y += GRAVITY;

		int pushRbottom = pField->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pField->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
		}

		int pushRtop = pField->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pField->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			
		}
	}
}

void Enemy::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	DrawRotaGraph(x, y,1.0f, 0, hWalkImage[animFrame], TRUE,!isRight);
}

void Enemy::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Enemy::CollideRectToCircle(float x, float y, float r)
{
	bool result = false;
	float myRectRight = transform_.position_.x + ENEMY_WIDTH / 2.0f;
	float myRectLeft = transform_.position_.x - ENEMY_WIDTH / 2.0f;
	float myRectTop = transform_.position_.y + ENEMY_HEIGHT / 2.0f;
	float myRectBottom = transform_.position_.y - ENEMY_HEIGHT / 2.0f;
	//// 矩形の四辺に対して円の半径分だけ足したとき円が重なっていたら
	//if ((x < myRectRight + r) && (x > myRectLeft - r) && (y > myRectTop - r) && (y < myRectBottom + r)) {
	//	result = true;
	//	if (x < myRectLeft) {//矩形の左
	//		float dx = myRectLeft - x;
	//		if (y < myRectTop) {//矩形の左上
	//			float dy = myRectTop - y;
	//			if ( sqrt((dx*dx)+(dy+dy)) >= r){
	//				result = false;
	//			}
	//		}
	//		else {
	//			if (y > myRectBottom){//矩形の左下
	//				float dy = myRectBottom - y;
	//				if (sqrt((dx * dx) + (dy + dy)) >= r){
	//					result = false;
	//				}
	//			}
	//		}
	//	}
	//	else {
	//		if (x > myRectRight) {//矩形の右
	//			float dx = myRectRight - x;
	//			if (y < myRectTop) {//矩形の右上
	//				float dy = myRectTop - y;
	//				if (sqrt((dx * dx) + (dy + dy)) >= r){
	//					result = false;
	//				}
	//			}
	//			else {
	//				if (y > myRectBottom){//矩形の右下
	//					float dy = myRectBottom - y;
	//					if (sqrt((dx * dx) + (dy + dy)) >= r){
	//						result = false;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//return result;
	float cx = 0.0f;
	float cy = 0.0f;
	if (x <= ENEMY_WIDTH/2.0f) {
		cx = -ENEMY_WIDTH / 2.0f;
	}
	else if (x >= ENEMY_WIDTH / 2.0f) {
		cx = ENEMY_WIDTH / 2.0f;
	}
	else {
		cx = x;
	}
	if (y <= ENEMY_HEIGHT / 2.0f) {
		cy = -ENEMY_HEIGHT / 2.0f;
	}
	else if (y >= ENEMY_HEIGHT / 2.0f) {
		cy = ENEMY_HEIGHT / 2.0f;
	}
	else {
		cy = y;
	}


	//角との判定
	double dx = cx - x;
	double dy = cy - y;
	return (sqrt(dx + dy) <= r );
}

bool Enemy::CollideRectToRect(float x, float y, float w, float h)
{
	float myRectRight = transform_.position_.x + ENEMY_WIDTH / 2.0f - CORRECT_WIDTH;
	float myRectLeft = transform_.position_.x - ENEMY_WIDTH / 2.0f + CORRECT_WIDTH;
	float myRectTop = transform_.position_.y - ENEMY_HEIGHT / 2.0f + CORRECT_TOP;
	float myRectBottom = transform_.position_.y + ENEMY_HEIGHT / 2.0f - CORRECT_BOTTOM;
	if (((x - w / 2.0f > myRectLeft && x - w / 2.0f < myRectRight) ||(myRectLeft > x - w / 2.0f && myRectLeft < x + w / 2.0f)) &&
		((y - h / 2.0f > myRectTop && y - h / 2.0f < myRectBottom) ||(myRectTop > y - h / 2.0f && myRectTop < y + h / 2.0f))) {
		return true;
	}
	else {
		return false;
	}
	return false;
}
