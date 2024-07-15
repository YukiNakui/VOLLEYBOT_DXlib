#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"GoalObj.h"
#include"Field.h"
#include"Camera.h"
#include"BackGround.h"

//テストシーンを管理するクラス
class PlayScene : public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
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