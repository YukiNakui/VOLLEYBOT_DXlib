#pragma once
#include "Engine/GameObject.h"
#include"Ball.h"

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
private:
	int hImage;
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
	

	enum State {
		S_Walk = 0,
		S_Cry,
	};
	State state;

};

