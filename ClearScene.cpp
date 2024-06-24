#include "ClearScene.h"
#include"Engine/SceneManager.h"

ClearScene::ClearScene(GameObject* parent)
{
	hImage = LoadGraph("Assets/Clear.png");
	assert(hImage > 0);
}

void ClearScene::Initialize()
{
}

void ClearScene::Update()
{
	if (CheckHitKey(KEY_INPUT_C)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TEST);
	}
}

void ClearScene::Draw()
{
	DrawGraph(0, 0, hImage, TRUE);
}

void ClearScene::Release()
{
}
