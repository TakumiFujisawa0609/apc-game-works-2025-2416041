#pragma once

#include <vector>
#include <list>
#include "../Common/Vector2.h"
#include "../StDefineData.h"

class StageBase;
class Player;
class Enemy;
class Bullet;

class GameScene
{
public:


    //敵の弾の構造体
    struct B{
        Vector2 pos;//座標
        Vector2 vel;//速度
        bool isActive = false;//生きてるか～？
        Vector2 accel = { 0, 0 }; //加速度（必要なら使う）
    };

    // スクロールを発生させる範囲
    static constexpr int SCROLL_AREA_WID = 150;
    static constexpr int SCROLL_AREA_HIG = 100;

    // エンカウント(値が大きいと出現する間隔が長くなる）
    static constexpr int ENCOUNT = 80;

    GameScene(void);
    ~GameScene(void);

    bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
    void GameInit(void);   // ゲーム起動
    void Update(void);     // 更新処理
    void Draw(void);       // 描画処理
    bool Release(void);    // 解放処理(最後の１回のみ実行)

    StageBase* GetLpStage(void) { return stage; }
    Player* GetLpPlayer(void) { return player; }

    E_SCENE_ID GetNextSceneID(void) { return nextSceneID; }

   

private:
    StageBase* stage;  // Stageクラスのインスタンスのポインタ
    Player* player;    // Playerクラスのインスタンスのポインタ

    // 複数に変更！
    std::vector<Bullet*> bullets; // Bulletクラスのインスタンスを複数管理

    B bullet_magazine[256] = {};

    // 敵関連
    std::vector<Enemy*> enemys;   // 複数の敵を動的に管理
    std::list<Enemy*> enemysSortTbl; // 敵の描画順ソート用
    int enCounter; // 敵出現用カウンタ

    // キー入力管理
    int prevShotKey;
    int nowShotKey;

    int startTime;      // 開始時間（ミリ秒）
    int limitTime;      // 制限時間（ミリ秒）
    bool isClear;       // クリア判定

    unsigned int frame;//フレーム管理用

    // シーン遷移ID
    E_SCENE_ID nextSceneID;

    // 内部処理関数
    void MapScrollProc(void);
    void EraseEnemys(void);
    Vector2 WorldPos2MapPos(Vector2 wpos);
    bool IsCollisionStage(Vector2 worldPos);
    void CollisionCheck(void);
    bool CollisionCheckRectCenter(Vector2 centerPos1, Vector2 size1, Vector2 centerPos2, Vector2 size2);
    void CreateOrbitRing(int numBullets, float radius, float speed, int lifetime);
};



