#pragma once
#include "Engine/GameObject.h"
#include"Ball.h"

namespace PLAYER_ANIMFRAME {
	const int WALK_MAXFRAME{ 6 };
}
namespace PAF = PLAYER_ANIMFRAME;

class Player :
    public GameObject
{
public:
	Player(GameObject* scene);
	~Player();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// プレイヤーの座標をセットする
	/// </summary>
	/// <param name="x">X座標（左）</param>
	/// <param name="y">Y座標（上）</param>
	void SetPosition(int x, int y);
	bool IsTouchBall(XMFLOAT3 pos);
private:
	int hImage;
	int hWalkImage[PAF::WALK_MAXFRAME];
	
	GameObject* sceneTop;
	bool prevSpaceKey;
	bool prevAttackKey;
	float jumpSpeed;
	bool onGround;
	int animType;//状況
	int animFrame;//コマ
	int frameCounter;
	bool isRight;
	Ball* pBall;
	bool isBallAlive;
	int tossCount;
	

	enum State {
		NORMAL = 0,
		WALK,
		TOSS,
		SPIKE,
		DEAD,
		MAX,
	};
	State state;

};

