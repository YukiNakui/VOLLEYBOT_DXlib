#pragma once
#include "../Engine/GameObject.h"

namespace BALL_ANIMFRAME{
	const int CHARGE_MAXFRAME = 10;
}
namespace BAF = BALL_ANIMFRAME;

class Ball :
    public GameObject
{
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Ball(GameObject* parent);
	~Ball();

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	void SetPosition(float x,float y);
	
	void Spike(bool isRight,bool moveNow);
	void FirstToss();
	void SecondToss();
	
	bool IsBallAlive();
	XMFLOAT3 GetPos();
	bool IsBallCatch(float x,float y);
	void SetIsAlive(bool isalive) { isAlive = isalive; }
	int GetBallSize();
	void SetIsRight(bool right);

	void SetCharge(bool chargenow,int tosscount);
	bool IsTouchGround();
private:
	int hImage;
	int hChargeImg[BAF::CHARGE_MAXFRAME*2];

	XMVECTOR moveVec;
	bool isAlive;
	float r;
	float accsel;
	float rot;
	float rotSpeed;
	bool isRight;

	bool chargeNow;
	bool isTouchGround;

	int animType;
	int animFrame;
	int frameCounter;

	enum State {
		NORMAL,
		TOSS,
		SPIKE,
	};
	State state;
private:
	int breakSound;
};

