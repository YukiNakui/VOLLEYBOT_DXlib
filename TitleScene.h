#pragma once
#include "Engine/GameObject.h"
class TitleScene :
    public GameObject
{
	int hImage;
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
};
