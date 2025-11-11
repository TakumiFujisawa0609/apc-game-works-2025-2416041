#include <DxLib.h>
#include "Enemy.h"
#include "../../Scene/Stage/StageBase.h"
#include "../../Scene/GameScene.h"
#include "../Player.h"
#include "../../Application.h"
Enemy::Enemy(void)
{
}
Enemy::~Enemy(void)
{
}
bool Enemy::SystemInit(GameScene* gs)
{
	gInst = gs;
	SetEnemyParam(); // 敵キャラ個別のパラメータ設定処理
	std::string path = Application::PATH_OBJECT +"Enemy/";
	path += imgFName;
	LoadDivGraph(path.c_str(), ANIM_NUMS, ANIM_NUMS, 1, size.x, size.y, img);

	return true;
}
void Enemy::GameInit(void)
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
	iFrame = 0;

}
void Enemy::Update(void)
{
	animCounter++;
	if (animCounter > (ANIM_NUMS * ANIM_INTERVAL) * 100)animCounter = 0;

	if (iFrame > 0) --iFrame;

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
void Enemy::Draw(void)
{
	int animNo = (animCounter / ANIM_INTERVAL) % ANIM_NUMS;
	Vector2 stpos = gInst->GetLpStage()->GetMapDispStPos();

	// 座標をint型に変換する
	Vector2 ePos = AsoUtility::Round(pos);

	DrawGraph(ePos.x - size.x / 2 - (stpos.x * StageBase::MAP_CHIP_SIZE_WID),
		ePos.y - size.y / 2 - (stpos.y * StageBase::MAP_CHIP_SIZE_HIG),
		img[dir],true);
}
bool Enemy::Release(void)
{
	for (int yy = static_cast<int>(AsoUtility::DIRECTION::E_DIR_MAX); yy > 0; yy--) {
		for (int xx = ANIM_NUMS; xx > 0; xx--) {
			if (DeleteGraph(img[yy - 1]) == -1)return false;
		}
	}
	return true;
}
void Enemy::SetDamege(int dp)
{
	if (iFrame > 0 || !aliveFlg) return;

	hp -= dp;
	if (hp <= 0) {
		hp = 0;
		aliveFlg = false;
	}
	else {
		// 追加: 次の数フレームは無敵
		iFrame = HURT_IFRAME;
	}
}
void Enemy::SetMoveDirection(Vector2F edir)
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
