#include "Ball.h"
#include"Field.h"
#include <DxLib.h>
#include <assert.h>

namespace {
	const float BALL_E = 0.8;
	const float GRAVITY = 9.8f / 60.0f;
	const float TOSS_HIGHT = 64.0f * 4.0f;
}

Ball::Ball(GameObject* parent)
	:GameObject(parent,"Ball")
{
	hImage = LoadGraph("Assets/ball.png");
	assert(hImage > 0);
	speed = 0.0f;
	vBall = XMVectorZero();
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
	
	
	float pushR = pField->CollisionRight(transform_.position_.x + 16,transform_.position_.y);
	if (pushR >= 1.0f) {
		transform_.position_.x -= pushR;
	}
	float pushL = pField->CollisionLeft(transform_.position_.x - 16, transform_.position_.y);
	if (pushL >= 1.0f) {
		transform_.position_.x += pushL;
	}

	speed += GRAVITY;
	transform_.position_.y += speed;

	float pushU = pField->CollisionUp(transform_.position_.x, transform_.position_.y + 16);
	if (pushU >= 1.0f) {
		transform_.position_.y -= pushU + 1;
	}
	float pushD = pField->CollisionDown(transform_.position_.x, transform_.position_.y - 16);
	if (pushD >= 1.0f) {
		transform_.position_.y += pushD + 1;
	}

	if (transform_.position_.y > 720.0f)
		KillMe();
}

void Ball::Draw()
{
	DrawRotaGraph(transform_.position_.x, transform_.position_.y,1.0, 0, hImage,TRUE);
}

void Ball::SetPosition(XMFLOAT3 pos)
{
	transform_.position_ = pos;
}

void Ball::SpikeBall()
{
	speed = 6.0f;
}

void Ball::TossBall()
{
	speed = -sqrt(2 * GRAVITY * TOSS_HIGHT);
}
