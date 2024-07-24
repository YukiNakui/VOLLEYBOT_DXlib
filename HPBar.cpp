#include "HPBar.h"

namespace {
	const float HPBAR_POSX = 200.0f;
	const float HPBAR_POSY = 50.0f;
	const float hartPosX = 150.0f;
	const float hartPosY = 50.0f;
	const float HART_SPACE = 50.0f;
	const int MAX_HP = 5;
}

HPBar::HPBar(GameObject* parent)
{
	hBarImage = LoadGraph("Assets/HPBar.png");
	assert(hBarImage > 0);
	hHartImage = LoadGraph("Assets/Hart.png");
	assert(hHartImage > 0);
	hBlackHartImage = LoadGraph("Assets/BlackHart.png");
	assert(hBlackHartImage > 0);
	manuImage = LoadGraph("Assets/manu.png");
	assert(manuImage > 0);
	playerHP = 5;
}

HPBar::~HPBar()
{
	if (hBarImage > 0)
		DeleteGraph(hBarImage);
	if (hHartImage > 0)
		DeleteGraph(hHartImage);
	if (hBlackHartImage > 0)
		DeleteGraph(hBlackHartImage);
	if (manuImage > 0)
		DeleteGraph(manuImage);
}

void HPBar::Update()
{
}

void HPBar::Draw()
{
	DrawRotaGraph(HPBAR_POSX, HPBAR_POSY, 1.0, 0, hBarImage, TRUE);
	for (int i = 0; i < MAX_HP; i++) {
		DrawRotaGraph(hartPosX+i*HART_SPACE, hartPosY, 2.0, 0, hBlackHartImage, TRUE);
	}
	for (int i = 0; i < playerHP; i++) {
		DrawRotaGraph(hartPosX+i*HART_SPACE, hartPosY, 2.0, 0, hHartImage, TRUE);
	}
	DrawRotaGraph(100, 630, 0.3, 0, manuImage, TRUE);
}

void HPBar::SetHP(int hp)
{
	playerHP = hp;
}
