#include "Player.h"
#include <DxLib.h>
#include <assert.h>
#include"PlayScene.h"
#include"Ball.h"
#include"Enemy.h"
#include"Field.h"
#include"ItemBox.h"
#include"Camera.h"
#include"Engine/SceneManager.h"

namespace {
	const int IDLE_MAXFRAME{ 6 };
	const int WALK_MAXFRAME{ 6 };
	const int TOSS_MAXFRAME{ 4 };
	const int SPIKE_MAXFRAME{ 4 };
	const int DAMAGE_MAXFRAME{ 4 };
	const int DEAD_MAXFRAME{ 5 };

	const float MAX_MOVE_SPEED = 3.5f;
	const float GROUND = 600.0f;
	const float JUMP_HIGHT = 64.0f * 1.5f;//ジャンプの高さ
	const float GRAVITY = 8.0f / 60.0f;//重力加速度
	
	const float PLAYER_WIDTH = 128.0f;
	const float PLAYER_HEIGHT = 128.0f;
	const float CORRECT_WIDTH = 50.0f;
	const float CORRECT_HEIGHT = 3.0f;
	const float CORRECT_TOP = 55.0f;
	const float CORRECT_BOTTOM = 3.0f;
};

Player::Player(GameObject* parent) : GameObject(sceneTop)
{
	hAnimImage = LoadGraph("Assets/Player/Player.png");
	assert(hAnimImage > 0);
	hBallImg = LoadGraph("Assets/Ball.png");
	assert(hBallImg > 0);

	transform_.position_.x = 128.0f;
	transform_.position_.y = GROUND;

	hp = 3;
	moveSpeed = 0.0;
	jumpSpeed = 0.0f;
	onGround = true;
	isRight = true;
	canMove = true;

	pBall = nullptr;
	isBallAlive = false;
	canSpike = false;
	tossCount = 0;
	
	damageTimer = 0.0f;
	nowDamage = false;
	blink = true;
	fallNow = false;

	animFrame = 0;
	animType = 0;
	frameCounter = 0;

	cdTimer = 0.0f;
	
	state = NORMAL;
}

Player::~Player()
{
	if (hAnimImage > 0)
	{
		DeleteGraph(hAnimImage);
	}
	if (hBallImg > 0) {
		DeleteGraph(hBallImg);
	}
}

void Player::Update()
{
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();
	Camera* cam = GetParent()->FindGameObject<Camera>();


	if (state == DAMAGE) {
		cdTimer += 1.0f / 60.0f;
		if (animFrame < DAMAGE_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 5) {
				frameCounter = 0;
				animFrame++;
			}
		}
		else {
			animFrame = 0;
		}
		if (cdTimer >= 0.5f) {
			if (hp <= 0) {
				canMove = false;
				animType = 5;
				animFrame = 0;

				//scene->StartDead();
				state = DEAD;
			}
			else {
				cdTimer = 0.0f;
				canMove = true;
				animType = 0;
				animFrame = 0;
				frameCounter = 0;
				nowDamage = true;
				state = NORMAL;
			}
		}
	}

	if (!scene->CanMove()) {
		return;
	}

	if (nowDamage) {
		damageTimer -= 1.0f / 60.0f;
		if (damageTimer <= 0.0f) {
			damageTimer = 0.0f;
			nowDamage = false;
		}
		if (0 == (int)(damageTimer * 30.0f) % 2)
			blink = false;
		else
			blink = true;
	}
	else
		blink = true;


	if (state == DEAD) {
		if (animFrame < DEAD_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 20) {
				frameCounter = 0;
				animFrame++;
			}
		}
		else {
			cdTimer += 1.0f / 60.0f;
			if (cdTimer > 2.0f) {
				cdTimer = 0.0f;
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
			}
		}
	}

	if (state == TOSS) {
		if (animFrame < TOSS_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 10) {
				frameCounter = 0;
				animFrame++;
			}
			if (animFrame == 1) {
				if (tossCount == 1) {
					if (pBall != nullptr) {
						pBall->FirstToss();
					}
				}
			}
		}
		else {
			canMove = true;
			animType = 0;
			animFrame = 0;
			frameCounter = 0;
			state = NORMAL;
		}
	}

	float nextPos_x = transform_.position_.x;
	float nextPos_y = transform_.position_.y;

	if (state == SPIKE) {
		if (animFrame < SPIKE_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 10) {
				frameCounter = 0;
				animFrame++;
			}
			if (animFrame == 2 && frameCounter==0) {
				if (pBall != nullptr) {
					nextPos_x = pBall->GetPos().x;
					nextPos_y = pBall->GetPos().y + PLAYER_HEIGHT / 2.0f - CORRECT_TOP;
					pBall->Spike(isRight);
					//if (cam != nullptr)
						//cam->VibrationY(10.0f);
				}
			}
		}
		else {
			canMove = true;
			animType = 0;
			animFrame = 0;
			frameCounter = 0;
			state = NORMAL;
		}
	}

	float len = 0.0f;
	if (pBall != nullptr) {
		float lenX = pBall->GetPos().x - transform_.position_.x;
		float lenY = pBall->GetPos().y - transform_.position_.y;
		len = sqrt(lenX * lenX + lenY * lenY);
	}

	if (CheckHitKey(KEY_INPUT_K))
	{
		if (!prevAttackKey) {
			if (!isBallAlive || (pBall != nullptr && !pBall->IsBallAlive())) {
				pBall = nullptr;
				pBall = Instantiate<Ball>(GetParent());
				isBallAlive = true;
				pBall->SetPosition(transform_.position_.x, transform_.position_.y - PLAYER_HEIGHT / 2.0f + 30.0f);
				pBall->SetIsRight(isRight);
				//pBall->FirstToss();
				canMove = false;
				tossCount += 1;
				animType = 2;
				animFrame = 0;
				frameCounter = 0;
				state = TOSS;
			}
			else {
				if (tossCount > 0) {
					if (IsTouchBall(pBall->GetPos())) {
						if (tossCount == 1) {
							pBall->SetIsRight(isRight);
							pBall->SecondToss();
							canMove = false;
							tossCount += 1;
							animType = 2;
							animFrame = 0;
							state = TOSS;
						}
					}
					else {
						//プレイヤーとボールが一定距離離れていて、かつプレイヤーよりも一定の高さにボールがあるとき
						if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - PLAYER_HEIGHT) {
							if (!pBall->IsTouchGround()) {
								canMove = false;
								tossCount = 0;
								animType = 3;
								animFrame = 0;
								pBall->SetIsRight(isRight);
								state = SPIKE;
							}
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

	if (pBall != nullptr) {
		if (tossCount > 0) {
			if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - PLAYER_HEIGHT)
				if (!pBall->IsTouchGround())
					canSpike = true;
				else
					canSpike = false;
			else {
				canSpike = false;
			}
		}
		pBall->SetCharge(canSpike, tossCount);
		if (len > 600) {
			pBall->SetPosition(0.0f, 800.0f);
			isBallAlive = false;
			pBall = nullptr;
			tossCount = 0;
		}
	}

	if (pBall!=nullptr){ 
		if (!pBall->IsBallAlive() || pBall->IsBallCatch(transform_.position_.x - 10.0f, transform_.position_.y + PLAYER_HEIGHT / 4.0f)) {
			pBall->SetPosition(0.0f, 800.0f);
			isBallAlive = false;
			pBall = nullptr;
			tossCount = 0;
		}
	}
	
	
	if (canMove) {
		if (CheckHitKey(KEY_INPUT_D))
		{
			if (onGround)
				animType = 1;
			if (moveSpeed < 0)
				moveSpeed = 0.0f;
			moveSpeed += 0.1f;
			if (moveSpeed >= MAX_MOVE_SPEED)
				moveSpeed = MAX_MOVE_SPEED;
			nextPos_x += moveSpeed;
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
			float cy = PLAYER_HEIGHT / 2.0f;
			if (++frameCounter >= WALK_MAXFRAME) {
				animFrame = (animFrame + 1) % WALK_MAXFRAME;
				frameCounter = 0;
			}
			if (pField != nullptr) {
				float pushTright = pField->CollisionRight(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
				float pushBright = pField->CollisionRight(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
				float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
				if (pushRight > 0.0f) {
					nextPos_x -= pushRight - 1.0f;
				}
			}
			for (ItemBox* pIBox : pIBoxs) {
				if (pIBox != nullptr) {
					float pushTright = pIBox->CollisionRight(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
					float pushBright = pIBox->CollisionRight(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
					float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
					if (pushRight > 0.0f) {
						nextPos_x -= pushRight - 1.0f;
					}
				}
			}
			isRight = true;
		}
		else if (CheckHitKey(KEY_INPUT_A))
		{
			if (onGround)
				animType = 1;
			if (moveSpeed > 0)
				moveSpeed = 0.0f;
			moveSpeed -= 0.1f;
			if (moveSpeed <= -MAX_MOVE_SPEED)
				moveSpeed = -MAX_MOVE_SPEED;
			nextPos_x += moveSpeed;
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
			float cy = PLAYER_HEIGHT / 2.0f;
			if (++frameCounter >= WALK_MAXFRAME) {
				animFrame = (animFrame + 1) % WALK_MAXFRAME;
				frameCounter = 0;
			}
			if (pField != nullptr) {
				float pushTleft = pField->CollisionLeft(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
				float pushBleft = pField->CollisionLeft(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
				float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
				if (pushLeft > 0.0f) {
					nextPos_x += pushLeft - 1.0f;
				}
			}
			for (ItemBox* pIBox : pIBoxs) {
				if (pIBox != nullptr) {
					float pushTleft = pIBox->CollisionLeft(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
					float pushBleft = pIBox->CollisionLeft(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
					float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
					if (pushLeft > 0.0f) {
						nextPos_x += pushLeft - 1.0f;
					}
				}
			}
			isRight = false;
		}
		else {
			moveSpeed = 0.0f;
			if (onGround) {
				animType = 0;
				animFrame = 0;
				frameCounter = 0;
			}
		}

		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			if (prevSpaceKey == false) {
				if (onGround) {
					jumpSpeed = -sqrt(2 * GRAVITY * JUMP_HIGHT);
					onGround = false;
					animType = 3;
				}
			}
			prevSpaceKey = true;
		}
		else {
			prevSpaceKey = false;
		}

		if (!onGround) {
			if (state == NORMAL && animType == 3)
			{
				animFrame = 3;
			}
		}
	}
	
	if (state != DAMAGE) {
		jumpSpeed += GRAVITY; //速度 += 加速度
		nextPos_y += jumpSpeed;//座標 += 速度
	}

	if (pField != nullptr) {
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
		float cy = PLAYER_HEIGHT / 2.0f;

		float pushRbottom = pField->CollisionDown(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) + 1.0f);
		float pushLbottom = pField->CollisionDown(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
		float pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
		if (pushBottom > 0.0f) {
			nextPos_y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
			onGround = true;
		}
		else {
			onGround = false;
		}
		float pushRtop = pField->CollisionUp(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
		float pushLtop = pField->CollisionUp(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) - 1.0f);
		float pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
		if (pushTop > 0.0f) {
			nextPos_y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}

	for (ItemBox* pIBox : pIBoxs) {
		if (pIBox != nullptr) {
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
			float cy = PLAYER_HEIGHT / 2.0f;

			float pushRbottom = pIBox->CollisionDown(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) + 1.0f);
			float pushLbottom = pIBox->CollisionDown(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
			float pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
			if (pushBottom > 0.0f) {
				nextPos_y -= pushBottom - 1.0f;
				jumpSpeed = 0.0f;
				onGround = true;
			}
			float pushRtop = pIBox->CollisionUp(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
			float pushLtop = pIBox->CollisionUp(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) - 1.0f);
			float pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
			if (pushTop > 0.0f) {
				nextPos_y += pushTop;
				jumpSpeed = 0.0f;
			}
		}
	}

	transform_.position_ = { nextPos_x,nextPos_y,0.0f };

	if (transform_.position_.y > 720.0f) {
		state = DEAD;
		if (pBall != nullptr) {
			pBall->KillMe();
		}
		//scene->StartDead();
		//SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		//pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
	}
	
	if (damageTimer <= 0.0f) {
		if (state != DEAD && state != DAMAGE) {
			std::list<Enemy*> pEnemies = GetParent()->FindGameObjects<Enemy>();
			for (Enemy* pEnemy : pEnemies) {
				if (pEnemy->IsSteppedOnHead(transform_.position_.x, transform_.position_.y + 40.0f, PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0)) {
					jumpSpeed = -sqrt(2 * GRAVITY * JUMP_HIGHT);
					onGround = false;
					animType = 3;
					pEnemy->KillEnemy();
				}
				if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y + 20, PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0f)) {
					hp--;
					/*if (cam != nullptr)
						cam->VibrationX(1.0f);*/
					//scene->StartStop(0.5f);
					damageTimer = 2.0f;
					canMove = false;
					animType = 4;
					animFrame = 0;
					state = DAMAGE;
					
					if (pBall != nullptr)
						pBall->KillMe();
				}
			}
		}
	}

	float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
	float cy = PLAYER_HEIGHT / 2.0f;
	if (pField->IsFall(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f)) {
		fallNow = true;
	}
	if (fallNow) {
		cdTimer++;
		if (cdTimer > 10000) {
			cdTimer = 0.0f;
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
		}
	}

	//ここでカメラ位置を調整
	if (cam != nullptr) {
		
		if (cam->IsVibNow()) {
			int x = (int)transform_.position_.x - cam->GetValueX();
			//if (x > 1000) {
			//	x = 1000;
			//	cam->SetValueX((int)transform_.position_.x - x);//カメラの値を出すには上の式を移項する
			//}
			//else if (x < 280) {
			//	x = 280;
			//	cam->SetValueX((int)transform_.position_.x - x);
			//}
		}
		else {
			int x = (int)transform_.position_.x - cam->GetValueX();
			if (x > 960) {
				x = 960;
				cam->SetValueX((int)transform_.position_.x - x);//カメラの値を出すには上の式を移項する
			}
			else if (x < 320) {
				x = 320;
				cam->SetValueX((int)transform_.position_.x - x);
			}
			int y = (int)transform_.position_.y - cam->GetValueY();
			if (!fallNow) {
				if (y > 592) {
					y = 592;
					cam->SetValueY((int)transform_.position_.y - y);//カメラの値を出すには上の式を移項する
				}
				else if (y < 32) {
					y = 32;
					cam->SetValueY((int)transform_.position_.y - y);
				}
			}
		}
	}
	
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();//プレイヤーの位置からカメラ分引く
		y -= cam->GetValueY();
	}
	if(blink)
	DrawRectGraph(x - PLAYER_WIDTH/2.0, y-PLAYER_HEIGHT/2.0-CORRECT_BOTTOM, animFrame * PLAYER_WIDTH, animType * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, hAnimImage, TRUE,!isRight);
	
	if (!isBallAlive && state!=DEAD) {
		int crrX = 0;
		if (isRight)
			crrX = 15;
		else
			crrX = -15;
		DrawRotaGraph(x + crrX, y + 20, 0.9, 0, hBallImg, TRUE, !isRight);
	}
	//unsigned int Cr = GetColor(0, 0, 255);
	//DrawCircle(10, 10, 10,Cr, isBallAlive);
}

void Player::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

XMFLOAT3 Player::GetPosition()
{
	return transform_.position_;
}

bool Player::IsTouchBall(XMFLOAT3 pos)
{
	float cx = PLAYER_WIDTH / 2.0f;
	float cy = 50.0f;
	if ((pos.x <= transform_.position_.x + cx) && (pos.x >= transform_.position_.x - cx)) {
		if ((pos.y <= transform_.position_.y) && (pos.y >= transform_.position_.y - cy)) {
			return true;
		}
	}
	return false;
}
