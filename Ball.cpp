#include "Ball.h"
#include"Field.h"
#include <DxLib.h>
#include <assert.h>
#include"Enemy.h"
#include"Player.h"
#include"Camera.h"
#include"ItemBox.h"
#include"GoalObj.h"
#include"Engine/SceneManager.h"

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
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();
	
	XMVECTOR vBall = XMLoadFloat3(&transform_.position_);
	XMFLOAT3 move;
	XMStoreFloat3(&move, moveVec);

	// 次のフレームでの位置を計算
	XMVECTOR nextPos = vBall + moveVec + XMVectorSet(0.0f, GRAVITY, 0.0f, 0.0f);
	XMFLOAT3 nextPosFloat;
	XMStoreFloat3(&nextPosFloat, nextPos);

	if (pField != nullptr) {
		float cx = BALL_WIDTH / 2.0f;
		float cy = BALL_HEIGHT / 2.0f;
		// 地形との衝突判定と位置補正
		float pushRTop = pField->CollisionRight(nextPosFloat.x + cx, nextPosFloat.y - cy + CORRECT_VALUE);
		float pushRBottom = pField->CollisionRight(nextPosFloat.x + cx, nextPosFloat.y + cy - CORRECT_VALUE);
		float pushR = max(pushRBottom, pushRTop);
		if (pushR > 0.0f) {
			transform_.position_.x -= pushR - 1.0f;
			move.x = -move.x * BALL_E;
		}
		float pushLTop = pField->CollisionLeft(nextPosFloat.x - cx, nextPosFloat.y - cy + CORRECT_VALUE);
		float pushLBottom = pField->CollisionLeft(nextPosFloat.x - cx, nextPosFloat.y + cy - CORRECT_VALUE);
		float pushL = max(pushLBottom, pushLTop);
		if (pushL > 0.0f) {
			transform_.position_.x += pushL - 1.0f;
			move.x = -move.x * BALL_E;
		}

		float pushTRight = pField->CollisionUp(nextPosFloat.x + cx - CORRECT_VALUE, nextPosFloat.y - cy);
		float pushTLeft = pField->CollisionUp(nextPosFloat.x - cx + CORRECT_VALUE, nextPosFloat.y - cy);
		float pushT = max(pushTRight, pushTLeft);
		if (pushT > 0.0f) {
			transform_.position_.y += pushT - 1.0f;
			move.y = -move.y * BALL_E;
		}
		float pushBRight = pField->CollisionDown(nextPosFloat.x + cx - CORRECT_VALUE, nextPosFloat.y + cy);
		float pushBLeft = pField->CollisionDown(nextPosFloat.x - cx + CORRECT_VALUE, nextPosFloat.y + cy);
		float pushB = max(pushBRight, pushBLeft);
		if (pushB > 0.0f) {
			transform_.position_.y -= pushB - 1.0f;
			move.y = -move.y * BALL_E;
			XMVECTOR v = XMVector3Length(moveVec);
			float length = XMVectorGetX(v);
			if (length < 5.0f)
			{
				isAlive = false;
				transform_.position_.y = 720.0f;
				//KillMe();
			}
		}
	}
	for (ItemBox* pIBox : pIBoxs) {
		float cx = BALL_WIDTH / 2.0f;
		float cy = BALL_HEIGHT / 2.0f;
		// アイテムボックスとの衝突判定と位置補正
		float pushRTop = pIBox->CollisionRight(nextPosFloat.x + cx, nextPosFloat.y - cy + CORRECT_VALUE);
		float pushRBottom = pIBox->CollisionRight(nextPosFloat.x + cx, nextPosFloat.y + cy - CORRECT_VALUE);
		float pushR = max(pushRBottom, pushRTop);
		if (pushR > 0.0f) {
			transform_.position_.x -= pushR - 1.0f;
			move.x = -move.x * BALL_E;
		}
		float pushLTop = pIBox->CollisionLeft(nextPosFloat.x - cx, nextPosFloat.y - cy + CORRECT_VALUE);
		float pushLBottom = pIBox->CollisionLeft(nextPosFloat.x - cx, nextPosFloat.y + cy - CORRECT_VALUE);
		float pushL = max(pushLBottom, pushLTop);
		if (pushL > 0.0f) {
			transform_.position_.x += pushL - 1.0f;
			move.x = -move.x * BALL_E;
		}

		float pushTRight = pIBox->CollisionUp(nextPosFloat.x + cx - CORRECT_VALUE, nextPosFloat.y - cy);
		float pushTLeft = pIBox->CollisionUp(nextPosFloat.x - cx + CORRECT_VALUE, nextPosFloat.y - cy);
		float pushT = max(pushTRight, pushTLeft);
		if (pushT > 0.0f) {
			transform_.position_.y += pushT - 1.0f;
			move.y = -move.y * BALL_E;
		}
		float pushBRight = pIBox->CollisionDown(nextPosFloat.x + cx - CORRECT_VALUE, nextPosFloat.y + cy);
		float pushBLeft = pIBox->CollisionDown(nextPosFloat.x - cx + CORRECT_VALUE, nextPosFloat.y + cy);
		float pushB = max(pushBRight, pushBLeft);
		if (pushB > 0.0f) {
			transform_.position_.y -= pushB - 1.0f;
			move.y = -move.y * BALL_E;
			XMVECTOR v = XMVector3Length(moveVec);
			float length = XMVectorGetX(v);
			if (length < 5.0f)
			{
				isAlive = false;
				transform_.position_.y = 720.0f;
				//KillMe();
			}
		}
	}

	moveVec = XMLoadFloat3(&move);
	moveVec += XMVectorSet(0.0f, GRAVITY, 0.0f, 0.0f);
	vBall += moveVec;
	XMStoreFloat3(&transform_.position_, vBall);

	std::list<Enemy*> pEnemies = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y, BALL_WIDTH / 2.0f, BALL_HEIGHT / 2.0f)) {
			pEnemy->KillMe();
			isAlive = false;
			transform_.position_.y = 720.0f;
		}
	}

	GoalObj* pGoal = GetParent()->FindGameObject<GoalObj>();
	if (pGoal->CollideRectToRect(transform_.position_.x, transform_.position_.y, BALL_WIDTH / 2.0f, BALL_HEIGHT / 2.0f)) {
		pGoal->Goal();
		isAlive = false;
		transform_.position_.y = 720.0f;
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
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
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	if (isAlive)
		DrawRotaGraph(x, y, 1.0, 0, hImage, TRUE);
}

void Ball::SetPosition(float x,float y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

void Ball::Spike(bool isRight)
{
	isAlive = true;

	if (isRight)
	{
		moveVec = { 10.0f,10.0f,0.0f,0.0f };
	}
	else
	{
		moveVec = { -10.0f,10.0f,0.0f,0.0f };
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
	if (len < 25.0f) {
		isAlive = false;
		return true;
	}
	return false;
}

int Ball::GetBallSize()
{
	return BALL_WIDTH;
}

