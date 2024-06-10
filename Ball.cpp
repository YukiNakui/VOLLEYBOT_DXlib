#include "Ball.h"
#include"Field.h"
#include <DxLib.h>
#include <assert.h>

namespace {
	const float BALL_E = 0.8f;
	const float GRAVITY = 9.8f / 60.0f;
	const float TOSS_HIGHT = 64.0f * 4.0f;
}

Ball::Ball(GameObject* parent)
	:GameObject(parent,"Ball")
{
	hImage = LoadGraph("Assets/ball.png");
	assert(hImage > 0);
	moveVec = XMVectorZero();
	isAlive = true;
}

Ball::~Ball()
{
	if (hImage > 0)
	{
		DeleteGraph(hImage);
	}
}

void Ball::Initialize()
{
	
}

void Ball::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	
	XMVECTOR vBall = XMLoadFloat3(&transform_.position_);
	
	XMFLOAT3 move;
	XMStoreFloat3(&move, moveVec);
	
	float pushRUp = pField->CollisionRight(transform_.position_.x + 16.0f,transform_.position_.y - 13.0f);
	float pushRDown = pField->CollisionRight(transform_.position_.x + 16.0f, transform_.position_.y + 13.0f);
	float pushR = max(pushRDown, pushRUp);
	if (pushR > 0.0f) {
		transform_.position_.x -= pushR;
		move.x = -move.x * BALL_E;
	}
	float pushLUp = pField->CollisionLeft(transform_.position_.x - 16.0f, transform_.position_.y - 13.0f);
	float pushLDown = pField->CollisionLeft(transform_.position_.x - 16.0f, transform_.position_.y + 13.0f);
	float pushL = max(pushLDown, pushLUp);
	if (pushL > 0.0f) {
		transform_.position_.x += pushL;
		move.x = -move.x * BALL_E;
	}

	float pushURight = pField->CollisionUp(transform_.position_.x + 13.0f, transform_.position_.y - 16.0f);
	float pushULeft = pField->CollisionUp(transform_.position_.x - 13.0f, transform_.position_.y - 16.0f);
	float pushU = max(pushURight, pushULeft);
	if (pushU > 0.0f) {
		transform_.position_.y += pushU;
		move.y = -move.y * BALL_E;
	}
	float pushDRight = pField->CollisionDown(transform_.position_.x + 13, transform_.position_.y + 16);
	float pushDLeft = pField->CollisionDown(transform_.position_.x - 13, transform_.position_.y + 16);
	float pushD = max(pushDRight, pushDLeft);
	if (pushD > 0.0f) {
		transform_.position_.y -= pushD;
		move.y = -move.y * BALL_E;
		XMVECTOR v = XMVector3Length(moveVec);
		float length = XMVectorGetX(v);
		if (length < 1.0f)
		{
			//moveVec = XMVectorZero();
			isAlive = false;
			KillMe();
		}
	}

	moveVec = XMLoadFloat3(&move);
	XMVECTOR gVec = XMVectorSet(0.0f, GRAVITY, 0.0f, 0.0f);
	moveVec += gVec;
	vBall += moveVec;
	XMStoreFloat3(&transform_.position_, vBall);
	

	if (transform_.position_.y > 720.0f) {
		isAlive = false;
		KillMe();
	}
}

void Ball::Draw()
{
	DrawRotaGraph(transform_.position_.x, transform_.position_.y,1.0, 0, hImage,TRUE);
}

void Ball::SetPosition(XMFLOAT3 pos)
{
	transform_.position_ = pos;
}

void Ball::SpikeBall(bool isRight)
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

void Ball::TossBall()
{
	isAlive = true;
	float tossSpeed = -sqrt(2 * GRAVITY * TOSS_HIGHT);
	moveVec = { 0.0f,  tossSpeed, 0.0f,0.0f};
}

bool Ball::GetIsBallAlive()
{
	return isAlive;
}

