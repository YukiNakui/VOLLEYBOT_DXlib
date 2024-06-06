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

	//開放
	void Release() override;

	void SetPosition(int x, int y);
	void ThrowBall();
	void TossBall();
private:
	int hImage;
	float speed;
	float bound;
};

