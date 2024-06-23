#include "BackGround.h"
#include"Camera.h"

namespace {
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
}

BackGround::BackGround(GameObject* scene)
{
	hImage = LoadGraph("Assets/BG.png");
	assert(hImage > 0);
	transform_.position_.x = 0;
	transform_.position_.y = SCREEN_HEIGHT / 2.0;
}

BackGround::~BackGround()
{
	if (hImage > 0)
	{
		DeleteGraph(hImage);
	}
}

void BackGround::Update()
{
	/*if (transform_.position_.x < -SCREEN_WIDTH/2.0) {
		transform_.position_.x += SCREEN_WIDTH / 2.0;
	}
	else if (transform_.position_.x > SCREEN_WIDTH + SCREEN_WIDTH / 2.0) {
		transform_.position_.x -= SCREEN_WIDTH / 2.0;
	}*/

}

void BackGround::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue() / 2.0f;
	}
	if (x < 0.0f) {
		x += SCREEN_WIDTH;
	}
	else if (x > SCREEN_WIDTH) {
		x -= SCREEN_WIDTH;
	}
	DrawRotaGraph(x, y, 1.0f, 0, hImage, TRUE);
	DrawRotaGraph(x + SCREEN_WIDTH, y, 1.0f, 0, hImage, TRUE);
	
	if (x > 0) {
		DrawRotaGraph(x - SCREEN_WIDTH, y, 1.0f, 0, hImage, TRUE);
	}
}
