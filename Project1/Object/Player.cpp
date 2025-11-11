#include <DxLib.h>
#include "Player.h"
#include "../Application.h"
#include "../Scene/GameScene.h"
#include "Bullet.h"
#include "../Manager/InputManager.h"
Player::Player(GameScene* gs)
{
	bullet = new Bullet(gs);
	bullet->SystemInit();
	gInst = gs;
}
Player::~Player(void)
{
	bullet->Release();
	delete bullet;
}
// 初期化処理(最初の１回のみ実行)
bool Player::SystemInit(void)
{
	int err = LoadDivGraph((Application::PATH_OBJECT + "Player/player_stand.png").c_str(),
		ANIM_NUMS,     // 総コマ数
		ANIM_NUMS,     
		1,             // 縦方向
		PLAYER_WID,    // 112
		PLAYER_HIG,    // 84
		player_img // 配列先頭
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
    if (animCounter > (ANIM_NUMS * ANIM_INTERVAL) * 10) animCounter = 0;

    float moveX = 0.0f;
    float moveY = 0.0f;

    // ───────────────
    // キーボード
    if (CheckHitKey(KEY_INPUT_W)) moveY -= MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_S)) moveY += MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_A)) moveX -= MOVE_SPEED;
    if (CheckHitKey(KEY_INPUT_D)) moveX += MOVE_SPEED;

    // ───────────────
    // パッド（PAD1）
    auto& pad = InputManager::GetInstance();
    // 毎フレーム更新（必要なら有効化）
    if constexpr (true) { pad.Update(); }

    auto st = pad.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

    // 値レンジ自動判別
    auto norm = [](float v) {
        return (fabsf(v) > 1.5f) ? (v / 32767.0f) : v;
        };
    float lx = norm(st.AKeyLX);
    float ly = norm(st.AKeyLY);

    // デッドゾーン
    const float DEAD = 0.20f;
    if (fabsf(lx) < DEAD) lx = 0.0f;
    if (fabsf(ly) < DEAD) ly = 0.0f;

    // 
    constexpr bool INVERT_Y = false;
    moveX += lx * MOVE_SPEED;
    moveY += (INVERT_Y ? -ly : ly) * MOVE_SPEED;

    //// 方向ボタン
    //if (pad.IsPadBtnDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP))    moveY = -MOVE_SPEED;
    //if (pad.IsPadBtnDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))   moveY = MOVE_SPEED;
    //if (pad.IsPadBtnDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT))   moveX = -MOVE_SPEED;
    //if (pad.IsPadBtnDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))  moveX = MOVE_SPEED;

    // ───────────────
    // 移動
    playerPos.x += moveX;
    playerPos.y += moveY;

    // 画面端補正
    if (playerPos.x < PLAYER_WID / 2) playerPos.x = PLAYER_WID / 2;
    if (playerPos.x > (StageBase::MAP_CHIP_SIZE_WID * StageBase::MAP_GROUND_NUM_X) - PLAYER_WID / 2)
        playerPos.x = (StageBase::MAP_CHIP_SIZE_WID * StageBase::MAP_GROUND_NUM_X) - PLAYER_WID / 2;

    if (playerPos.y < PLAYER_HIG / 2) playerPos.y = PLAYER_HIG / 2;
    if (playerPos.y > (StageBase::MAP_CHIP_SIZE_HIG * StageBase::MAP_GROUND_NUM_Y) - PLAYER_HIG / 2)
        playerPos.y = (StageBase::MAP_CHIP_SIZE_HIG * StageBase::MAP_GROUND_NUM_Y) - PLAYER_HIG / 2;

    // 向き（移動優先）
    if (moveX < 0) playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_LEFT);
    else if (moveX > 0) playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_RIGHT);
    if (moveY < 0) playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_UP);
    else if (moveY > 0) playerDir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_DOWN);

    // 弾
    bullet->Update();
}
// 描画処理
void Player::Draw(void)
{
	int animNo = (animCounter / ANIM_INTERVAL) % ANIM_NUMS;
	Vector2 stpos = gInst->GetLpStage()->GetMapDispStPos();
	DrawGraph(playerPos.x - PLAYER_WID / 2 - (stpos.x * StageBase::MAP_CHIP_SIZE_WID),
		playerPos.y - PLAYER_HIG / 2 - (stpos.y * StageBase::MAP_CHIP_SIZE_HIG),
		player_img[animNo], true);

	for (int i = 0;i < hp; i++) {
		DrawBox(50 + (i * 5), Application::SCREEN_SIZE_HIG-30, 70 + (i + 5), Application::SCREEN_SIZE_WID, GetColor(255, 1, 1), true);
	}

	bullet->Draw();

	
}
// 解放処理(最後の１回のみ実行)
bool Player::Release(void)
{
	bool err = true;

	for (auto& id : player_img) {
		if (DeleteGraph(id) == -1) { err = false; }
	}

	return err;
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

