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

	void SetPosition(XMFLOAT3 pos);
	void SpikeBall();
	void TossBall();
private:
	int hImage;
	float speed;
	float bound;
	XMVECTOR vBall;
};

