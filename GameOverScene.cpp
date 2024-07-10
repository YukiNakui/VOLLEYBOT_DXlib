#include "GameOverScene.h"
#include"Engine/SceneManager.h"

GameOverScene::GameOverScene(GameObject* parent)
{
	hImage = LoadGraph("Assets/GameOver.png");
	assert(hImage > 0);
}

void GameOverScene::Initialize()
{
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
}
