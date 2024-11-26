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

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
private:
	float bgPosX;
	float bgPosY;
	float rotAngle;

	int titleBGM;
};

