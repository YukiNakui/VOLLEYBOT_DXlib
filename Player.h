#pragma once
#include "Engine/GameObject.h"
#include"Ball.h"

//namespace PLAYER_ANIMFRAME {
//	const int IDLE_MAXFRAME{ 6 };
//	const int WALK_MAXFRAME{ 6 };
//	const int TOSS_MAXFRAME{ 4 };
//	const int SPIKE_MAXFRAME{ 4 };
//	const int DEAD_MAXFRAME{ 5 };
//}
//namespace PAF = PLAYER_ANIMFRAME;

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
	
	//int hWalkImage[PAF::WALK_MAXFRAME];
	int hAnimImage;
	int hBallImg;
	
	enum State {
		NORMAL = 0,
		TOSS,
		SPIKE,
		DAMAGE,
		DEAD,
	};
	State state;

	GameObject* sceneTop;
	bool prevSpaceKey;
	bool prevAttackKey;
	float jumpSpeed;
	bool onGround;

	int animType;//状況
	int animFrame;//コマ
	int frameCounter;
	
	bool isRight;
	bool canMove;

	Ball* pBall;

	bool isBallAlive;
	bool canSpike;
	int tossCount;
	float cdTimer;
	
	bool IsTouchBall(XMFLOAT3 pos);
	void KnockBack(XMFLOAT3& pos);//ダメージ時のノックバック

};

