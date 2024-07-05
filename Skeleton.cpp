#include "Skeleton.h"

Skeleton::Skeleton(GameObject* parent):Enemy(parent)
{
}

Skeleton::~Skeleton()
{
}

void Skeleton::Update()
{
}

void Skeleton::Draw()
{
}

void Skeleton::SetPosition(int x, int y)
{
}

bool Skeleton::CollideRectToRect(float x, float y, float w, float h)
{
    return false;
}
