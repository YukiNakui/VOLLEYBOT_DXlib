#pragma once
#include "../Engine/GameObject.h"
class TitleScene :
    public GameObject
{
	int hBallImage;
	int hBGImage;
	int hPlessImage;
	int hTitleImage;
public:
	TitleScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
private:
	float bgPosX;
	float bgPosY;
	float rotAngle;

	int titleBGM;
};

