#pragma once
#include "../Engine/GameObject.h"
class GameOverScene :
    public GameObject
{
	int hImage;
public:
	GameOverScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
private:
	int goBGM;
};

