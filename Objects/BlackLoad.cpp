#include "BlackLoad.h"

BlackLoad::BlackLoad(GameObject* parent)
{
	hImage = LoadGraph("Assets/Black.png");
	assert(hImage > 0);
	loadSound = LoadSoundMem("Assets/Sounds/Scene_Change04-2(Up-Delay).mp3");
	assert(loadSound > 0);
	scale = 0.0f;
	load = L_Nothing;
}

BlackLoad::~BlackLoad()
{
	if (hImage > 0)
		DeleteGraph(hImage);
	if (loadSound > 0)
		DeleteSoundMem(loadSound);
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
		PlaySoundMem(loadSound, DX_PLAYTYPE_BACK);
	}
	else if (id == LoadID::L_GameOver) {
		//•Ï”‚Ì‰Šú‰»
		scale = 5.0f;
	}
}
