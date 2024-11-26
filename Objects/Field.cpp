#include "Field.h"
#include <assert.h>
//#include"Player.h"
//#include"Wolf.h"
//#include"Skeleton.h"
//#include"Bird.h"
//#include"ItemBox.h"
//#include"GoalObj.h"
#include"../Engine/CsvReader.h"
#include"Camera.h"

namespace {
	const int STAGE_NUM = 3;
}

Field::Field(GameObject* scene) :GameObject(scene)
{
	Map = nullptr;//�|�C���^�ϐ��̓R���X�g���N�^��null�����Ă���
	hImage = LoadGraph("Assets/bgchar2.png");
	assert(hImage > 0);
	stageNum = 1;
}

Field::~Field()
{
	if (hImage > 0)
	{
		DeleteGraph(hImage);
	}
	if (Map != nullptr) {//Map�ɂȂɂ������Ă��Ȃ��Ƃ�delete���Ȃ��悤�ɂ���
		delete[] Map;//Map�͔z��
	}
}

void Field::Reset()
{
	if (Map != nullptr) {
		delete[] Map;
		Map = nullptr;
	}
//	GameObjectsReset();
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	char s[20];
	sprintf_s<20>(s, "Assets/stage%d.csv", stageNum);
	bool ret = csv.Load(s);
	assert(ret);
	width = csv.GetWidth(0);
	height = csv.GetHeight();
	Map = new int[width * height];//C����ł̓��I�񎟌��z��̎���

	for (int h = 0; h < height; h++) {
		if (csv.GetString(0, h) == "") {
			height = h;
			break;
		}
		for (int w = 0; w < width; w++) {
			Map[h * width + w] = csv.GetInt(w, h);
		}
	}
	//Map�f�[�^�̒���0������΁APlayer�̍��W��0�̈ʒu�ɂ���
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			switch (csv.GetInt(w, h))
			{
			case 0:
			{
				pGoal = GetParent()->FindGameObject<GoalObj>();
				pGoal->SetPosition(w * 32, h * 32);
			}
			break;
			case 1:
			{
				pIBox = Instantiate<ItemBox>(GetParent());
				pIBox->SetPosition(w * 32, h * 32);
			}
			break;
			case 10://Player
			{//switch case�̂Ȃ��ł͕ϐ��̐錾�ł��Ȃ���{}�̂Ȃ��Ȃ�ł���
				pPlayer = GetParent()->FindGameObject<Player>();
				pPlayer->SetPosition(w * 32, h * 32 + 2.0f);
			}
			break;
			case 11://Wolf
			{
				pWolf = Instantiate<Wolf>(GetParent());
				pWolf->SetPosition(w * 32, h * 32);
			}
			break;
			case 12://Skeleton
			{
				pSkeleton = Instantiate<Skeleton>(GetParent());
				pSkeleton->SetPosition(w * 32, h * 32);
			}
			break;
			case 13://Bird
			{
				pBird = Instantiate<Bird>(GetParent());
				pBird->SetPosition(w * 32, h * 32);
			}
			break;
			}
		}
	}
}

void Field::Update()
{
	/*if (CheckHitKey(KEY_INPUT_R))
		Reset();*/
}

void Field::Draw()
{
	int scrollX = 0;
	int scrollY = 0;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		scrollX = cam->GetValueX();
		scrollY = cam->GetValueY();
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int chip = Map[y * width + x];
			DrawRectGraph(x * 32 - scrollX, y * 32 - scrollY, 32 * (chip % 16), 32 * (chip / 16), 32, 32, hImage, TRUE);
		}
	}
}

float Field::CollisionRight(float x, float y)
{
	if (IsWallBlock(x, y))
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return (int)x % 32 + 1;
	return 0;
}

float Field::CollisionDown(float x, float y)
{
	if (IsWallBlock(x, y))
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return (int)y % 32 + 1;
	return 0;
}

float Field::CollisionLeft(float x, float y)
{
	if (IsWallBlock(x, y))
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return 32 - ((int)x % 32);
	return 0;
}

float Field::CollisionUp(float x, float y)
{
	if (IsWallBlock(x, y))
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return 32 - ((int)y % 32);
	return 0;
}

void Field::SetNextStage()
{
	stageNum += 1;
}

bool Field::CanNextStageChange()
{
	if (stageNum < STAGE_NUM)
		return true;
	return false;
}

void Field::GameObjectsReset()
{
	if (pGoal != nullptr) {
		pGoal = nullptr;
	}
	if (pIBox != nullptr) {
		pIBox = nullptr;
	}
	if (pPlayer != nullptr) {
		pPlayer = nullptr;
	}
	if (pWolf != nullptr) {
		pWolf = nullptr;
	}
	if (pSkeleton != nullptr) {
		pSkeleton = nullptr;
	}
	if (pBird != nullptr) {
		pBird = nullptr;
	}
}

bool Field::IsFall(float x, float y)
{
	int chipX = x / 32;
	int chipY = y / 32;
	switch (Map[chipY * width + chipX]) {
	case 47:
		return true;
	}
	return false;
}

bool Field::IsWallBlock(int x, int y)
{
	int chipX = x / 32;
	int chipY = y / 32;
	switch (Map[chipY * width + chipX]) {
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
		return true;
	}
	return false;//�Ō�ɕK��return���������ق�������
}
