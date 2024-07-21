#include "HPBar.h"

HPBar::HPBar(GameObject* parent)
{
	hBarImage = LoadGraph("Assets/HPBar.png");
	assert(hBarImage > 0);
	hHartImage = LoadGraph("Assets/Hart.png");
	assert(hHartImage > 0);
	hBlackHartImage = LoadGraph("Assets/BlackHart.png");
	assert(hBlackHartImage > 0);
}

HPBar::~HPBar()
{
}

void HPBar::Update()
{
}

void HPBar::Draw()
{
}

void HPBar::GetHP(int hp)
{
}
