#include "GoalObj.h"
#include"Camera.h"

namespace {
	const int IMAGE_WIDTH = 515;
	const int IMAGE_HEIGHT = 384;
	const float CORRECT_LEFT = 150;
	const float CORRECT_RIGHT = 40.0f;
	const float CORRECT_X = 277;
	const float CORRECT_Y = 16.0f;
}

GoalObj::GoalObj(GameObject* scene)
{
	LoadDivGraph("Assets/Goal.png", GAF::GOAL_MAXFRAME, GAF::GOAL_MAXFRAME, 1, IMAGE_WIDTH, IMAGE_HEIGHT, hImage);
	for (int i = 0; i < GAF::GOAL_MAXFRAME; i++) {
		assert(hImage[i] > 0);
	}
	animFrame = 0;
	frameCounter = 0;
	UpdateRect();
}

GoalObj::~GoalObj()
{
	for (int i = 0; i < GAF::GOAL_MAXFRAME; i++) {
		if (hImage[i] > 0)
		{
			DeleteGraph(hImage[i]);
		}
	}
}

void GoalObj::Update()
{
	UpdateRect();
}

void GoalObj::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRotaGraph(x, y, 1.0f, 0, hImage[animFrame], TRUE);

	// デバッグ用の当たり判定の矩形を描画
    //unsigned int Cr = GetColor(0, 0, 255); // 青色の値を取得
    //DrawBox((int)gRectLeft, (int)gRectTop, (int)gRectRight, (int)gRectBottom, Cr, FALSE); // 四角形を描画
}

void GoalObj::SetPosition(int x, int y)
{
	transform_.position_.x = x - CORRECT_X;
	transform_.position_.y = y - CORRECT_Y;
	UpdateRect();
}

bool GoalObj::CollideRectToRect(float x, float y, float w, float h)
{
	if (x - w / 2.0f < gRectRight && x + w / 2.0f > gRectLeft) {
		if (y - h / 2.0f < gRectBottom && y + h / 2.0f > gRectTop) {
			return true;
		}
	}
	return false;
}

void GoalObj::Goal()
{
	animFrame = 2;
}

void GoalObj::UpdateRect()
{
	gRectTop = transform_.position_.y - IMAGE_HEIGHT / 2.0f;
	gRectBottom = transform_.position_.y + IMAGE_HEIGHT / 2.0f - CORRECT_Y;
	gRectLeft = transform_.position_.x + CORRECT_LEFT;
	gRectRight = transform_.position_.x + IMAGE_WIDTH / 2.0f - CORRECT_RIGHT;
}
