#include "BlackLoad.h"

BlackLoad::BlackLoad(GameObject* parent)
{
	hImage = LoadGraph("Assets/Black.png");
	assert(hImage > 0);
	scale = 0.0f;
	load = L_Nothing;
}

BlackLoad::~BlackLoad()
{
	if (hImage > 0)
		DeleteGraph(hImage);
}

void BlackLoad::Update()
{
	if (load == LoadID::L_Start) {
		scale += 10.0f / 60.0f;
		if (scale >= 20.0f)
			scale = 20.0f;
	}
	else if (load == LoadID::L_GameOver) {
		scale -= 10.0f / 60.0f;
		if (scale <= 0.5f)
			scale = 0.5f;
	}
}

void BlackLoad::Draw()
{
	DrawRotaGraph(transform_.position_.x, transform_.position_.y, scale, 0, hImage, TRUE);
}

void BlackLoad::Load(LoadID id, float x, float y)
{
	load = id;
	transform_.position_.x = x;
	transform_.position_.y = y;
	if (id == LoadID::L_Start) {
		//•Ï”‚Ì‰Šú‰»
		scale = 0.5f;
	}
	else if (id == LoadID::L_GameOver) {
		//•Ï”‚Ì‰Šú‰»
		scale = 5.0f;
	}
}
