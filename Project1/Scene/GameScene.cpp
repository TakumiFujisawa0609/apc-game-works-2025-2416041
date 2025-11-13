// GameScene class
#include <DxLib.h>
#include <cmath>

#include "GameScene.h"
#include "../Application.h"
#include "../Scene/Stage/Stage_1.h"

#include "../Object/Player.h"
#include "../Object/Bullet.h"
#include "../Object/Enemy/Enemy.h"
#include "../Object/Enemy/Enemynormal.h"
#include "../Object/Enemy/Enemy_eye.h"
#include "../Object/Enemy/Enemy_GoblinRun.h"
#include "../Object/Enemy/Enemy_Mushroom.h"
#include "../Object/Enemy/Enemy_Skeleton.h"

#include "../Manager/InputManager.h"

GameScene::GameScene(void)
{
	stage = nullptr;
	player = nullptr;
}

GameScene::~GameScene(void)
{
}

// 初期化処理(最初の１回のみ実行)
bool GameScene::SystemInit(void)
{
	stage = new StageBase();
	if (!stage) return false;

	player = new Player(this);
	if (!player) return false;

	if (stage->SystemInit() == -1) return false;
	player->SystemInit();

	// 弾の共通リソース読み込み（画像など） 
	Bullet* tmp = new Bullet(this);
	if (!tmp->SystemInit()) return false;
	delete tmp;
	//個別弾は Update() で生成 
	bullets.clear();

	return true;
}

// ゲーム起動・再開時に必ず呼び出す処理
void GameScene::GameInit(void)
{
	stage->GameInit();
	player->GameInit();

	// 弾の初期化
	for (auto& b : bullets) { // もし既に残っている弾があれば削除
		b->Release();
		delete b;
	}

	bullets.clear();
	prevShotKey = nowShotKey = 0;
	enCounter = 0;
	nextSceneID = E_SCENE_GAME;

	startTime = GetNowCount();
	limitTime = 50000;
	isClear = false;

	autoShotTimer_ = 0;
	autoOrbitTimer_ = 0;
	autoFanTimer_ = 0;
}

// 更新処理
void GameScene::Update(void)
{
	
	InputManager::GetInstance().Update();

	Vector2 oldPos = player->GetPlayerPos(); // 移動前のプレイヤーの位置
	stage->Update();
	player->Update();

	static int shotTimer = 0;
	const int SHOT_INTERVAL = 30;

	
	autoShotTimer_++;
	autoOrbitTimer_++;
	autoFanTimer_++;

	// 直進の通常弾（一定間隔）
	{
		constexpr int SHOT_INTERVAL = 20; //30フレームごと
		if (autoShotTimer_ >= SHOT_INTERVAL)
		{
			autoShotTimer_ = 0;

			Bullet* newBullet = new Bullet(this);
			newBullet->SystemInit();
			newBullet->GameInit();

			Vector2 pos = player->GetPlayerPos();
			Vector2F posF(static_cast<float>(pos.x), static_cast<float>(pos.y));
			newBullet->Create(posF, player->GetPlayerDir()); // 既存API
			bullets.push_back(newBullet);
		}
	}

	// 回転弾（5発同時を一定間隔）
	{
		constexpr int ORBIT_INTERVAL = 360; //60×秒
		if (autoOrbitTimer_ >= ORBIT_INTERVAL)
		{
			autoOrbitTimer_ = 0;

			Vector2 pos = player->GetPlayerPos();

			constexpr int   N = 4;
			const     float radius = 80.0f;
			const     float omega = 0.1f;
			const     int   life = 600;
			const     float TWO_PI = 6.28318530718f;

			for (int i = 0; i < N; ++i)
			{
				Bullet* b = new Bullet(this);
				b->SystemInit();
				b->GameInit();

				b->CreateOrbit({ static_cast<float>(pos.x), static_cast<float>(pos.y) },
					radius, omega, life);               // 回転弾:contentReference[oaicite:5]{index=5}

				b->angle = (TWO_PI / N) * i;                        // 初期角度ずらし
				b->bPos.x = static_cast<float>(pos.x) + std::cos(b->angle) * radius;
				b->bPos.y = static_cast<float>(pos.y) + std::sin(b->angle) * radius;

				bullets.push_back(b);
			}
		}
	}

	// 扇状弾（10発を一定間隔）
	{
		constexpr int FAN_INTERVAL = 70; //1.5秒
		if (autoFanTimer_ >= FAN_INTERVAL)
		{
			autoFanTimer_ = 0;

			Vector2 pos = player->GetPlayerPos();

			constexpr int   N = 6;                         // 本数
			constexpr float SPREAD = 3.1415926535f / 3.0f;       // 60度
			constexpr int   LIFE = 600;                        // 寿命
			const     float HALF = SPREAD * 0.5f;

			// プレイヤー向き
			float baseAngle = 0.0f;
			switch (player->GetPlayerDir()) {
			case AsoUtility::DIRECTION::E_DIR_RIGHT: baseAngle = 0.0f;                      break;
			case AsoUtility::DIRECTION::E_DIR_LEFT:  baseAngle = 3.1415926535f;             break;
			case AsoUtility::DIRECTION::E_DIR_DOWN:  baseAngle = 3.1415926535f * 0.5f;      break;
			case AsoUtility::DIRECTION::E_DIR_UP:    baseAngle = -3.1415926535f * 0.5f;     break;
			default: break;
			}

			const float step = (N > 1) ? (SPREAD / (N - 1)) : 0.0f;

			for (int i = 0; i < N; ++i)
			{
				const float ang = baseAngle - HALF + step * i;

				Bullet* b = new Bullet(this);
				b->SystemInit();
				b->GameInit();

				b->CreateAngle({ static_cast<float>(pos.x), static_cast<float>(pos.y) },
					ang, LIFE);                       

				bullets.push_back(b);
			}
		}
	}

	// 弾の更新
	for (auto& b : bullets) {
		b->Update();
	}

	// 終了した弾を削除
	bullets.erase(
		std::remove_if(bullets.begin(), bullets.end(),
			[](Bullet* b) {
				if (b->IsEnableCreate()) {
					b->Release();
					delete b;
					return true;
				}
				return false;
			}),
		bullets.end()
	);


	// 敵の更新
	size_t size = enemys.size(); // 敵のテーブルの要素数を取得
	for (int ii = 0; ii < size; ii++) {
		enemys[ii]->Update();
	}

	// エンカウンター
	if (stage->GetMapType() == StageBase::MAP_TYPE::E_MIYPE_GROUND)enCounter++;
	if (enCounter > ENCOUNT) {

		// 敵の生成
		Enemy* e = nullptr;

		// ランダムに種別を決める
		int rr = GetRand(static_cast<int>(Enemy::ENEMY_TYPE::E_TYPE_MAX) - 1);
		Enemy::ENEMY_TYPE rType = static_cast<Enemy::ENEMY_TYPE>(rr);

		// 種別に対応した派生クラスのインスタンスを生成
		switch (rType) {
		case Enemy::ENEMY_TYPE::E_TYPE_NORMAL:
			e = new Enemynormal();
			break;
		case Enemy::ENEMY_TYPE::E_TYPE_EYE:
			e = new Eenmy_eye();
			break;
		/*case Enemy::ENEMY_TYPE::E_TYPE_GOBLIN:
			e = new Enemy_GoblinRun();
			break;
		case Enemy::ENEMY_TYPE::E_TYPE_MUSHROOM:
			e = new Enemy_Mushroom();
			break;
		case Enemy::ENEMY_TYPE::E_TYPE_SKELETON:
			e = new Enemy_Skeleton();
			break;*/
		}

		if (e != nullptr) {
			e->SystemInit(this);
			e->GameInit();
			// 可変長配列に要素を追加する
			enemys.push_back(e);
			enCounter = 0; // エンカウンターをリセット
		}
	}
	Vector2 playerPos = player->GetPlayerPos(); // 移動後のプレイヤーの位置
	AsoUtility::DIRECTION pdir = player->GetPlayerDir(); // 移動方向を取得

	if (pdir == AsoUtility::DIRECTION::E_DIR_DOWN) {
		// 下移動の場合のみプレイヤーのY座標を画像の下端に設定する
		playerPos.y += (Player::PLAYER_HIG / 2);
	}

	// 移動先が衝突対象地形かを調べる
	if (IsCollisionStage(playerPos)) {
		// 衝突対象地形なので移動不可なので、元の位置にプレイヤーを戻す。
	//	player->SetPlayerPos(oldPos);
	//	if (pdir == AsoUtility::DIRECTION::E_DIR_DOWN) {
	//		Vector2 mp = WorldPos2MapPos(playerPos);
	//		oldPos.y = Stage::MAP_CHIP_SIZE_HIG * mp.y - (Player::PLAYER_HIG / 2) - 1;
	//	}
		player->SetPlayerPos(oldPos);
	}
	else {
		if (pdir == AsoUtility::DIRECTION::E_DIR_LEFT || pdir == AsoUtility::DIRECTION::E_DIR_RIGHT) {
			// 念の為に、足元の座標が移動不可能地形にかかっていないか調べる
			Vector2 wPos = playerPos;
			wPos.y += (Player::PLAYER_HIG / 2);
			if (IsCollisionStage(wPos)) {
				// 足元が衝突対象地形にかかっているので、移動不可として元の位置にプレイヤーを戻す。
				player->SetPlayerPos(oldPos);
			}
		}
	}

	if (pdir == AsoUtility::DIRECTION::E_DIR_DOWN) {
		// 下移動の場合のみプレイヤーのY座標を変更しているので、元に戻す
		playerPos.y -= (Player::PLAYER_HIG / 2);
	}
	MapScrollProc();

	int elapsed = GetNowCount() - startTime;
	int remain = limitTime - elapsed;

	if (remain <= 0 && !isClear) {
		isClear = true;
		nextSceneID = E_SCENE_GAMECLEAR; // ← クリアシーンに遷移
	}

	// 衝突判定
	CollisionCheck();
	if (player->GetAlive()) {
		// 死亡した敵データを消去する
		for (int ii = (int)size; ii > 0; ii--) {
			if (!enemys[ii - 1]->GetAlive()) {
				enemys[ii - 1]->Release();
				delete enemys[ii - 1];
				//std::vector<Enemy*>::iterator itr;
				std::vector<Enemy*>::iterator itr;
				itr = enemys.begin() + (ii - 1);
				enemys.erase(itr);
			}
		}
	}
	else {
		// プレイヤーのＨＰが 0 になったらゲームオーバー
		EraseEnemys();
		nextSceneID = E_SCENE_GAMEOVER;
	}


}

// 描画処理
void GameScene::Draw(void)
{
	stage->Draw();
	player->Draw();
	// 敵の描画
	size_t size = enemys.size();
	for (int ii = 0; ii < size; ii++) {
		enemys[ii]->Draw();
	}

	for (auto& b : bullets) b->Draw();

	DrawBox(0, 0, Application::SCREEN_SIZE_WID, 20, GetColor(0, 0, 0), true);
	int php = player->GetHp();
	// 残り時間を表示
	int elapsed = GetNowCount() - startTime;
	int remain = max(0, limitTime - elapsed);
	DrawFormatString(300, 0, GetColor(255, 255, 255),
		"残り時間：%.2f秒", remain / 1000.0f);
	//-----------------------------------------------------------------------
	// デバッグ用
	Vector2 pPos = player->GetPlayerPos();
	Vector2 mPos = WorldPos2MapPos(pPos);
	DrawFormatString(0, 32, GetColor(0xff, 0xff, 0xff), "プレイヤー座標：(%d, %d)→(%d, %d)",
		pPos.x, pPos.y, mPos.x, mPos.y);
	int num = (int)enemys.size();
	DrawFormatString(0, 48, GetColor(0xff, 0xff, 0xff), "敵の数：%d", num);
	//-----------------------------------------------------------------------
	// 画面のスクロール範囲(デバッグ用)
	DrawBox(SCROLL_AREA_WID, SCROLL_AREA_HIG,
		Application::SCREEN_SIZE_WID - SCROLL_AREA_WID, Application::SCREEN_SIZE_HIG - SCROLL_AREA_HIG,
		GetColor(0, 0, 255), false);
}

// 解放処理(最後の１回のみ実行)
bool GameScene::Release(void)
{
	EraseEnemys();

	for (auto& b : bullets) { b->Release(); delete b; }

	player->Release();
	delete player;
	player = nullptr;

	stage->Release();
	delete stage;
	stage = nullptr;

	return true;
}

void GameScene::MapScrollProc(void)
{
	Vector2 mapStPos = stage->GetMapDispStPos();
	Vector2 playerPos = player->GetPlayerPos();
	// 現在のマップの端のマップ座標ではなく、ワールド座標を求める
	int leftEdge = mapStPos.x * StageBase::MAP_CHIP_SIZE_WID;
	int rightEdge = mapStPos.x * StageBase::MAP_CHIP_SIZE_WID + Application::SCREEN_SIZE_WID - 1;
	int upperEdge = mapStPos.y * StageBase::MAP_CHIP_SIZE_HIG;
	int lowerEdge = mapStPos.y * StageBase::MAP_CHIP_SIZE_HIG + Application::SCREEN_SIZE_HIG - 1;
	// 画面右端のマップチップ座標内に達しているか調べる

	if (playerPos.x >= rightEdge - SCROLL_AREA_WID) {
		// 表示画面の右端に達しているので、プレイヤー位置が横中央にくる表示開始位置を求める
		int dx = playerPos.x - Application::SCREEN_SIZE_WID / 2;
		dx /= StageBase::MAP_CHIP_SIZE_WID;
		dx -= mapStPos.x;
		if (dx < 0)dx *= -1;
		stage->MoveMapToRight(dx);
	}

	// 画面左端のマップチップ座標内に達しているか調べる
	if (playerPos.x <= leftEdge + SCROLL_AREA_WID) {
		// 表示画面の左端に達しているので、プレイヤー位置が横中央にくる表示開始位置を求める
		int dx = playerPos.x - Application::SCREEN_SIZE_WID / 2;
		dx /= StageBase::MAP_CHIP_SIZE_WID;
		dx -= mapStPos.x;
		if (dx < 0)dx *= -1;
		stage->MoveMapToLeft(dx);
	}

	// 画面上端のマップチップ座標内に達しているか調べる
	if (playerPos.y <= upperEdge + SCROLL_AREA_HIG) {
		// 表示画面の上端に達しているので、プレイヤー位置が縦中央にくる表示開始位置を求める
		int dy = playerPos.y - Application::SCREEN_SIZE_HIG / 2;
		dy /= StageBase::MAP_CHIP_SIZE_HIG;
		dy -= mapStPos.y;
		if (dy < 0)dy *= -1;
		stage->MoveMapToUpper(dy);
	}

	// 画面下端のマップチップ座標内に達しているか調べる
	if (playerPos.y >= lowerEdge - SCROLL_AREA_HIG) {
		// 表示画面の下端に達しているので、プレイヤー位置が横中央にくる表示開始位置を求める
		int dy = playerPos.y - Application::SCREEN_SIZE_HIG / 2;
		dy /= StageBase::MAP_CHIP_SIZE_HIG;
		dy -= mapStPos.y;
		if (dy < 0)dy *= -1;
		stage->MoveMapToDown(dy);
	}
}

/*
 *----------------------------------------------------
 * 敵データテーブルを空にする
 * Input:
 * 無し
 * Output:
 * 無し
 *----------------------------------------------------
 */
void GameScene::EraseEnemys(void)
{
	size_t size = enemys.size();
	for (int ii = (int)size; ii > 0; ii--) {
		enemys[ii - 1]->Release();
		// インスタンスの解放
		delete enemys[ii - 1];
	}
	enemys.clear(); // 可変長配列を空にする(サイズを0にする)
}

/*
 *----------------------------------------------------
 * ワールド座標をマップ座標に変換する
 * Input:
 * Vector2 wpos : ワールド座標
 * Output:
 * Vector2 : マップ座標
 *----------------------------------------------------
 */
Vector2 GameScene::WorldPos2MapPos(Vector2 wpos)
{
	Vector2 mpos;
	mpos.x = mpos.y = 0;
	mpos.x = wpos.x / StageBase::MAP_CHIP_SIZE_WID;
	mpos.y = wpos.y / StageBase::MAP_CHIP_SIZE_HIG;
	return mpos;
}

/*
 *----------------------------------------------------
 * ワールド座標の指す地形は衝突対象か調べる(衝突判定)
 * Input:
 * Vector2 worldPos : ワールド座標
 * Output:
 * bool : true = 衝突対象 / false = 衝突対象ではない(通行可能)
 *----------------------------------------------------
 */
bool GameScene::IsCollisionStage(Vector2 worldPos)
{
	// チップ対応の衝突対象情報テーブル
	bool chipCollisionInfoTbl[StageBase::MAP_CHIP_ALL_NUM] = {
	false, false, true, true, true, false, false, false, false,
	false, true, true, true, false, false, false, true, true,
	true, true, true, false, false, false, false, false, false,
	true, true, true, false, false, false, false, false, false,
	true, true, true, false, false, false, false, false, false,
	false, false, false, false, false, false, true, true, true,
	false, false, false, false, false, false, true, true, true,
	false, false, false, false, false, false, true, true, true,
	false, false, false, false, false, false, true, true, true,
	};
	if (worldPos.x >= StageBase::MAP_SIZE_WID || worldPos.y >= StageBase::MAP_SIZE_HIG) {
		// マップ範囲外の座標
		return true;
	}
	Vector2 mpos = WorldPos2MapPos(worldPos);
	int chipNo = stage->GetMapChipNo(mpos);
	bool rb = chipCollisionInfoTbl[chipNo];
	return rb;
}

/*
 *----------------------------------------------------
 * 衝突判定
 * Input:
 * 無し
 * Output:
 * 無し
 *----------------------------------------------------
 */
void GameScene::CollisionCheck(void)
{
	Vector2 pPos = player->GetPlayerPos();
	Vector2 pSize = { Player::PLAYER_WID, Player::PLAYER_HIG };

	std::vector<Bullet*> spawnQueue;

	// 敵の数だけチェック
	for (auto& e : enemys) {
		if (!e->GetAlive()) continue;
		Vector2 ePos = AsoUtility::Round(e->GetEnemyPos());
		Vector2 eSize = e->GetEnemySize();

		// 弾との判定
		for (auto& b : bullets) {
			if (!b->IsShotState()) continue;
			Vector2 bPos = AsoUtility::Round(b->GetBulletPos());
			Vector2 bSize = { Bullet::BULLET_SIZE_WID, Bullet::BULLET_SIZE_HIG };

			if (CollisionCheckRectCenter(bPos, bSize, ePos, eSize)) {
				if (!e->IsInvincible()) {
					e->SetDamege(1);
					////命中位置から小拡散弾を準備（8方向）
					const int   SHARD_COUNT = 6;
					const float TWO_PI = 6.28318530718f;
					const float STEP = TWO_PI / SHARD_COUNT;
					const int   SHARD_LIFE = 25;   // 短寿命

					for (int i = 0; i < SHARD_COUNT; ++i) {
						const float ang = STEP * i;

						Bullet* nb = new Bullet(this);
						nb->SystemInit();
						nb->GameInit();

						//任意角度直進を使
						nb->CreateAngle(
							{ static_cast<float>(bPos.x), static_cast<float>(bPos.y) },
							ang,
							SHARD_LIFE
						);

					spawnQueue.push_back(nb);
					}

					// 元の弾は爆発
					b->BlastOn(b->GetBulletPos());
				}
				
			}
		}

		// プレイヤーとの衝突判定
		if (!e->GetAlive()) continue;
		if (CollisionCheckRectCenter(pPos, pSize, ePos, eSize)) {
			player->SetDamage(1);
		}

		if (!player->GetAlive()) break;
	}
	// 最後にまとめて追加
	if (!spawnQueue.empty()) {
		bullets.insert(bullets.end(), spawnQueue.begin(), spawnQueue.end());
	}
}

/*
 *----------------------------------------------------
 * 中心座標から衝突判定を行う
 * Input:
 * Vector2 centerPos1 : 判定対象１の中心座標
 * Vector2 size1 : 判定対象１の縦横のサイズ
 * Vector2 centerPos2 : 判定対象２の中心座標
 * Vector2 size2 : 判定対象２の縦横のサイズ
 * Output:
 * bool : true = 衝突している / false = 衝突していない
 *----------------------------------------------------
 */
bool GameScene::CollisionCheckRectCenter(Vector2 centerPos1, Vector2 size1, Vector2 centerPos2, Vector2 size2)
{
	// １つ目の矩形の座標を求める
	Vector2 stPos1 = centerPos1;
	Vector2 edPos1 = centerPos1;
	stPos1.x -= (size1.x / 2);
	stPos1.y -= (size1.y / 2);
	edPos1.x += (size1.x / 2);
	edPos1.y += (size1.y / 2);
	// ２つ目の矩形の座標を求める
	Vector2 stPos2 = centerPos2;
	Vector2 edPos2 = centerPos2;
	stPos2.x -= (size2.x / 2);
	stPos2.y -= (size2.y / 2);
	edPos2.x += (size2.x / 2);
	edPos2.y += (size2.y / 2);
	// 矩形同士の衝突判定
	// 矩形１の左より矩形２の右が大きい かつ
	// 矩形１の右が矩形２の左より大きい かつ
	// 矩形１の上より矩形２の下が大きい かつ
	// 矩形１の下が矩形２の上より大きい
	if (stPos1.x < edPos2.x &&
		edPos1.x > stPos2.x &&
		stPos1.y < edPos2.y &&
		edPos1.y > stPos2.y) {
		return true;
	}
	return false;
}