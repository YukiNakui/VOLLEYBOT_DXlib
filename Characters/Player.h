#pragma once
#include "../Engine/GameObject.h"
#include"../Objects/Ball.h"

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
	XMFLOAT3 GetPosition();
private:
	
	//int hWalkImage[PAF::WALK_MAXFRAME];
	int hAnimImage;
	int hBallImg;

	int hp;
	
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
	bool prevTossKey;
	bool prevSpikeKey;
	bool prevMoveKey;
	float moveSpeed;
	float jumpSpeed;
	bool onGround;
	float jumpTimer;

	int animType;//状況
	int animFrame;//コマ
	int frameCounter;
	
	bool isRight;
	bool canMove;
	bool canWalk;

	Ball* pBall;

	bool isBallAlive;
	bool canSpike;
	int tossCount;
	float cdTimer;
	bool spikeAfter;
	
	float damageTimer;
	bool nowDamage;
	bool blink;
	bool fallNow;
	bool tossNow;

	VECTOR firstPos;
	VECTOR target;
	float moveTime;
	VECTOR start;
	float currentTime;
	
	bool IsTouchBall(XMFLOAT3 pos);
	
	int spikeSound;
	int tossSound;
	int catchSound;
	int walkSound;
	int jumpSound;
	int damageSound;
	int deathSound;
};

