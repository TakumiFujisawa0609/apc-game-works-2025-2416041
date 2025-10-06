#include <DxLib.h>
#include "Enemy.h"
#include "../Scene/Stage/StageBase.h"
#include "Player.h"
EnemyBase::EnemyBase(void)
{
}
EnemyBase::~EnemyBase(void)
{
}
bool EnemyBase::SystemInit(GameScene* gs)
{
	gInst = gs;
	SetEnemyParam(); // 敵キャラ個別のパラメータ設定処理
	std::string path = "image/";
	path += imgFName;
	int err = LoadDivGraph(path.c_str(), CHARA_MAX,
		ANIM_NUMS, static_cast<int>(AsoUtility::DIRECTION::E_DIR_MAX),
		size.x, size.y, img[0]);
	if (err == -1)return false;
	return true;
}
void EnemyBase::GameInit(void)
{
	dir = GetRand(static_cast<int>(AsoUtility::DIRECTION::E_DIR_MAX) - 1);
	AsoUtility::DIRECTION eDir = static_cast<AsoUtility::DIRECTION>(dir);
	Vector2 rpos = gInst->GetLpStage()->GetMapDispStPos();
	rpos.x *= StageBase::MAP_CHIP_SIZE_WID;
	rpos.y *= StageBase::MAP_CHIP_SIZE_HIG;
	switch (eDir) {
	case AsoUtility::DIRECTION::E_DIR_UP:
		rpos.x += GetRand(Application::SCREEN_SIZE_WID - 1);
		rpos.y += Application::SCREEN_SIZE_HIG;
		break;
	case AsoUtility::DIRECTION::E_DIR_DOWN:
		rpos.x += GetRand(Application::SCREEN_SIZE_WID - 1);
		break;
	case AsoUtility::DIRECTION::E_DIR_LEFT:
		rpos.y += GetRand(Application::SCREEN_SIZE_HIG - 1);
		rpos.x += Application::SCREEN_SIZE_WID;
		break;
	case AsoUtility::DIRECTION::E_DIR_RIGHT:
		rpos.y += GetRand(Application::SCREEN_SIZE_HIG - 1);
		break;
	}
	pos.x = static_cast<float>(rpos.x);
	pos.y = static_cast<float>(rpos.y);
	animCounter = 0;
	aliveFlg = true;
}
void EnemyBase::Update(void)
{
	animCounter++;
	if (animCounter > (ANIM_NUMS * ANIM_INTERVAL) * 100)animCounter = 0;
	// プレイヤーの位置
	Vector2 pPos = gInst->GetLpPlayer()->GetPlayerPos();

	// ベクトル(相手－自分)
	Vector2F vec = Vector2F(static_cast<float>(pPos.x) - pos.x,
		static_cast<float>(pPos.y) - pos.y);

	// ベクトルの大きさ(長さ)
	float size = sqrtf(vec.x * vec.x + vec.y * vec.y); // 平方根の計算
	if (size >= speed) {
		// 方向(単位ベクトル)
		Vector2F unitDir;
		unitDir.x = vec.x / size;
		unitDir.y = vec.y / size;

		// 座標に移動量を加える
		pos.x += unitDir.x * speed;
		pos.y += unitDir.y * speed;

		// 向きを変える(移動量が大きい向きを優先する)
#if 0
		// abs() は絶対値を取得する関数
		if (abs(unitDir.x) < abs(unitDir.y)) {
			// 上下
			if (unitDir.y > 0.0f) {
				dir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_DOWN);
			}
			else {
				dir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_UP);
			}
		}
		else {
			// 左右
			if (unitDir.x > 0.0f) {
				dir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_RIGHT);
			}
			else {
				dir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_LEFT);
			}
		}
#else
		SetMoveDirection(unitDir);
#endif
	}
}
void EnemyBase::Draw(void)
{
	int animNo = (animCounter / ANIM_INTERVAL) % ANIM_NUMS;
	Vector2 stpos = gInst->GetLpStage()->GetMapDispStPos();

	// 座標をint型に変換する
	Vector2 ePos = AsoUtility::Round(pos);

	DrawGraph(ePos.x - size.x / 2 - (stpos.x * StageBase::MAP_CHIP_SIZE_WID),
		ePos.y - size.y / 2 - (stpos.y * StageBase::MAP_CHIP_SIZE_HIG),
		img[dir][animNo], true);
}
bool EnemyBase::Release(void)
{
	for (int yy = static_cast<int>(AsoUtility::DIRECTION::E_DIR_MAX); yy > 0; yy--) {
		for (int xx = ANIM_NUMS; xx > 0; xx--) {
			if (DeleteGraph(img[yy - 1][xx - 1]) == -1)return false;
		}
	}
	return true;
}
void EnemyBase::SetDamege(int dp)
{
	hp -= dp;
	if (hp <= 0) {
		hp = 0;
		aliveFlg = false;
	}
}
void EnemyBase::SetMoveDirection(Vector2F edir)
{
	if (abs(edir.x) < abs(edir.y)) {
		// 上下
		if (edir.y > 0.0f) {
			dir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_DOWN);
		}
		else {
			dir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_UP);
		}
	}
	else {
		// 左右
		if (edir.x > 0.0f) {
			dir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_RIGHT);
		}
		else {
			dir = static_cast<int>(AsoUtility::DIRECTION::E_DIR_LEFT);
		}
	}
}
