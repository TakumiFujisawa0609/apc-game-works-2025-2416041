#include <DxLib.h>
#include "Player.h"
#include "../Application.h"
Player::Player(GameScene* gs)
{
	gInst = gs;
}
Player::~Player(void)
{
}
// 初期化処理(最初の１回のみ実行)
bool Player::SystemInit(void)
{
	player_img_stand = LoadGraph((Application::PATH_OBJECT + "player_stand.png").c_str());
	player_img_walk = LoadGraph((Application::PATH_OBJECT + "player_walk.png").c_str());
	return true;
}
// ゲーム起動・再開時に必ず呼び出す処理
void Player::GameInit(void)
{
	playerPos.x = PLAYER_WID / 2 + StageBase::MAP_CHIP_SIZE_WID;
	playerPos.y = PLAYER_HIG / 2 + StageBase::MAP_CHIP_SIZE_HIG;
	// デフォルトのプレイヤーの向き
	playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_DOWN);
	animCounter = 0;
	aliveFlg = true;
	hp = PLAYER_HP_MAX;
}
// 更新処理
void Player::Update(void)
{
	animCounter++;
	if (animCounter > (ANIM_NUMS * ANIM_INTERVAL) * 10)animCounter = 0;
	if (CheckHitKey(KEY_INPUT_UP)) {
		playerPos.y -= MOVE_SPEED;
		if (playerPos.y < (PLAYER_HIG / 2))playerPos.y = PLAYER_HIG / 2;
		playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_UP);
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		playerPos.y += MOVE_SPEED;
		if (playerPos.y >= (StageBase::MAP_CHIP_SIZE_HIG * StageBase::MAP_GROUND_NUM_Y) - PLAYER_HIG / 2) {
			playerPos.y = (StageBase::MAP_CHIP_SIZE_HIG * StageBase::MAP_GROUND_NUM_Y) - PLAYER_HIG / 2;
		}
		playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_DOWN);
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		playerPos.x -= MOVE_SPEED;
		if (playerPos.x < (PLAYER_WID / 2))playerPos.x = PLAYER_WID / 2;
		playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_LEFT);
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		playerPos.x += MOVE_SPEED;
		if (playerPos.x >= (StageBase::MAP_CHIP_SIZE_WID * StageBase::MAP_GROUND_NUM_X) - PLAYER_WID / 2) {
			playerPos.x = (StageBase::MAP_CHIP_SIZE_WID * StageBase::MAP_GROUND_NUM_X) - PLAYER_WID / 2;
		}
		playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_RIGHT);
	}
}
// 描画処理
void Player::Draw(void)
{
	// 動いているかどうかをチェック
	isMoving = CheckHitKey(KEY_INPUT_UP) ||
		CheckHitKey(KEY_INPUT_DOWN) ||
		CheckHitKey(KEY_INPUT_LEFT) ||
		CheckHitKey(KEY_INPUT_RIGHT);

	// 画像切り替え
	imgToDraw = isMoving ? player_img_walk : player_img_stand;

	// 描画
	if (imgToDraw != -1)
		DrawGraph(drawX, drawY, imgToDraw, TRUE);
}
// 解放処理(最後の１回のみ実行)
bool Player::Release(void)
{
	
	return true;
}
// プレイヤーにダメージを与える
// Input:
// int dp : ダメージ
// Ountput:
// 無し
void Player::SetDamage(int dp)
{
	hp -= dp;
	if (hp <= 0) {
		hp = 0;
		aliveFlg = false;
	}
}
