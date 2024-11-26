#include "ClearScene.h"
#include"../Engine/SceneManager.h"

ClearScene::ClearScene(GameObject* parent)
{
	hImage = LoadGraph("Assets/Scenes/GAMECLEAR.png");
	assert(hImage > 0);
	clearBGM = LoadSoundMem("Assets/Sounds/NES-Action-C10-3(Clear1).mp3");
	assert(clearBGM > 0);
}

void ClearScene::Initialize()
{
	PlaySoundMem(clearBGM, DX_PLAYTYPE_BACK);
}

void ClearScene::Update()
{
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	DrawGraph(0, 0, hImage, TRUE);
}

void ClearScene::Release()
{
	if (hImage > 0) {
		DeleteGraph(hImage);
	}
	if (clearBGM > 0) {
		DeleteSoundMem(clearBGM);
	}
}
