#pragma once
#include "../Application.h"
class Player
{
public:
	Player(void);
	~Player(void);

	void Init(void) ;
	void Update(void);
	void Draw(void);
	void Release(void);
private:

	const int PLAYER_SIZE_WID = 672;
	const int PLAYER_SIZE_HIG = 84;
	const int PLAYER_ANIM_XNUM = 7;
	const int PLAYER_ANIM_YNUM = 1;
	const int PLAYER_ANIM_MAX = PLAYER_ANIM_XNUM * PLAYER_ANIM_YNUM;

	const int PLAYER_MOVE_SPEED = 4;

	int numberHandle_[7];
	int playerPosX;
	int playerPosY;
	int i = 0;
};

