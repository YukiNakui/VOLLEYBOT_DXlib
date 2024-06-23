#pragma once
#include "Engine/GameObject.h"

namespace GOAL_ANIMFRAME {
	const int GOAL_MAXFRAME = 3;
}
namespace GAF = GOAL_ANIMFRAME;

class GoalObj :
    public GameObject
{
	int hImage[GAF::GOAL_MAXFRAME];
	int animFrame;
	int frameCounter;
	float gRectTop;
	float gRectBottom;
	float gRectLeft;
	float gRectRight;
public:
	GoalObj(GameObject* scene);
	~GoalObj();
	void Update() override;
	void Draw() override;

	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h);
	void Goal();
	void UpdateRect();
};

