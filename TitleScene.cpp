#include "TitleScene.h"
#include"Engine/SceneManager.h"

namespace {
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	const int BALL_POSX = 640;
	const int BALL_POSY = 350;
	const int PLESS_POSX = 640;
	const int PLESS_POSY = 600;
}

TitleScene::TitleScene(GameObject* parent)
{
	hBallImage = LoadGraph("Assets/ValleyBall.png");
	assert(hBallImage > 0);
	hBGImage = LoadGraph("Assets/BG.png");
	assert(hBGImage > 0);
	hTitleImage = LoadGraph("Assets/Scenes/TITLE_TEXT.png");
	assert(hTitleImage > 0);
	hPlessImage = LoadGraph("Assets/Scenes/TITLE_PLESS.png");
	assert(hPlessImage > 0);
	bgPosX = 0;
	bgPosY = SCREEN_HEIGHT / 2.0;
	rotAngle = 0.0f;
	titleBGM = LoadSoundMem("Assets/Sounds/GB-Action-D01-3(Title-Loop140).mp3");
	assert(titleBGM > 0);
}

void TitleScene::Initialize()
{
	PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP);
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}

	bgPosX -= 1.0f;
	while (bgPosX < 0) {
		bgPosX += SCREEN_WIDTH;
	}
	rotAngle += 1.0f/60.0f;
	if (rotAngle > 360.0f) {
		rotAngle = 0.0f;
	}
}

void TitleScene::Draw()
{
	DrawRotaGraph(bgPosX, bgPosY, 1.0f, 0, hBGImage, TRUE);
	DrawRotaGraph(bgPosX + SCREEN_WIDTH, bgPosY, 1.0f, 0, hBGImage, TRUE);
	DrawRotaGraph(bgPosX - SCREEN_WIDTH, bgPosY, 1.0f, 0, hBGImage, TRUE);
	DrawRotaGraph(BALL_POSX, BALL_POSY, 1.0f, rotAngle, hBallImage, TRUE);
	DrawRotaGraph(BALL_POSX, BALL_POSY, 1.0f, 0, hTitleImage, TRUE);
	DrawRotaGraph(PLESS_POSX, PLESS_POSY, 0.7f, 0, hPlessImage, TRUE);
}

void TitleScene::Release()
{
	if (hBallImage > 0) {
		DeleteGraph(hBallImage);
	}
	if (hBGImage > 0) {
		DeleteGraph(hBGImage);
	}
	if (hPlessImage > 0) {
		DeleteGraph(hPlessImage);
	}
	if (hTitleImage > 0) {
		DeleteGraph(hTitleImage);
	}
	if (titleBGM > 0) {
		DeleteSoundMem(titleBGM);
	}
}
