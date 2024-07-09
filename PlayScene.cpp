#include"PlayScene.h"
//#include"Player.h"
//#include"GoalObj.h"
////#include"Field.h"
//#include"Camera.h"
//#include"BackGround.h"

//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
{
}

//初期化 
void PlayScene::Initialize()
{
	/*Instantiate<Camera>(this);
	Instantiate<BackGround>(this);
	pField = Instantiate<Field>(this);
	Instantiate<GoalObj>(this);
	Instantiate<Player>(this);*/
	StartReady();
}

//更新
void PlayScene::Update()
{
	switch (state) {
	case s_Ready:
		UpdateReady();
		break;
	case s_Play:
		UpdatePlay();
		break;
	case s_Clear:
		UpdateClear();
		break;
	case s_Stop:
		UpdateStop();
		break;
	case s_Dead:
		UpdateDead();
		break;
	}
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{
}

bool PlayScene::CanMove()
{
	return (state == s_Play);
}

void PlayScene::GameObjectsReset()
{
	if (pCam != nullptr) {
		pCam = nullptr;
	}
	if (pBG != nullptr) {
		pBG = nullptr;
	}
	if (pField != nullptr) {
		pField = nullptr;
	}
	if (pGObj != nullptr) {
		pGObj = nullptr;
	}
	if (pPlayer != nullptr) {
		pPlayer = nullptr;
	}
	if (pField != nullptr) {
		pField = nullptr;
	}
}

void PlayScene::StartReady()
{
	state = s_Ready;
	//GameObjectsReset();
	pCam = Instantiate<Camera>(this);
	pBG = Instantiate<BackGround>(this);
	pField = Instantiate<Field>(this);
	pGObj = Instantiate<GoalObj>(this);
	pPlayer = Instantiate<Player>(this);
	pField->Reset();
	readyTimer = 2.0f;
}

void PlayScene::StartClear()
{
	state = s_Clear;
}

void PlayScene::StartStop(float time)
{
	state = s_Stop;
	timerEnd = time;
}

void PlayScene::UpdateReady()
{
	readyTimer -= 1.0f / 60.0f;
	if (readyTimer <= 0.0f) {
		StartPlay();
	}
}

void PlayScene::StartPlay()
{
	state = s_Play;
}

void PlayScene::UpdatePlay()
{
}

void PlayScene::UpdateClear()
{
	StartReady();
	pField->SetNextStage();//ステージ番号を進める
	pField->Reset();//ステージの再読み込み
}

void PlayScene::UpdateStop()
{
	readyTimer -= 1.0f / 60.0f;
	if (readyTimer <= timerEnd) {
		StartPlay();
	}
}

void PlayScene::StartDead()
{
	state = s_Dead;
	readyTimer = 1.0f;
}

void PlayScene::UpdateDead()
{
	readyTimer -= 1.0f / 60.0f;
	if (readyTimer <= 0.0f) {
		StartPlay();
	}
}
