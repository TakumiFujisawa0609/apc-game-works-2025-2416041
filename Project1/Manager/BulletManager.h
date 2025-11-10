#pragma once
#include <vector>
#include "../Object/Bullet.h"
#include "../Scene/GameScene.h"

class BulletManager
{
public:
    static const int MAX_NORMAL = 128;
    static const int MAX_ORBIT = 16;
    static const int MAX_RAIN = 16;

    BulletManager(GameScene* gs);
    ~BulletManager();

    bool SystemInit();
    void GameInit();
    void Update();
    void Draw();
    void Release();

    // íeê∂ê¨
    void CreateNormal(Vector2F pos, Vector2F dir);
    void CreateOrbit(Vector2F center, float rad, float speed, int time);
    void CreateRain(Vector2F center, float rad, float speed, int time);
    void CreateDiffusion(Vector2F pos, AsoUtility::DIRECTION dir, float spreadAngle, float speed);

    // ìGÇ∆ÇÃè’ìÀîªíË
    void CheckCollisionWithEnemies(std::vector<Enemy*>& enemys);
private:
    GameScene* gInst;

    std::vector<Bullet*> normalBullets;
    std::vector<Bullet*> orbitBullets;
    std::vector<Bullet*> rainBullets;
    std::vector<Bullet*> diffusionBullets;

    Bullet* GetFreeBullet(std::vector<Bullet*>& vec, int maxCount);
};



