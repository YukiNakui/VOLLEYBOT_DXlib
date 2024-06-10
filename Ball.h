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

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	void SetPosition(XMFLOAT3 pos);
	void SpikeBall(bool isRight);
	void TossBall();
	bool IsBallAlive();
private:
	int hImage;
	XMVECTOR moveVec;
	bool isAlive;
};

