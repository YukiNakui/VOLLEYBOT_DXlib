#include "Skeleton.h"
#include"../Objects/Field.h"
#include"../Objects/Camera.h"
#include"../Objects/ItemBox.h"
#include"Player.h"
#include"Arrow.h"
#include"../Scenes/PlayScene.h"

namespace {
	const int SHOT_MAXFRAME{ 15 };
	const int DEAD_MAXFRAME{ 5 };

	const float SKELETON_WIDTH = 128.0f;
	const float SKELETON_HEIGHT = 128.0f;

	const float MOVE_SPEED = 1.5f;
	const float GRAVITY = 9.8f / 60.0f;//重力加速度

	const float CORRECT_WIDTH = 35.0f;
	const float CORRECT_BOTTOM = 1.0f;
	const float CORRECT_TOP = 35.0f;
	const float SHOT_HEIGHT = 17.0f;
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	const float COLLIDE_WIDTH = 60.0f;
	const float COLLIDE_HEIGHT = 60.0f;
}

Skeleton::Skeleton(GameObject* parent):Enemy(parent)
{
	hAnimImg = LoadGraph("Assets/Enemy/Skeleton.png");
	assert(hAnimImg > 0);
	shotSound = LoadSoundMem("Assets/Sounds/Onoma-Surprise08-3(High).mp3");
	assert(shotSound > 0);
	deathSound = LoadSoundMem("Assets/Sounds/Monster01-8.mp3");
	assert(deathSound > 0);

	animType = 0;
	animFrame = 0;
	frameCounter = 0;

	isRight = false;
	onGround = true;
	jumpSpeed = 0.0f;
	cdTimer = 0.0f;
	afterShot = false;

	state = NORMAL;
}

Skeleton::~Skeleton()
{
	if (hAnimImg > 0) {
		DeleteGraph(hAnimImg);
	}
	if (shotSound > 0) {
		DeleteSoundMem(shotSound);
	}
	if (deathSound > 0) {
		DeleteSoundMem(deathSound);
	}
}

void Skeleton::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();
	Player* pPlayer = GetParent()->FindGameObject<Player>();

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->CanMove())
		return;

	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	if (x > WINDOW_WIDTH + SKELETON_WIDTH)//画面外に敵がいたら動かさない
		return;
	else if (x < -SKELETON_WIDTH) {
		return;
	}
	if (y < 0 || y > WINDOW_HEIGHT)
		KillMe();

	if (state == DEAD) {
		if (animFrame < DEAD_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter > 10) {
				animFrame = (animFrame + 1) % DEAD_MAXFRAME;
				frameCounter = 0;
			}
		}
		else
			cdTimer++;
		if (cdTimer > 100.0f)
			KillMe();
	}

	if (state == NORMAL) {
		VECTOR targetVec = { pPlayer->GetPosition().x - transform_.position_.x,pPlayer->GetPosition().y - transform_.position_.y,0.0f };//相手へのベクトル
		targetVec = VNorm(targetVec);
		VECTOR forward = VGet(1, 0, 0);//自分の右ベクトル
		float ip = VDot(targetVec, forward);//２つのベクトルの内積
		if (ip > 0)
			isRight = true;
		else
			isRight = false;

		cdTimer+=1.0f/60.0f;

		if (cdTimer > 5.0f) {
			state = SHOT;
			animFrame = 0;
			cdTimer = 0.0f;
		}
	}

	if (state == SHOT) {
		if (animFrame < SHOT_MAXFRAME - 1) {
			frameCounter++;
			if (animFrame == 11) {
				if (frameCounter >= 50) {
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
			if (animFrame >= 12 && !afterShot) {
				afterShot = true;
				PlaySoundMem(shotSound, DX_PLAYTYPE_BACK);
				Arrow* pArrow = Instantiate<Arrow>(GetParent());
				pArrow->SetArrow(transform_.position_.x, transform_.position_.y + SHOT_HEIGHT, isRight);
			}
		}
		else {
			state = NORMAL;
			animFrame = 0;
			frameCounter = 0;
			afterShot = false;
		}
	}

	if (pField != nullptr) {
		float cx = SKELETON_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = SKELETON_HEIGHT / 2.0f;

		float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		jumpSpeed += GRAVITY;
		transform_.position_.y += jumpSpeed;

		int pushRbottom = pField->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pField->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
		}

		int pushRtop = pField->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pField->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}

	for (ItemBox* pIBox : pIBoxs) {
		float cx = SKELETON_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = SKELETON_HEIGHT / 2.0f;

		float pushTright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		//transform_.position_.y += GRAVITY;

		int pushRbottom = pIBox->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pIBox->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
		}

		int pushRtop = pIBox->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pIBox->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}
}

void Skeleton::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	//DrawRotaGraph(x, y,1.0f, 0, hWalkImage[animFrame], TRUE,!isRight);
	DrawRectGraph(x - SKELETON_WIDTH / 2.0, y - SKELETON_HEIGHT / 2.0 - CORRECT_BOTTOM, animFrame * SKELETON_WIDTH, animType * SKELETON_HEIGHT, SKELETON_WIDTH, SKELETON_HEIGHT, hAnimImg, TRUE, !isRight);
}

void Skeleton::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Skeleton::CollideRectToRect(float x, float y, float w, float h)
{
	if (state != DEAD) {
		// 敵の矩形の範囲を計算
		float myRectRight = transform_.position_.x + SKELETON_WIDTH / 2.0f - COLLIDE_WIDTH;
		float myRectLeft = transform_.position_.x - SKELETON_WIDTH / 2.0f + COLLIDE_WIDTH;
		float myRectTop = transform_.position_.y - SKELETON_HEIGHT / 2.0f + COLLIDE_HEIGHT;
		float myRectBottom = transform_.position_.y + SKELETON_HEIGHT / 2.0f - CORRECT_BOTTOM;

		// 指定された矩形と敵の矩形が交差しているかチェック
		if ((x - w / 2.0f < myRectRight && x + w / 2.0f > myRectLeft) &&
			(y - h / 2.0f < myRectBottom && y + h / 2.0f > myRectTop)) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void Skeleton::KillEnemy()
{
	state = DEAD;
	animType = 1;
	animFrame = 0;
	frameCounter = 0;
	cdTimer = 0.0f;
	PlaySoundMem(deathSound, DX_PLAYTYPE_BACK);
}

bool Skeleton::IsSteppedOnHead(float x, float y, float w, float h)
{
	if (state != DEAD) {
		// 敵の矩形の範囲を計算
		float myHeadRight = transform_.position_.x + SKELETON_WIDTH / 2.0f - COLLIDE_WIDTH;
		float myHeadLeft = transform_.position_.x - SKELETON_WIDTH / 2.0f + COLLIDE_WIDTH;
		float myHeadTop = transform_.position_.y - SKELETON_HEIGHT / 2.0f + COLLIDE_HEIGHT;
		float myHeadBottom = transform_.position_.y - 30.0f;

		// 指定された矩形と敵の矩形が交差しているかチェック
		if ((x - w / 2.0f < myHeadRight && x + w / 2.0f > myHeadLeft) &&
			(y - h / 2.0f < myHeadBottom && y + h / 2.0f > myHeadTop)) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
