#include "Ball.h"
#include"Field.h"
#include <DxLib.h>
#include <assert.h>

const int BALL_E = 0.8;

Ball::Ball(GameObject* parent)
{
	hImage = LoadGraph("Assets/ball.png");
	assert(hImage > 0);
	speed = 0.0f;
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
}

void Ball::Draw()
{
}

void Ball::Release()
{
}

void Ball::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}
