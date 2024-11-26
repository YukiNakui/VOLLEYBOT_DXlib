#pragma once
#include "../Engine/GameObject.h"
#include"../Characters/Player.h"
#include"../Objects/GoalObj.h"
#include"../Objects/Field.h"
#include"../Objects/Camera.h"
#include"../Objects/BackGround.h"

//�e�X�g�V�[�����Ǘ�����N���X
class PlayScene : public GameObject
{
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	PlayScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	bool CanMove();
	void GameObjectsReset();
private:
	Camera* pCam;
	BackGround* pBG;
	Field* pField;
	GoalObj* pGObj;
	Player* pPlayer;

	enum State {
		s_Ready,
		s_Play,
		s_Clear,
		s_Dead,
		s_Stop,
	};
	State state;
	
	void UpdateReady();
	void StartPlay();
	void UpdatePlay();
	void UpdateClear();
	void UpdateStop();
public:
	void StartReady();
	void StartClear();
	void StartStop(float time);
	void StartDead();
private:
	void UpdateDead();
	float readyTimer;
	float timerEnd;

	int playBGM;
};