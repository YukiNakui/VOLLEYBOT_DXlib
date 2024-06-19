#include "Enemy.h"
#include"Field.h"
#include "Engine/time.h"
#include"Camera.h"

namespace {
	const float ENEMY_WIDTH = 128.0f;
	const float ENEMY_HEIGHT = 128.0f;
	const float MOVE_SPEED = 1.5f;
	const float GRAVITY = 9.8f / 60.0f;//èdóÕâ¡ë¨ìx
	const float CORRECT_WIDTH = 35.0f;
	const float CORRECT_BOTTOM = 1.0f;
	const float CORRECT_TOP = 35.0f;
}

Enemy::Enemy(GameObject* scene)
{
	LoadDivGraph("Assets/EnemyWalk.png", EAF::WALK_MAXFRAME, EAF::WALK_MAXFRAME, 1, 128, 128, hWalkImage);
	for (int i = 0; i < EAF::WALK_MAXFRAME; i++) {
		assert(hWalkImage[i] > 0);
	}
	hWalkImage[EAF::DEAD_MAXFRAME];
	LoadDivGraph("Assets/EnemyDead.png", EAF::DEAD_MAXFRAME, EAF::DEAD_MAXFRAME, 1, 128, 128, hDeadImage);
	for (int i = 0; i < EAF::DEAD_MAXFRAME; i++) {
		assert(hDeadImage[i] > 0);
	}

	animFrame = 0;
	frameCounter = 0;
	isRight = true;
}

Enemy::~Enemy()
{
	for (int i = 0; i < EAF::WALK_MAXFRAME; i++) {
		if (hWalkImage[i] > 0)
		{
			DeleteGraph(hWalkImage[i]);
		}
	}
}

void Enemy::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	frameCounter ++;
	if (frameCounter > 6) {
		animFrame = (animFrame + 1) % EAF::WALK_MAXFRAME;
		frameCounter = 0;
	}
	if (isRight) {
		transform_.position_.x += MOVE_SPEED;
	}
	else {
		transform_.position_.x -= MOVE_SPEED;
	}

	

	if (pField != nullptr) {
		float cx = ENEMY_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = ENEMY_HEIGHT / 2.0f;

		float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);//âEë§ÇÃì™Ç∆ë´å≥Ç≈ìñÇΩÇËîªíË
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//ç∂ë§ÇÃì™Ç∆ë´å≥Ç≈ìñÇΩÇËîªíË
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		transform_.position_.y += GRAVITY;

		int pushRbottom = pField->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pField->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2Ç¬ÇÃë´å≥ÇÃÇﬂÇËçûÇ›ÇÃëÂÇ´Ç¢ÇŸÇ§
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
		}

		int pushRtop = pField->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pField->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2Ç¬ÇÃì™ÇÃÇﬂÇËçûÇ›ÇÃëÂÇ´Ç¢ÇŸÇ§
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			
		}
	}
}

void Enemy::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();
	}
	DrawRotaGraph(x, y,1.0f, 0, hWalkImage[animFrame], TRUE,!isRight);
}

void Enemy::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}


bool Enemy::CollideRectToRect(float x, float y, float w, float h)
{
	float myRectRight = transform_.position_.x + ENEMY_WIDTH / 2.0f - CORRECT_WIDTH;
	float myRectLeft = transform_.position_.x - ENEMY_WIDTH / 2.0f + CORRECT_WIDTH;
	float myRectTop = transform_.position_.y - ENEMY_HEIGHT / 2.0f + CORRECT_TOP;
	float myRectBottom = transform_.position_.y + ENEMY_HEIGHT / 2.0f - CORRECT_BOTTOM;
	if (((x - w / 2.0f > myRectLeft && x - w / 2.0f < myRectRight) ||(myRectLeft > x - w / 2.0f && myRectLeft < x + w / 2.0f)) &&
		((y - h / 2.0f > myRectTop && y - h / 2.0f < myRectBottom) ||(myRectTop > y - h / 2.0f && myRectTop < y + h / 2.0f))) {
		return true;
	}
	else {
		return false;
	}
	return false;
}
