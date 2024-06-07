#include "Field.h"
#include <assert.h>
#include"Player.h"
#include"Engine/CsvReader.h"

Field::Field(GameObject* scene) :GameObject(scene)
{
	Map = nullptr;//ポインタ変数はコンストラクタでnullを入れておく
	hImage = LoadGraph("Assets/bgchar.png");
	assert(hImage > 0);
}

Field::~Field()
{
	if (hImage > 0)
	{
		DeleteGraph(hImage);
	}
	if (Map != nullptr) {//Mapになにも入っていないときdeleteしないようにする
		delete[] Map;//Mapは配列
	}
}

void Field::Reset()
{
	if (Map != nullptr) {
		delete[] Map;
		Map = nullptr;
	}
	CsvReader csv;//データを読むクラスのインスタンスを作成
	bool ret = csv.Load("Assets/stage1.csv");
	assert(ret);
	width = csv.GetWidth(0);
	height = csv.GetHeight();
	Map = new int[width * height];//C言語での動的二次元配列の取り方

	for (int h = 0; h < height; h++) {
		if (csv.GetString(0, h) == "") {
			height = h;
			break;
		}
		for (int w = 0; w < width; w++) {
			Map[h * width + w] = csv.GetInt(w, h);
		}
	}
	//Mapデータの中で0があれば、Playerの座標を0の位置にする
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			switch (csv.GetInt(w, h + height + 1))
			{
			case 0://Player
			{//switch caseのなかでは変数の宣言できないが{}のなかならできる
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
		//当たっているので、めり込んだ量を返す
		return x % 32 + 1;
	return 0;
}

float Field::CollisionDown(int x, int y)
{
	if (IsWallBlock(x, y))
		//当たっているので、めり込んだ量を返す
		return y % 32 + 1;
	return 0;
}

float Field::CollisionLeft(int x, int y)
{
	if (IsWallBlock(x, y))
		//当たっているので、めり込んだ量を返す
		return 32 - (x % 32);
	return 0;
}

float Field::CollisionUp(int x, int y)
{
	if (IsWallBlock(x, y))
		//当たっているので、めり込んだ量を返す
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
	return false;//最後に必ずreturnがあったほうがいい
}