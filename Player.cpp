#include "Player.h"
#include <DxLib.h>
#include <assert.h>
//#include"Camera.h"
#include"Field.h"
#include"TestScene.h"
#include"Ball.h"
#include"Enemy.h"

namespace {
	const float MOVE_SPEED = 2.5f;
	const float GROUND = 600.0f;
	const float JUMP_HIGHT = 64.0f * 4.0f;//ジャンプの高さ
	const float GRAVITY = 9.8f / 60.0f;//重力加速度
	const float PLAYER_WIDTH = 64.0f;
	const float PLAYER_HEIGHT = 64.0f;
	const float CORRECT_WIDTH = 16.0f;
	const float CORRECT_HEIGHT = 3.0f;
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
	tossCount = 0;
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
		if (!prevAttackKey){
			if (!isBallAlive ||(pBall != nullptr && !pBall->IsBallAlive())) {
				pBall = nullptr;
				pBall = Instantiate<Ball>(GetParent());
				isBallAlive = true;
				XMFLOAT3 ballPos = { transform_.position_.x,transform_.position_.y - PLAYER_HEIGHT / 2.0f,transform_.position_.z };
				pBall->SetPosition(ballPos);
				pBall->FirstToss();
				tossCount += 1;
			}
			else {
				if (tossCount > 0){
					if (IsTouchBall(pBall->GetPos())){
						if (tossCount == 1) {
							pBall->SecondToss();
								tossCount += 1;
						}
					}
					else{
						float lenX = pBall->GetPos().x - transform_.position_.x;
						float lenY = pBall->GetPos().y - transform_.position_.y;
						float len = sqrt(lenX * lenX + lenY * lenY);
						if (len > PLAYER_HEIGHT * 2.0f && pBall->GetPos().y <= transform_.position_.y - PLAYER_HEIGHT * 2.0f) {
							SetPosition(pBall->GetPos().x, pBall->GetPos().y + PLAYER_HEIGHT/2.0f);
							pBall->Spike(isRight);
							tossCount = 0;
						}
					}
				}
				
			}
			prevAttackKey = true;
		}
	}
	else {
		prevAttackKey = false;
	}

	if (pBall!=nullptr){ 
		if (!pBall->IsBallAlive() || pBall->IsBallCatch(transform_.position_.x, transform_.position_.y + PLAYER_HEIGHT / 4.0f)) {
			isBallAlive = false;
			pBall = nullptr;
			tossCount = 0;
		}
	}
	
	if (CheckHitKey(KEY_INPUT_D))
	{
		transform_.position_.x += MOVE_SPEED;
		/*if (++frameCounter >= 8) {
			animFrame = (animFrame + 1) % 4;
			frameCounter = 0;
		}*/
		if (pField != nullptr) {
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
			float cy = PLAYER_HEIGHT / 2.0f - CORRECT_HEIGHT-1.0f;
			float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy);
			float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy);
			float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
			if (pushRight > 0.0f) {
				transform_.position_.x -= pushRight - 1.0f;
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
		if (pField != nullptr) {
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
			float cy = PLAYER_HEIGHT / 2.0f - CORRECT_HEIGHT-1.0f;
			float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy);
			float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy);
			float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
			if (pushLeft > 0.0f) {
				transform_.position_.x += pushLeft - 1.0f;
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
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH-1.0f;
		float cy = PLAYER_HEIGHT / 2.0f - CORRECT_HEIGHT;
		float pushRbottom = pField->CollisionDown(transform_.position_.x + cx, transform_.position_.y + cy);
		float pushLbottom = pField->CollisionDown(transform_.position_.x - cx, transform_.position_.y + cy);
		float pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
			onGround = true;
			animType = 0;
		}
		else {
			onGround = false;
		}
		float pushRtop = pField->CollisionUp(transform_.position_.x + cx, transform_.position_.y - cy);
		float pushLtop = pField->CollisionUp(transform_.position_.x - cx, transform_.position_.y - cy);
		float pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}
	/*if (transform_.position_.y >= GROUND){
		transform_.position_.y = GROUND;
		jumpSpeed = 0.0f;
		onGround = true;
		animType = 0;
	}*/

	std::list<Enemy*> pEnemies = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y, PLAYER_WIDTH,PLAYER_HEIGHT)) {
			/*animType = 4;
			animFrame = 0;
			state = S_Cry;
			scene->StartDead();*/
			KillMe();
		}
	}

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
	DrawRotaGraph(x, y, 1.0, 0, hImage, TRUE, !isRight);
	
	unsigned int Cr = GetColor(0, 0, 255);
	DrawCircle(10, 10, 10,Cr, isBallAlive);
}

void Player::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Player::IsTouchBall(XMFLOAT3 pos)
{
	float cx = PLAYER_WIDTH / 2.0f;
	float cy = PLAYER_HEIGHT;
	if ((pos.x <= transform_.position_.x + cx) && (pos.x >= transform_.position_.x - cx)) {
		if ((pos.y <= transform_.position_.y) && (pos.y >= transform_.position_.y - cy)) {
			return true;
		}
	}
	return false;
}
