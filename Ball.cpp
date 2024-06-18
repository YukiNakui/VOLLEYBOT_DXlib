#include "Ball.h"
#include"Field.h"
#include <DxLib.h>
#include <assert.h>
#include"Enemy.h"
#include"Player.h"
#include"Camera.h"

namespace {
	const float BALL_E = 0.8f;
	const float GRAVITY = 9.8f / 60.0f;
	const float MAX_TOSS_HEIGHT = 64.0f * 6.0f;
	const float BALL_WIDTH = 32.0f;
	const float BALL_HEIGHT = 32.0f;
	const float CORRECT_VALUE = 5.0f;
}

Ball::Ball(GameObject* parent)
	:GameObject(parent,"Ball")
{
	hImage = LoadGraph("Assets/ball.png");
	assert(hImage > 0);
	moveVec = XMVectorZero();
	isAlive = true;
	r = 32.0f;
	speed = 0.0f;
	accsel = 0.0f;
}

Ball::~Ball()
{
	if (hImage > 0)
	{
		DeleteGraph(hImage);
	}
}

void Ball::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	
	XMVECTOR vBall = XMLoadFloat3(&transform_.position_);
	
	XMFLOAT3 move;
	XMStoreFloat3(&move, moveVec);
	
	if (pField != nullptr) {
		float cx = BALL_WIDTH / 2.0f;
		float cy = BALL_HEIGHT / 2.0f;
		float pushRTop = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_VALUE);
		float pushRBottom = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_VALUE);
		float pushR = max(pushRBottom, pushRTop);
		if (pushR > 0.0f) {
			transform_.position_.x -= pushR - 1.0f;
			move.x = -move.x * BALL_E;
		}
		float pushLTop = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_VALUE);
		float pushLBottom = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_VALUE);
		float pushL = max(pushLBottom, pushLTop);
		if (pushL > 0.0f) {
			transform_.position_.x += pushL - 1.0f;
			move.x = -move.x * BALL_E;
		}

		float pushTRight = pField->CollisionUp(transform_.position_.x + cx- CORRECT_VALUE, transform_.position_.y - cy);
		float pushTLeft = pField->CollisionUp(transform_.position_.x - cx+ CORRECT_VALUE, transform_.position_.y - cy);
		float pushT = max(pushTRight, pushTLeft);
		if (pushT > 0.0f) {
			transform_.position_.y += pushT - 1.0f;
			move.y = -move.y * BALL_E;
		}
		float pushBRight = pField->CollisionDown(transform_.position_.x + cx- CORRECT_VALUE, transform_.position_.y + cy);
		float pushBLeft = pField->CollisionDown(transform_.position_.x - cx+ CORRECT_VALUE, transform_.position_.y + cy);
		float pushB = max(pushBRight, pushBLeft);
		if (pushB > 0.0f) {
			transform_.position_.y -= pushB - 1.0f;
			move.y = -move.y * BALL_E;
			XMVECTOR v = XMVector3Length(moveVec);
			float length = XMVectorGetX(v);
			if (length < 1.0f)
			{
				isAlive = false;
				transform_.position_.y = 720.0f;
				//KillMe();
			}
		}
	}

	moveVec = XMLoadFloat3(&move);
	XMVECTOR gVec = XMVectorSet(0.0f, GRAVITY, 0.0f, 0.0f);
	moveVec += gVec;
	vBall += moveVec;
	XMStoreFloat3(&transform_.position_, vBall);
	
	std::list<Enemy*> pEnemies = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y, BALL_WIDTH/2.0f,BALL_HEIGHT/2.0f)) {
			pEnemy->KillMe();
			isAlive = false;
			transform_.position_.y = 720.0f;
		}
	}

	if (transform_.position_.y > 720.0f) {
		isAlive = false;
		KillMe();
	}
}

void Ball::Draw()
{
	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	if (isAlive)
		DrawRotaGraph(x, y, 1.0, 0, hImage, TRUE);
}

void Ball::SetPosition(XMFLOAT3 pos)
{
	transform_.position_ = pos;
}

void Ball::Spike(bool isRight)
{
	isAlive = true;

	if (isRight)
	{
		moveVec = { 8.0f,8.0f,0.0f,0.0f };
	}
	else
	{
		moveVec = { -8.0f,8.0f,0.0f,0.0f };
	}
}

void Ball::FirstToss()
{
	isAlive = true;
	float tossSpeed = -sqrt(2 * GRAVITY * MAX_TOSS_HEIGHT / 2.0f);
	moveVec = { 0.0f,  tossSpeed, 0.0f,0.0f};
}

void Ball::SecondToss()
{
	isAlive = true;
	float tossSpeed = -sqrt(2 * GRAVITY * MAX_TOSS_HEIGHT);
	moveVec = { 0.0f,  tossSpeed, 0.0f,0.0f };
}

bool Ball::IsBallAlive()
{
	return isAlive;
}

XMFLOAT3 Ball::GetPos()
{
	return transform_.position_;
}

bool Ball::IsBallCatch(float x,float y)
{
	float lenX = x - transform_.position_.x;
	float lenY = y - transform_.position_.y;
	float len = sqrt(lenX * lenX + lenY * lenY);
	if (len < 20.0f) {
		isAlive = false;
		return true;
	}
	return false;
}

