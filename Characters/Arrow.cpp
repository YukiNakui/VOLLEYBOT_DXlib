#include "Arrow.h"
#include"../Objects/Camera.h"
#include"../Objects/Field.h"
#include"../Objects/ItemBox.h"
#include"../Scenes/PlayScene.h"

namespace {
	const float ARROW_WIDTH = 48.0f;
	const float ARROW_HEIGHT = 6.0f;

	const float MOVE_SPEED = 4.0f;
	const float GRAVITY = 9.8f / 60.0f;//重力加速度

	const float CORRECT_WIDTH = 1.0f;
	const float CORRECT_HEIGHT = 21.0f;
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	const float COLLIDE_WIDTH = 5.0f;
	const float COLLIDE_HEIGHT = 5.0f;
}

Arrow::Arrow(GameObject* parent):Enemy(parent)
{
	hImage = LoadGraph("Assets/Enemy/Arrow.png");
	assert(hImage > 0);

	isRight = false;
	cdTimer = 0.0f;
	state = NORMAL;
}

Arrow::~Arrow()
{
	if (hImage > 0) {
		DeleteGraph(hImage);
	}
}

void Arrow::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->CanMove())
		return;

	if (state == HIT) {
		cdTimer++;
		if (cdTimer > 10.0f)
			state == NORMAL;
			KillMe();
	}

	if (state == NORMAL) {
		if (isRight) {
			transform_.position_.x += MOVE_SPEED;
		}
		else {
			transform_.position_.x -= MOVE_SPEED;
		}
	}

	if (pField != nullptr) {
		float cx = ARROW_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = ARROW_HEIGHT / 2.0f;

		float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_HEIGHT - 1.0f);
		float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_HEIGHT + 1.0f);
		float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
		if (pushRight > 0.0f) {
			transform_.position_.x -= pushRight - 1.0f;
			state = HIT;
		}

		float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_HEIGHT - 1.0f);
		float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_HEIGHT + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
		if (pushLeft > 0.0f) {
			transform_.position_.x += pushLeft - 1.0f;
			state = HIT;
		}

		int pushRbottom = pField->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_HEIGHT);
		int pushLbottom = pField->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_HEIGHT);
		int pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			state = HIT;
		}

		int pushRtop = pField->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_HEIGHT);
		int pushLtop = pField->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_HEIGHT);
		int pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			state = HIT;
		}
	}

	for (ItemBox* pIBox : pIBoxs) {
		float cx = ARROW_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = ARROW_HEIGHT / 2.0f;

		float pushTright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_HEIGHT - 1.0f);
		float pushBright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_HEIGHT + 1.0f);
		float pushRight = max(pushBright, pushTright);//右側の頭と足元で当たり判定
		if (pushRight > 0.0f) {
			transform_.position_.x -= pushRight - 1.0f;
			state = HIT;
		}

		float pushTleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_HEIGHT - 1.0f);
		float pushBleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_HEIGHT + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//左側の頭と足元で当たり判定
		if (pushLeft > 0.0f) {
			transform_.position_.x += pushLeft - 1.0f;
			state = HIT;
		}

		int pushRbottom = pIBox->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_HEIGHT);
		int pushLbottom = pIBox->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_HEIGHT);
		int pushBottom = max(pushRbottom, pushLbottom);//2つの足元のめり込みの大きいほう
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			state = HIT;
		}

		int pushRtop = pIBox->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_HEIGHT);
		int pushLtop = pIBox->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_HEIGHT);
		int pushTop = max(pushRtop, pushLtop);//2つの頭のめり込みの大きいほう
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			state = HIT;
		}
	}

	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	if ((y < 0 || y > WINDOW_HEIGHT) ||
		(x < 0 || x > WINDOW_WIDTH)) {
		KillMe();
	}
}

void Arrow::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRotaGraph(x, y,1.0f, 0, hImage, TRUE,!isRight);
}

void Arrow::SetArrow(int x, int y, bool isright)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
	isRight = isright;
}

bool Arrow::CollideRectToRect(float x, float y, float w, float h)
{
	if (state != HIT) {
		// 敵の矩形の範囲を計算
		float myRectRight = transform_.position_.x + ARROW_WIDTH / 2.0f - COLLIDE_WIDTH;
		float myRectLeft = transform_.position_.x - ARROW_WIDTH / 2.0f + COLLIDE_WIDTH;
		float myRectTop = transform_.position_.y - ARROW_HEIGHT / 2.0f + COLLIDE_HEIGHT;
		float myRectBottom = transform_.position_.y + ARROW_HEIGHT / 2.0f - COLLIDE_HEIGHT;

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

void Arrow::KillEnemy()
{
	state = HIT;
}

bool Arrow::IsSteppedOnHead(float x, float y, float w, float h)
{
    return false;
}
