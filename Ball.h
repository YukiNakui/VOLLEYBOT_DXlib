#pragma once
#include "Engine/GameObject.h"
class Ball :
    public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Ball(GameObject* parent);
	~Ball();

	//更新
	void Update() override;

	//描画
	void Draw() override;

	void SetPosition(float x,float y);
	void Spike(bool isRight);
	void FirstToss();
	void SecondToss();
	bool IsBallAlive();
	XMFLOAT3 GetPos();
	bool IsBallCatch(float x,float y);
	void SetIsAlive(bool isalive) { isAlive = isalive; }
	int GetBallSize();
private:
	int hImage;
	XMVECTOR moveVec;
	bool isAlive;
	float r;
	float speed;
	float accsel;
};

