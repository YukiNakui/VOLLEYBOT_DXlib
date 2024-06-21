#include "TestScene.h"
#include"Player.h"
#include"Field.h"
#include"Camera.h"
#include"BackGround.h"

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化 
void TestScene::Initialize()
{
	Instantiate<Camera>(this);
	Instantiate<BackGround>(this);
	Field* pField = Instantiate<Field>(this);
	Instantiate<Player>(this);
	pField->Reset();
}

//更新
void TestScene::Update()
{
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}
