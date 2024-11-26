#include "Player.h"
#include <DxLib.h>
#include <assert.h>
#include"../Scenes/PlayScene.h"
#include"../Objects/Ball.h"
#include"Enemy.h"
#include"../Objects/Field.h"
#include"../Objects/ItemBox.h"
#include"../Objects/Camera.h"
#include"../Objects/HPBar.h"
#include"../Engine/SceneManager.h"

namespace {
	const int IDLE_MAXFRAME{ 6 };
	const int WALK_MAXFRAME{ 6 };
	const int TOSS_MAXFRAME{ 4 };
	const int SPIKE_MAXFRAME{ 4 };
	const int DAMAGE_MAXFRAME{ 4 };
	const int DEAD_MAXFRAME{ 5 };

	const float MAX_MOVE_SPEED = 3.5f;
	const float GROUND = 600.0f;
	const float JUMP_HIGHT = 64.0f * 3.2f;//�W�����v�̍���
	const float GRAVITY = 8.0f / 60.0f;//�d�͉����x
	
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

	spikeSound = LoadSoundMem("Assets/Sounds/Explosion04-1(Short).mp3");
	assert(spikeSound > 0);
	tossSound = LoadSoundMem("Assets/Sounds/Onoma-Button-Click03-1(High).mp3");
	assert(tossSound > 0);
	catchSound = LoadSoundMem("Assets/Sounds/Motion-Grab02-1(Low).mp3");
	assert(catchSound > 0);
	walkSound = LoadSoundMem("Assets/Sounds/Retro_Anime-Switch04-2(High).mp3");
	assert(walkSound > 0);
	jumpSound = LoadSoundMem("Assets/Sounds/SNES-Action01-01(Jump).mp3");
	assert(jumpSound > 0);
	damageSound = LoadSoundMem("Assets/Sounds/GB-Action01-05(Damage).mp3");
	assert(damageSound > 0);
	deathSound = LoadSoundMem("Assets/Sounds/Retro_Anime-Robot03-3(Low-Long).mp3");
	assert(deathSound > 0);


	transform_.position_.x = 128.0f;
	transform_.position_.y = GROUND;

	hp = 5;
	moveSpeed = 0.0;
	jumpSpeed = 0.0f;
	onGround = true;
	isRight = true;
	canMove = true;
	canWalk = true;
	jumpTimer = 0.0f;

	pBall = nullptr;
	isBallAlive = false;
	canSpike = false;
	tossCount = 0;
	
	damageTimer = 0.0f;
	nowDamage = false;
	blink = true;
	fallNow = false;
	tossNow = false;

	animFrame = 0;
	animType = 0;
	frameCounter = 0;

	currentTime = 0.5f;
	moveTime = currentTime;

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
	if (spikeSound > 0) {
		DeleteSoundMem(spikeSound);
	}
	if (tossSound > 0) {
		DeleteSoundMem(tossSound);
	}
	if (catchSound > 0) {
		DeleteSoundMem(catchSound);
	}
	if (walkSound > 0) {
		DeleteSoundMem(walkSound);
	}
	if (jumpSound > 0) {
		DeleteSoundMem(jumpSound);
	}
	if (damageSound > 0) {
		DeleteSoundMem(damageSound);
	}
	if (deathSound > 0) {
		DeleteSoundMem(deathSound);
	}
}

void Player::Update()
{
	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();
	Camera* cam = GetParent()->FindGameObject<Camera>();
	HPBar* pHPBar = GetParent()->FindGameObject<HPBar>();


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
				PlaySoundMem(deathSound, DX_PLAYTYPE_BACK);
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
		int x = (int)transform_.position_.x - cam->GetValueX();
		if (x > 900) {
			x = 900;
			cam->SetValueX((int)transform_.position_.x - x);//�J�����̒l���o���ɂ͏�̎����ڍ�����
		}
		else if (x < 380) {
			x = 380;
			cam->SetValueX((int)transform_.position_.x - x);
		}
		int y = (int)transform_.position_.y - cam->GetValueY();
		if (!fallNow) {
			if (y > 550) {
				y = 550;
				cam->SetValueY((int)transform_.position_.y - y);//�J�����̒l���o���ɂ͏�̎����ڍ�����
			}
			else if (y < 32) {
				y = 32;
				cam->SetValueY((int)transform_.position_.y - y);
			}
		}
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
		canMove = false;
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
			if (animFrame == 1 && frameCounter == 0) {
				if (tossCount == 1) {
					if (pBall != nullptr) {
						PlaySoundMem(tossSound, DX_PLAYTYPE_BACK);
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
		tossNow = true;
#if 0
		target.x = pBall->GetPos().x;
		target.y = pBall->GetPos().y;
		if (animFrame < SPIKE_MAXFRAME - 1) {
			frameCounter++;
			if (animFrame == 1 || animFrame == 2) {
				if (frameCounter >= 20) {
					frameCounter = 0;
					animFrame++;
				}
			}
			else {
				if (frameCounter >= 10) {
					frameCounter = 0;
					animFrame++;
				}
			}
			if (animFrame == 1) {
				if (currentTime >= moveTime)
					return;
				currentTime += 1.0f / 60.0f;
				if (currentTime > moveTime)
					currentTime = moveTime;
				float rate = currentTime / moveTime;
				rate = pow(rate, 0.1f);
				nextPos_x = (target.x - firstPos.x) * rate + firstPos.x;
				nextPos_y = (target.y - firstPos.y) * rate + firstPos.y;
			}
			if (animFrame == 2 && frameCounter==0) {
				if (pBall != nullptr) {
					nextPos_x = pBall->GetPos().x;
					nextPos_y = pBall->GetPos().y + PLAYER_HEIGHT / 2.0f - CORRECT_TOP;
					PlaySoundMem(spikeSound, DX_PLAYTYPE_BACK);
					pBall->Spike(isRight);
					//if (cam != nullptr)
						//cam->VibrationY(10.0f);
				}
			}
		}
		else {
			canMove = true;
			animType = 3;
			animFrame = 3;
			frameCounter = 0;
			state = NORMAL;
		}
#endif
		spikeAfter = true;
		if (pBall != nullptr) {
			PlaySoundMem(spikeSound, DX_PLAYTYPE_BACK);
			pBall->Spike(isRight, prevMoveKey);
		}
		state = NORMAL;
	}

	float len = 0.0f;
	float lenX = 0.0f;
	float lenY = 0.0f;
	if (pBall != nullptr) {
		lenX = pBall->GetPos().x - transform_.position_.x;
		lenY = pBall->GetPos().y - transform_.position_.y;
		len = sqrt(lenX * lenX + lenY * lenY);
	}

	if (CheckHitKey(KEY_INPUT_J))
	{
		if (!prevTossKey) {
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
						if (tossCount == 1&&state!=TOSS) {
							pBall->SetIsRight(isRight);
							PlaySoundMem(tossSound, DX_PLAYTYPE_BACK);
							pBall->SecondToss();
							canMove = false;
							tossCount += 1;
							animType = 2;
							animFrame = 0;
							state = TOSS;
						}
					}
					else {
#if 0
						//�v���C���[�ƃ{�[������苗������Ă��āA���v���C���[�������̍����Ƀ{�[��������Ƃ�
						if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - PLAYER_HEIGHT) {
							if (!pBall->IsTouchGround()) {
								if (sqrt(lenX*lenX) < 64.0f) {
									firstPos.x = transform_.position_.x;
									firstPos.y = transform_.position_.y;
									moveTime = 0.5f;
									currentTime = 0.0f;

									canMove = false;
									tossCount = 0;
									animType = 3;
									animFrame = 0;
									pBall->SetIsRight(isRight);
									state = SPIKE;
								}
							}
						}
#endif
					}
				}
			}
			prevTossKey = true;
		}
	}
	else {
		prevTossKey = false;
	}

	if (CheckHitKey(KEY_INPUT_K)) {
		if (!prevSpikeKey) {
			if (pBall != nullptr) {
				if (state == NORMAL && tossCount > 0) {
					if (IsTouchBall(pBall->GetPos()) && !pBall->IsTouchGround()) {
						if (!onGround/* && prevMoveKey*/) {
							//canMove = false;
							tossCount = 0;
							animType = 3;
							//animFrame = 0;
							pBall->SetIsRight(isRight);
							state = SPIKE;
						}
					}
				}
			}
			prevSpikeKey = true;
		}
	}
	else {
		prevSpikeKey = false;
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

	if (pBall!=nullptr && !tossNow){ 
		if (!pBall->IsBallAlive() || pBall->IsBallCatch(transform_.position_.x, transform_.position_.y + PLAYER_HEIGHT / 4.0f)) {
			if (state == NORMAL) {
				PlaySoundMem(catchSound, DX_PLAYTYPE_BACK);
				pBall->SetPosition(0.0f, 800.0f);
				isBallAlive = false;
				pBall = nullptr;
				tossCount = 0;
			}
		}
	}
	
	
	if (canMove) {
		if (canWalk) {
			if (CheckHitKey(KEY_INPUT_D))
			{
				prevMoveKey = true;
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
					if (animFrame % 2 == 1 && onGround) {
						ChangeVolumeSoundMem(255 * 80 / 100, walkSound);
						PlaySoundMem(walkSound, DX_PLAYTYPE_BACK);
					}
					animFrame = (animFrame + 1) % WALK_MAXFRAME;
					frameCounter = 0;
				}
				if (pField != nullptr) {
					float pushTright = pField->CollisionRight(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
					float pushBright = pField->CollisionRight(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
					float pushRight = max(pushBright, pushTright);//�E���̓��Ƒ����œ����蔻��
					if (pushRight > 0.0f) {
						nextPos_x -= pushRight - 1.0f;
					}
				}
				for (ItemBox* pIBox : pIBoxs) {
					if (pIBox != nullptr) {
						float pushTright = pIBox->CollisionRight(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
						float pushBright = pIBox->CollisionRight(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
						float pushRight = max(pushBright, pushTright);//�E���̓��Ƒ����œ����蔻��
						if (pushRight > 0.0f) {
							nextPos_x -= pushRight - 1.0f;
						}
					}
				}
				isRight = true;
			}
			else if (CheckHitKey(KEY_INPUT_A))
			{
				prevMoveKey = true;
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
					if (animFrame % 2 == 1 && onGround) {
						ChangeVolumeSoundMem(255 * 80 / 100, walkSound);
						PlaySoundMem(walkSound, DX_PLAYTYPE_BACK);
					}
					animFrame = (animFrame + 1) % WALK_MAXFRAME;
					frameCounter = 0;
				}
				if (pField != nullptr) {
					float pushTleft = pField->CollisionLeft(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
					float pushBleft = pField->CollisionLeft(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
					float pushLeft = max(pushBleft, pushTleft);//�����̓��Ƒ����œ����蔻��
					if (pushLeft > 0.0f) {
						nextPos_x += pushLeft - 1.0f;
					}
				}
				for (ItemBox* pIBox : pIBoxs) {
					if (pIBox != nullptr) {
						float pushTleft = pIBox->CollisionLeft(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
						float pushBleft = pIBox->CollisionLeft(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
						float pushLeft = max(pushBleft, pushTleft);//�����̓��Ƒ����œ����蔻��
						if (pushLeft > 0.0f) {
							nextPos_x += pushLeft - 1.0f;
						}
					}
				}
				isRight = false;
			}
			else {
				moveSpeed = 0.0f;
				prevMoveKey = false;
				if (onGround) {
					animType = 0;
					animFrame = 0;
					frameCounter = 0;
				}
			}
		}

		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			if (prevSpaceKey == false) {
				if (onGround) {
					canWalk = false;
					jumpTimer += 1.0f / 60.0f;
					animFrame = 0;
					animType = 3;
					/*PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
					jumpSpeed = -sqrt(2 * GRAVITY * JUMP_HIGHT);
					onGround = false;
					animType = 3;*/
				}
			}
			//prevSpaceKey = true;
		}
		else {
			if (jumpTimer > 0.0f && jumpTimer<=0.2f) {
				prevSpaceKey = true;
				PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
				jumpSpeed = -sqrt(2 * GRAVITY * (JUMP_HIGHT / 2.0f));
				onGround = false;
			}
			else if (jumpTimer > 0.2f/* && jumpTimer <= 1.0f*/) {
				prevSpaceKey = true;
				PlaySoundMem(jumpSound, DX_PLAYTYPE_BACK);
				jumpSpeed = -sqrt(2 * GRAVITY * JUMP_HIGHT);
				onGround = false;
			}
			//prevSpaceKey = false;
			jumpTimer = 0.0f;
			canWalk = true;
		}

		if (!onGround) {
			/*if (state == NORMAL && animType == 3)
			{
				animFrame = 3;
			}*/
			if (state == NORMAL) {
				if (!spikeAfter) {
					if (jumpSpeed <= 0) {
						if (tossCount == 0) {
							animFrame = 3;
						}
						else if (tossCount > 0) {
							animFrame = 1;
						}
					}
					else
						animFrame = 3;
				}
				else
					animFrame = 2;
			}
		}
	}
	
	if (state != DAMAGE/* && state != SPIKE*/) {
		jumpSpeed += GRAVITY; //���x += �����x
		nextPos_y += jumpSpeed;//���W += ���x
	}

	if (pField != nullptr) {
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
		float cy = PLAYER_HEIGHT / 2.0f;

		float pushRbottom = pField->CollisionDown(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) + 1.0f);
		float pushLbottom = pField->CollisionDown(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
		float pushBottom = max(pushRbottom, pushLbottom);//2�̑����̂߂荞�݂̑傫���ق�
		if (pushBottom > 0.0f) {
			nextPos_y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
			prevSpaceKey = false;
			spikeAfter = false;
			onGround = true;
			tossNow = false;
		}
		else {
			onGround = false;
		}
		float pushRtop = pField->CollisionUp(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
		float pushLtop = pField->CollisionUp(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) - 1.0f);
		float pushTop = max(pushRtop, pushLtop);//2�̓��̂߂荞�݂̑傫���ق�
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
			float pushBottom = max(pushRbottom, pushLbottom);//2�̑����̂߂荞�݂̑傫���ق�
			if (pushBottom > 0.0f) {
				nextPos_y -= pushBottom - 1.0f;
				jumpSpeed = 0.0f;
				prevSpaceKey = false;
				spikeAfter = false;
				onGround = true;
				tossNow = false;
			}
			float pushRtop = pIBox->CollisionUp(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
			float pushLtop = pIBox->CollisionUp(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) - 1.0f);
			float pushTop = max(pushRtop, pushLtop);//2�̓��̂߂荞�݂̑傫���ق�
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
#if 0
				if (pEnemy->IsSteppedOnHead(transform_.position_.x, transform_.position_.y + 40.0f, PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0)) {
					jumpSpeed = -sqrt(2 * GRAVITY * JUMP_HIGHT);
					onGround = false;
					animType = 3;
					pEnemy->KillEnemy();
				}
#endif
#if 1
				if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y + 20, PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0f)) {
					hp--;
					/*if (cam != nullptr)
						cam->VibrationX(1.0f);*/
					//scene->StartStop(0.5f);
					PlaySoundMem(damageSound, DX_PLAYTYPE_BACK);
					damageTimer = 2.0f;
					canMove = false;
					animType = 4;
					animFrame = 0;
					state = DAMAGE;
					
					if (pBall != nullptr)
						pBall->KillMe();
				}
#endif
			}
		}
	}

	float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
	float cy = PLAYER_HEIGHT / 2.0f;
	if (pField->IsFall(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f)) {
		fallNow = true;
		state = DEAD;
	}
	/*if (fallNow) {
		cdTimer++;
		if (cdTimer > 10000) {
			cdTimer = 0.0f;
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
		}
	}*/

	if (pHPBar != nullptr)
		pHPBar->SetHP(hp);

	//�����ŃJ�����ʒu�𒲐�
	if (cam != nullptr) {
		
		if (cam->IsVibNow()) {
			int x = (int)transform_.position_.x - cam->GetValueX();
			//if (x > 1000) {
			//	x = 1000;
			//	cam->SetValueX((int)transform_.position_.x - x);//�J�����̒l���o���ɂ͏�̎����ڍ�����
			//}
			//else if (x < 280) {
			//	x = 280;
			//	cam->SetValueX((int)transform_.position_.x - x);
			//}
		}
		else {
			int x = (int)transform_.position_.x - cam->GetValueX();
			if (x > 900) {
				x = 900;
				cam->SetValueX((int)transform_.position_.x - x);//�J�����̒l���o���ɂ͏�̎����ڍ�����
			}
			else if (x < 380) {
				x = 380;
				cam->SetValueX((int)transform_.position_.x - x);
			}
			int y = (int)transform_.position_.y - cam->GetValueY();
			if (state!=DEAD) {
				if (y > 550) {
					y = 550;
					cam->SetValueY((int)transform_.position_.y - y);//�J�����̒l���o���ɂ͏�̎����ڍ�����
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
		x -= cam->GetValueX();//�v���C���[�̈ʒu����J����������
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
	float cy = 60.0f;
	if ((pos.x <= transform_.position_.x + cx) && (pos.x >= transform_.position_.x - cx)) {
		if ((pos.y <= transform_.position_.y) && (pos.y >= transform_.position_.y - cy)) {
			return true;
		}
	}
	return false;
}
