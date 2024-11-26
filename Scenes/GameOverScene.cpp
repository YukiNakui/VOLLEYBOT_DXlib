#include "GameOverScene.h"
#include"../Engine/SceneManager.h"

GameOverScene::GameOverScene(GameObject* parent)
{
	hImage = LoadGraph("Assets/Scenes/GAMEOVER.png");
	assert(hImage > 0);
	goBGM = LoadSoundMem("Assets/Sounds/GB-Action-D09-1(ED).mp3");
	assert(goBGM > 0);
}

void GameOverScene::Initialize()
{
	PlaySoundMem(goBGM, DX_PLAYTYPE_LOOP);
}

void GameOverScene::Update()
{
	if (CheckHitKey(KEY_INPUT_C)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	DrawGraph(0, 0, hImage, TRUE);
}

void GameOverScene::Release()
{
	if (hImage > 0) {
		DeleteGraph(hImage);
	}
	if (goBGM > 0) {
		DeleteSoundMem(goBGM);
	}
}
