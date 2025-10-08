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
	int err = LoadDivGraph((Application::PATH_OBJECT + "player_stand.png").c_str(),
		ANIM_NUMS,     // 総コマ数 = 6
		ANIM_NUMS,     // 横方向 = 6
		1,             // 縦方向 = 1
		PLAYER_WID,    // 112
		PLAYER_HIG,    // 84
		&player_img[0][0] // 配列先頭
	);

	if (err == -1)
	{
		printfDx("player_stand.png 読み込み失敗\n");
		return false;
	}

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
	if (CheckHitKey(KEY_INPUT_W)) {
		playerPos.y -= MOVE_SPEED;
		if (playerPos.y < (PLAYER_HIG / 2))playerPos.y = PLAYER_HIG / 2;
		playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_UP);
	}
	if (CheckHitKey(KEY_INPUT_S)) {
		playerPos.y += MOVE_SPEED;
		if (playerPos.y >= (StageBase::MAP_CHIP_SIZE_HIG * StageBase::MAP_GROUND_NUM_Y) - PLAYER_HIG / 2) {
			playerPos.y = (StageBase::MAP_CHIP_SIZE_HIG * StageBase::MAP_GROUND_NUM_Y) - PLAYER_HIG / 2;
		}
		playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_DOWN);
	}
	if (CheckHitKey(KEY_INPUT_A)) {
		playerPos.x -= MOVE_SPEED;
		if (playerPos.x < (PLAYER_WID / 2))playerPos.x = PLAYER_WID / 2;
		playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_LEFT);
	}
	if (CheckHitKey(KEY_INPUT_D)) {
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
	int animNo = (animCounter / ANIM_INTERVAL) % ANIM_NUMS;
	Vector2 stpos = gInst->GetLpStage()->GetMapDispStPos();
	DrawGraph(playerPos.x - PLAYER_WID / 2 - (stpos.x * StageBase::MAP_CHIP_SIZE_WID),
		playerPos.y - PLAYER_HIG / 2 - (stpos.y * StageBase::MAP_CHIP_SIZE_HIG),
		player_img[playerDir][animNo], true);
}
// 解放処理(最後の１回のみ実行)
bool Player::Release(void)
{
	for (int yy = static_cast<int>(AsoUtility::DIRECTION::E_DIR_MAX); yy > 0; yy--) {
		for (int xx = ANIM_NUMS; xx > 0; xx--) {
			if (DeleteGraph(player_img[yy - 1][xx - 1]) == -1)return false;
		}
	}
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
