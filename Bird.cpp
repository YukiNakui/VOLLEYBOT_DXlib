#include "Bird.h"

Bird::Bird(GameObject* parent):Enemy(parent)
{
}

Bird::~Bird()
{
}

void Bird::Update()
{
}

void Bird::Draw()
{
}

void Bird::SetPosition(int x, int y)
{
}

bool Bird::CollideRectToRect(float x, float y, float w, float h)
{
    return false;
}
