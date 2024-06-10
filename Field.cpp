#include "Field.h"
#include <assert.h>
#include"Player.h"
#include"Engine/CsvReader.h"

Field::Field(GameObject* scene) :GameObject(scene)
{
	Map = nullptr;//�|�C���^�ϐ��̓R���X�g���N�^��null�����Ă���
	hImage = LoadGraph("Assets/bgchar.png");
	assert(hImage > 0);
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
	CsvReader csv;//�f�[�^��ǂރN���X�̃C���X�^���X���쐬
	bool ret = csv.Load("Assets/stage1.csv");
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
			switch (csv.GetInt(w, h + height + 1))
			{
			case 0://Player
			{//switch case�̂Ȃ��ł͕ϐ��̐錾�ł��Ȃ���{}�̂Ȃ��Ȃ�ł���
				Player* pPlayer = GetParent()->FindGameObject<Player>();
				//pPlayer->SetPosition(w * 32, h * 32);
			}
			break;
			}
		}
	}
}

void Field::Update()
{
	if (CheckHitKey(KEY_INPUT_R))
		Reset();
}

void Field::Draw()
{
	/*int scroll = 0;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		scroll = cam->GetValue();
	}*/

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int chip = Map[y * width + x];
			DrawRectGraph(x * 32/* - scroll*/, y * 32, 32 * (chip % 16), 32 * (chip / 16), 32, 32, hImage, TRUE);
		}
	}
}

float Field::CollisionRight(int x, int y)
{
	if (IsWallBlock(x, y))
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return x % 32 + 1;
	return 0;
}

float Field::CollisionDown(int x, int y)
{
	if (IsWallBlock(x, y))
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return y % 32 + 1;
	return 0;
}

float Field::CollisionLeft(int x, int y)
{
	if (IsWallBlock(x, y))
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return 32 - (x % 32);
	return 0;
}

float Field::CollisionUp(int x, int y)
{
	if (IsWallBlock(x, y))
		//�������Ă���̂ŁA�߂荞�񂾗ʂ�Ԃ�
		return 32 - (y % 32);
	return 0;
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
	case 32:
	case 33:
	case 34:
	case 35:
		return true;
	}
	return false;//�Ō�ɕK��return���������ق�������
}