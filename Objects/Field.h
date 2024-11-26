#pragma once
#include "../Engine/GameObject.h"
#include"../Characters/Player.h"
#include"../Characters/Wolf.h"
#include"../Characters/Skeleton.h"
#include"../Characters/Bird.h"
#include"ItemBox.h"
#include"GoalObj.h"

class Field :
    public GameObject
{
public:
	Field(GameObject* scene);
	~Field();
	void Reset();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// �E���̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂荞�񂾗ʁi�h�b�g�j</returns>
	float CollisionRight(float x, float y);

	/// <summary>
	/// ���̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂荞�񂾗ʁi�h�b�g�j</returns>
	float CollisionDown(float x, float y);

	/// <summary>
	/// ���̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂荞�񂾗ʁi�h�b�g�j</returns>
	float CollisionLeft(float x, float y);

	/// <summary>
	/// ��̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂荞�񂾗ʁi�h�b�g�j</returns>
	float CollisionUp(float x, float y);

	void SetNextStage();//���̃X�e�[�W���Z�b�g����
	bool CanNextStageChange();//���̃X�e�[�W�ɕύX�ł��邩
	void GameObjectsReset();
	bool IsFall(float x, float y);
private:
	int hImage;

	bool IsWallBlock(int x, int y);
	int* Map;
	int width;
	int height;
	int stageNum;

	GoalObj* pGoal;
	ItemBox* pIBox;
	Player* pPlayer;
	Wolf* pWolf;
	Skeleton* pSkeleton;
	Bird* pBird;
};

