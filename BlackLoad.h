#pragma once
#include "Engine/GameObject.h"

class BlackLoad :
    public GameObject
{
    int hImage;
public:
	BlackLoad(GameObject* parent);
	~BlackLoad();
	void Update() override;
	void Draw() override;
	enum LoadID {
		L_Nothing = 1,
		L_Start = 0,
		L_Clear,
		L_GameOver,
	};
	void Load(LoadID id,float x,float y);
private:
	LoadID load;
	float scale;
	//float overY;
	//float overAlpha;
	//float overTimer;
};

