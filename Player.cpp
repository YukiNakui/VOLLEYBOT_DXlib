#include "Player.h"
#include <DxLib.h>
#include <assert.h>
//#include"Camera.h"
#include"Field.h"
#include"TestScene.h"
#include"Ball.h"

namespace {
	const float MOVE_SPEED = 2.5f;
	const float GROUND = 600.0f;
	const float JUMP_HIGHT = 64.0f * 4.0f;//ジャンプの高さ
	const float GRAVITY = 9.8f / 60.0f;//重力加速度
};

Player::Player(GameObject* parent) : GameObject(sceneTop)
{
	hImage = LoadGraph("Assets/chara.png");
	assert(hImage > 0);
	transform_.position_.x = 128.0f;
	transform_.position_.y = GROUND;
	jumpSpeed = 0.0f;
	onGround = true;
	isRight = true;
	pBall = nullptr;
	isBallAlive = false;
	/*frameCounter = 0;
	animType = 0;
	animFrame = 0;
	state = S_Walk;*/
}

Player::~Player()
{
	if (hImage > 0)
	{
		DeleteGraph(hImage);
	}
}

void Player::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	/*if (state == S_Cry) {
		frameCounter++;
		if (frameCounter >= 16)
		{
			frameCounter = 0;
			animFrame = (animFrame + 1) % 2;
		}
		return;
	}
	TestScene* scene = dynamic_cast<TestScene*>(GetParent());
	if (!scene->CanMove())
		return;*/
	
	if (CheckHitKey(KEY_INPUT_K))
	{
		if (!prevAttackKey && !isBallAlive) {
			pBall = Instantiate<Ball>(GetParent());
			isBallAlive = true;
			XMFLOAT3 ballPos = { transform_.position_.x + 32,transform_.position_.y,transform_.position_.z };
			pBall->SetPosition(ballPos);
			pBall->SpikeBall(isRight);
		}
		prevAttackKey = true;
	}
	else {
		prevAttackKey = false;
	}

	if (pBall == nullptr) {
		isBallAlive = false;
	}
	
	if (CheckHitKey(KEY_INPUT_D))
	{
		transform_.position_.x += MOVE_SPEED;
		/*if (++frameCounter >= 8) {
			animFrame = (animFrame + 1) % 4;
			frameCounter = 0;
		}*/
		int hitX = transform_.position_.x + 50;
		int hitY = transform_.position_.y + 63;
		if (pField != nullptr) {
			int pushDown = pField->CollisionRight(hitX, hitY);
			int pushUp = pField->CollisionRight(hitX, transform_.position_.y);
			int pushRight = max(pushDown, pushUp);//右側の頭と足元で当たり判定
			if (pushRight >= 1) {
				transform_.position_.x -= pushRight;
			}
		}
		isRight = true;
	}
	else if (CheckHitKey(KEY_INPUT_A))
	{
		transform_.position_.x -= MOVE_SPEED;
		/*if (++frameCounter >= 8) {
			animFrame = (animFrame + 1) % 4;
			frameCounter = 0;
		}*/
		int hitX = transform_.position_.x + 10;
		int hitY = transform_.position_.y + 63;
		if (pField != nullptr) {
			int pushDown = pField->CollisionLeft(hitX, hitY);
			int pushUp = pField->CollisionLeft(hitX, transform_.position_.y);
			int pushLeft = max(pushDown, pushUp);//左側の頭と足元で当たり判定
			if (pushLeft >= 1) {
				transform_.position_.x += pushLeft;
			}
		}
		isRight = false;
	}
	/*else {
		animFrame = 0;
		frameCounter = 0;
	}*/
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (prevSpaceKey == false) {
			if (onGround) {
				jumpSpeed = -sqrt(2 * GRAVITY * JUMP_HIGHT);
				onGround = false;
				animType = 2;
			}
		}
		prevSpaceKey = true;
	}
	else {
		prevSpaceKey = false;
	}

	jumpSpeed += GRAVITY; //速度 += 加速度
	transform_.position_.y += jumpSpeed; //座標 += 速度

	/*if (animType == 2)
	{
		if (jumpSpeed < 0) {
			animFrame = 1;
		}
		else {
			animFrame = 2;
		}
	}*/

	if (pField != nullptr) {
		int pushRfeet = pField->CollisionDown(transform_.position_.x + 50, transform_.position_.y + 64);
		int pushLfeet = pField->CollisionDown(transform_.position_.x + 14, transform_.position_.y + 64);
		int pushFeet = max(pushRfeet, pushLfeet);//2つの足元のめり込みの大きいほう
		if (pushFeet >= 1) {
			transform_.position_.y -= pushFeet - 1;
			jumpSpeed = 0.0f;
			onGround = true;
			animType = 0;
		}
		else {
			onGround = false;
		}
		int pushRhead = pField->CollisionUp(transform_.position_.x + 50, transform_.position_.y);
		int pushLhead = pField->CollisionUp(transform_.position_.x + 10, transform_.position_.y);
		int pushHead = max(pushRhead, pushLhead);//2つの頭のめり込みの大きいほう
		if (pushHead >= 1) {
			transform_.position_.y += pushHead - 1;
			jumpSpeed = 0.0f;
		}
	}
	if (transform_.position_.y >= GROUND){
		transform_.position_.y = GROUND;
		jumpSpeed = 0.0f;
		onGround = true;
		animType = 0;
	}

	//std::list<Bird*> pBirds = GetParent()->FindGameObjects<Bird>();
	//for (Bird* pBird : pBirds) {
	//	if (pBird->CollideCircle(transform_.position_.x + 32.0f, transform_.position_.y + 32.0f, 20.0f)) {
	//		animType = 4;
	//		animFrame = 0;
	//		state = S_Cry;
	//		scene->StartDead();
	//	}
	//}

	//ここでカメラ位置を調整
	//Camera* cam = GetParent()->FindGameObject<Camera>();
	//int x = (int)transform_.position_.x - cam->GetValue();
	//if (x > 400) {
	//	x = 400;
	//	cam->SetValue((int)transform_.position_.x - x);//カメラの値を出すには上の式を移項する
	//}
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	//Camera* cam = GetParent()->FindGameObject<Camera>();
	//if (cam != nullptr) {
	//	x -= cam->GetValue();//プレイヤーの位置からカメラ分引く
	//}
	//DrawRectGraph(x, y, animFrame * 64, animType * 64, 64, 64, hImage, TRUE);
	DrawRotaGraph(x+32, y+32, 1.0, 0, hImage, TRUE, !isRight);
	
	unsigned int Cr = GetColor(0, 0, 255);
	DrawCircle(10, 10, 10,Cr, isBallAlive);
}

void Player::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}