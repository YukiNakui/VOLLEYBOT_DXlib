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
	const int DEAD_MAXFRAME{ 5 };
	const float MOVE_SPEED = 2.5f;
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
	/*LoadDivGraph("Assets/Player/PlayerWalk.png", PAF::WALK_MAXFRAME, PAF::WALK_MAXFRAME, 1, 128, 128, hWalkImage);
	for (int i = 0; i < PAF::WALK_MAXFRAME; i++) {
		assert(hWalkImage[i] > 0);
	}*/
	hAnimImage = LoadGraph("Assets/Player/Player.png");
	assert(hAnimImage > 0);

	transform_.position_.x = 128.0f;
	transform_.position_.y = GROUND;
	jumpSpeed = 0.0f;
	onGround = true;
	isRight = true;

	pBall = nullptr;
	isBallAlive = false;
	tossCount = 0;

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
	/*for (int i = 0; i < PAF::WALK_MAXFRAME; i++) {
		if (hWalkImage[i] > 0)
		{
			DeleteGraph(hWalkImage[i]);
		}
	}*/
	if (pBall != nullptr) {
		delete pBall;
	}
}

void Player::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();
	Camera* cam = GetParent()->FindGameObject<Camera>();


	/*switch (state) {
	case NORMAL: UpdateNormal(); break;
	case TOSS: UpdateToss(); break;
	case SPIKE: UpdateSpike(); break;
	case DAMAGE: UpdateDamage(); break;
	case DEAD: UpdateDead(); break;
	}*/

	if (state == DEAD) {
		if (animFrame < DEAD_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 16) {
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
		return;
	}
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->CanMove()) {
		return;
	}

	
	if (CheckHitKey(KEY_INPUT_K))
	{
		if (!prevAttackKey){
			if (!isBallAlive ||(pBall != nullptr && !pBall->IsBallAlive())) {
				pBall = nullptr;
				pBall = Instantiate<Ball>(GetParent());
				isBallAlive = true;
				pBall->SetPosition(transform_.position_.x, transform_.position_.y - PLAYER_HEIGHT / 2.0f + CORRECT_TOP);
				pBall->FirstToss();
				tossCount += 1;
				state = TOSS;
			}
			else {
				if (tossCount > 0){
					if (IsTouchBall(pBall->GetPos())){
						if (tossCount == 1) {
							pBall->SecondToss();
								tossCount += 1;
								state = TOSS;
						}
					}
					else{
						float lenX = pBall->GetPos().x - transform_.position_.x;
						float lenY = pBall->GetPos().y - transform_.position_.y;
						float len = sqrt(lenX * lenX + lenY * lenY);
						//プレイヤーとボールが一定距離離れていて、かつプレイヤーよりも一定の高さにボールがあるとき
						if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - 64.0f) {
							SetPosition(pBall->GetPos().x, pBall->GetPos().y + PLAYER_HEIGHT/2.0f - CORRECT_TOP);
							pBall->Spike(isRight);
							tossCount = 0;
							cam->VibrationY(10.0f);
							state = SPIKE;
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
		float lenX = pBall->GetPos().x - transform_.position_.x;
		float lenY = pBall->GetPos().y - transform_.position_.y;
		float len = sqrt(lenX * lenX + lenY * lenY);
		if (len > 600) {
			pBall->SetPosition(0.0f, 800.0f);
			isBallAlive = false;
			pBall = nullptr;
			tossCount = 0;
		}
	}

	if (pBall!=nullptr){ 
		if (!pBall->IsBallAlive() || pBall->IsBallCatch(transform_.position_.x, transform_.position_.y + PLAYER_HEIGHT / 4.0f)) {
			pBall->SetPosition(0.0f, 800.0f);
			isBallAlive = false;
			pBall = nullptr;
			tossCount = 0;
		}
	}

	
		if (CheckHitKey(KEY_INPUT_D))
		{
			if (onGround)
				animType = 1;
			transform_.position_.x += MOVE_SPEED;
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
			float cy = PLAYER_HEIGHT / 2.0f;
			if (++frameCounter >= WALK_MAXFRAME) {
				animFrame = (animFrame + 1) % WALK_MAXFRAME;
				frameCounter = 0;
			}
			if (pField != nullptr) {
				float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
				float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
				float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
				if (pushRight > 0.0f) {
					transform_.position_.x -= pushRight - 1.0f;
				}
			}
			for (ItemBox* pIBox : pIBoxs) {
				if (pIBox != nullptr) {
					float pushTright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
					float pushBright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
					float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
					if (pushRight > 0.0f) {
						transform_.position_.x -= pushRight - 1.0f;
					}
				}
			}
			isRight = true;
		}
		else if (CheckHitKey(KEY_INPUT_A))
		{
			if (onGround)
				animType = 1;
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
			float cy = PLAYER_HEIGHT / 2.0f;
			transform_.position_.x -= MOVE_SPEED;
			if (++frameCounter >= WALK_MAXFRAME) {
				animFrame = (animFrame + 1) % WALK_MAXFRAME;
				frameCounter = 0;
			}
			if (pField != nullptr) {
				float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
				float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
				float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
				if (pushLeft > 0.0f) {
					transform_.position_.x += pushLeft - 1.0f;
				}
			}
			for (ItemBox* pIBox : pIBoxs) {
				if (pIBox != nullptr) {
					float pushTleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
					float pushBleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
					float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
					if (pushLeft > 0.0f) {
						transform_.position_.x += pushLeft - 1.0f;
					}
				}
			}
			isRight = false;
		}
		else {
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
	

	jumpSpeed += GRAVITY; //速度 += 加速度
	transform_.position_.y += jumpSpeed; //座標 += 速度

	if (animType == 3)
	{
		if (jumpSpeed == 0) {
			animFrame = 0;
		}
		else {
			animFrame = 3;
		}
	}

	if (pField != nullptr) {
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
		float cy = PLAYER_HEIGHT / 2.0f;

		float pushRbottom = pField->CollisionDown(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM));
		float pushLbottom = pField->CollisionDown(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM));
		float pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
			onGround = true;
			//animType = 0;
		}
		else {
			onGround = false;
		}
		float pushRtop = pField->CollisionUp(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP));
		float pushLtop = pField->CollisionUp(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP));
		float pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}

	for (ItemBox* pIBox : pIBoxs) {
		if (pIBox != nullptr) {
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
			float cy = PLAYER_HEIGHT / 2.0f;

			float pushRbottom = pIBox->CollisionDown(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM));
			float pushLbottom = pIBox->CollisionDown(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM));
			float pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
			if (pushBottom > 0.0f) {
				transform_.position_.y -= pushBottom - 1.0f;
				jumpSpeed = 0.0f;
				onGround = true;
				//animType = 0;
			}
			float pushRtop = pIBox->CollisionUp(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP));
			float pushLtop = pIBox->CollisionUp(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP));
			float pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
			if (pushTop > 0.0f) {
				transform_.position_.y += pushTop;
				jumpSpeed = 0.0f;
			}
		}
	}

	if (transform_.position_.y > 720.0f) {
		if (pBall != nullptr) {
			pBall->KillMe();
		}
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
	}
	

	std::list<Enemy*> pEnemies = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y, PLAYER_WIDTH/2.0f,PLAYER_HEIGHT/2.0f)) {
			animType = 5;
			animFrame = 0;
			state = DEAD;
			scene->StartDead();
			if (pBall != nullptr)
				pBall->KillMe();
			/*SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);*/
		}
	}

	//ここでカメラ位置を調整
	if (cam != nullptr) {
		int x = (int)transform_.position_.x - cam->GetValueX();
		if (x > 960) {
			x = 960;
			cam->SetValueX((int)transform_.position_.x - x);//カメラの値を出すには上の式を移項する
		}
		else if(x<320){
			x = 320;
			cam->SetValueX((int)transform_.position_.x - x);
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
	DrawRectGraph(x - PLAYER_WIDTH/2.0, y-PLAYER_HEIGHT/2.0-CORRECT_BOTTOM, animFrame * PLAYER_WIDTH, animType * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, hAnimImage, TRUE,!isRight);
	//DrawRotaGraph(x, y, 1.0, 0, hWalkImage[animFrame], TRUE, !isRight);
	
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
	float cy = PLAYER_HEIGHT / 2.0f;
	if ((pos.x <= transform_.position_.x + cx) && (pos.x >= transform_.position_.x - cx)) {
		if ((pos.y <= transform_.position_.y) && (pos.y >= transform_.position_.y - cy)) {
			return true;
		}
	}
	return false;
}

//
//void Player::UpdateNormal()
//{
//	//if (CheckHitKey(KEY_INPUT_D))
//	//{
//	//	if (onGround)
//	//		animType = 1;
//	//	transform_.position_.x += MOVE_SPEED;
//	////	float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
//	////	float cy = PLAYER_HEIGHT / 2.0f;
//	//	if (++frameCounter >= WALK_MAXFRAME) {
//	//		animFrame = (animFrame + 1) % WALK_MAXFRAME;
//	//		frameCounter = 0;
//	//	}
//	//	//if (pField != nullptr) {
//	//	//	float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
//	//	//	float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM) -1.0f);
//	//	//	float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
//	//	//	if (pushRight > 0.0f) {
//	//	//		transform_.position_.x -= pushRight - 1.0f;
//	//	//	}
//	//	//}
//	//	//for (ItemBox* pIBox : pIBoxs) {
//	//	//	if (pIBox != nullptr) {
//	//	//		float pushTright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
//	//	//		float pushBright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
//	//	//		float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
//	//	//		if (pushRight > 0.0f) {
//	//	//			transform_.position_.x -= pushRight - 1.0f;
//	//	//		}
//	//	//	}
//	//	//}
//	//	isRight = true;
//	//}
//	//else if (CheckHitKey(KEY_INPUT_A))
//	//{
//	//	if (onGround)
//	//		animType = 1;
//	////	float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
//	////	float cy = PLAYER_HEIGHT / 2.0f;
//	//	transform_.position_.x -= MOVE_SPEED;
//	//	if (++frameCounter >= WALK_MAXFRAME) {
//	//		animFrame = (animFrame + 1) % WALK_MAXFRAME;
//	//		frameCounter = 0;
//	//	}
//	//	//if (pField != nullptr) {
//	//	//	float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
//	//	//	float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
//	//	//	float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
//	//	//	if (pushLeft > 0.0f) {
//	//	//		transform_.position_.x += pushLeft - 1.0f;
//	//	//	}
//	//	//}
//	//	//for (ItemBox* pIBox : pIBoxs) {
//	//	//	if (pIBox != nullptr) {
//	//	//		float pushTleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
//	//	//		float pushBleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
//	//	//		float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
//	//	//		if (pushLeft > 0.0f) {
//	//	//			transform_.position_.x += pushLeft - 1.0f;
//	//	//		}
//	//	//	}
//	//	//}
//	//	isRight = false;
//	//}
//	//else {
//	//	if (onGround) {
//	//		animType = 0;
//	//		animFrame = 0;
//	//		frameCounter = 0;
//	//	}
//	//}
//}
//
//void Player::UpdateToss()
//{
//	//if (CheckHitKey(KEY_INPUT_K))
//	//{
//	//	if (!prevAttackKey){
//	//		if (!isBallAlive ||(pBall != nullptr && !pBall->IsBallAlive())) {
//	//			pBall = nullptr;
//	//			pBall = Instantiate<Ball>(GetParent());
//	//			isBallAlive = true;
//	//			pBall->SetPosition(transform_.position_.x, transform_.position_.y - PLAYER_HEIGHT / 2.0f + CORRECT_TOP);
//	//			pBall->FirstToss();
//	//			tossCount += 1;
//	//		}
//	//		else {
//	//			if (tossCount > 0){
//	//				if (IsTouchBall(pBall->GetPos())){
//	//					if (tossCount == 1) {
//	//						pBall->SecondToss();
//	//							tossCount += 1;
//	//					}
//	//				}
//	//				else{
//	//					//float lenX = pBall->GetPos().x - transform_.position_.x;
//	//					//float lenY = pBall->GetPos().y - transform_.position_.y;
//	//					//float len = sqrt(lenX * lenX + lenY * lenY);
//	//					////プレイヤーとボールが一定距離離れていて、かつプレイヤーよりも一定の高さにボールがあるとき
//	//					//if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - 64.0f) {
//	//					//	SetPosition(pBall->GetPos().x, pBall->GetPos().y + PLAYER_HEIGHT/2.0f - CORRECT_TOP);
//	//					//	pBall->Spike(isRight);
//	//					//	tossCount = 0;
//	//					//	cam->VibrationY(10.0f);
//	//					//}
//	//				}
//	//			}
//	//			
//	//		}
//	//		prevAttackKey = true;
//	//	}
//	//}
//	//else {
//	//	prevAttackKey = false;
//	//}
//}
//
//void Player::UpdateSpike()
//{
//	//float lenX = pBall->GetPos().x - transform_.position_.x;
//	//float lenY = pBall->GetPos().y - transform_.position_.y;
//	//float len = sqrt(lenX * lenX + lenY * lenY);
//	////プレイヤーとボールが一定距離離れていて、かつプレイヤーよりも一定の高さにボールがあるとき
//	//if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - 64.0f) {
//	//	SetPosition(pBall->GetPos().x, pBall->GetPos().y + PLAYER_HEIGHT/2.0f - CORRECT_TOP);
//	//	pBall->Spike(isRight);
//	//	tossCount = 0;
//	//	pCam->VibrationY(10.0f);
//	//}
//	//state = NORMAL;
//}
//
//void Player::UpdateDamage()
//{
//}
//
//void Player::UpdateDead()
//{
//	//if (state == DEAD) {
//	//	animType = 5;
//	//	frameCounter++;
//	//	if (frameCounter >= 16)
//	//	{
//	//		frameCounter = 0;
//	//		animFrame = (animFrame + 1) % 2;
//	//	}
//	//	//return;
//	//}
//}
