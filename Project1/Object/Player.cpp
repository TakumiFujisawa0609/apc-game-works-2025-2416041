#include <DxLib.h>
#include "Player.h"


Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	 LoadDivGraph((
		Application::PATH_OBJECT + "player.png").c_str(), PLAYER_ANIM_XNUM, PLAYER_ANIM_XNUM, 0, 96, 12, numberHandle_);
}

void Player::Update(void)
{
}

void Player::Draw(void)
{
	DrawGraph(0, 0, numberHandle_[i], true);
}

void Player::Release(void)
{
	
}
