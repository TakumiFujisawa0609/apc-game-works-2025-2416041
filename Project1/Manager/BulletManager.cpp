#include "BulletManager.h"
#include <algorithm>
#include <cmath>

BulletManager::BulletManager(GameScene* gs) : gInst(gs) {}
BulletManager::~BulletManager() {
    Release();
    for (auto b : normalBullets) delete b;
    for (auto b : orbitBullets) delete b;
    for (auto b : rainBullets) delete b;
    for (auto b : diffusionBullets) delete b;
}

bool BulletManager::SystemInit() {
    auto createBullets = [this](std::vector<Bullet*>& vec, int count) {
        for (int i = 0; i < count; ++i) {
            Bullet* b = new Bullet(gInst);
            b->SystemInit();
            vec.push_back(b);
        }
        };

    createBullets(normalBullets, MAX_NORMAL);
    createBullets(orbitBullets, MAX_ORBIT);
    createBullets(rainBullets, MAX_RAIN);

    return true;
}

void BulletManager::GameInit() {
    auto initVec = [](std::vector<Bullet*>& vec) {
        for (auto b : vec) b->GameInit();
        };
    initVec(normalBullets);
    initVec(orbitBullets);
    initVec(rainBullets);
    initVec(diffusionBullets);
}

void BulletManager::Update() {
    auto updateVec = [](std::vector<Bullet*>& vec) {
        for (auto b : vec) b->Update();
        };
    updateVec(normalBullets);
    updateVec(orbitBullets);
    updateVec(rainBullets);
    updateVec(diffusionBullets);

    // I—¹‚µ‚½’e‚ðíœidiffusion‚Ì‚Ý–³§ŒÀ‚È‚Ì‚Å‚±‚±‚Åj
    diffusionBullets.erase(
        std::remove_if(diffusionBullets.begin(), diffusionBullets.end(),
            [](Bullet* b) {
                if (b->IsEnableCreate()) {
                    b->Release();
                    delete b;
                    return true;
                }
                return false;
            }),
        diffusionBullets.end()
    );
}

void BulletManager::Draw() {
    auto drawVec = [](std::vector<Bullet*>& vec) {
        for (auto b : vec) b->Draw();
        };
    drawVec(normalBullets);
    drawVec(orbitBullets);
    drawVec(rainBullets);
    drawVec(diffusionBullets);
}

void BulletManager::Release() {
    auto releaseVec = [](std::vector<Bullet*>& vec) {
        for (auto b : vec) b->Release();
        };
    releaseVec(normalBullets);
    releaseVec(orbitBullets);
    releaseVec(rainBullets);
    releaseVec(diffusionBullets);
}

Bullet* BulletManager::GetFreeBullet(std::vector<Bullet*>& vec, int maxCount) {
    for (auto b : vec) {
        if (b->IsEnableCreate()) return b;
    }
    if (vec.size() < maxCount) {
        Bullet* b = new Bullet(gInst);
        b->SystemInit();
        vec.push_back(b);
        return b;
    }
    return nullptr;
}

void BulletManager::CreateNormal(Vector2F pos, Vector2F dir) {
    Bullet* b = GetFreeBullet(normalBullets, MAX_NORMAL);
    if (!b) return;
    b->bPos = pos;
    b->bVec = dir;
    b->ChangeStatus(Bullet::STATUS::E_STAT_MOVE);
}

void BulletManager::CreateOrbit(Vector2F center, float rad, float speed, int time) {
    Bullet* b = GetFreeBullet(orbitBullets, MAX_ORBIT);
    if (!b) return;
    b->CreateOrbit(center, rad, speed, time);
}

void BulletManager::CreateRain(Vector2F center, float rad, float speed, int time) {
    Bullet* b = GetFreeBullet(rainBullets, MAX_RAIN);
    if (!b) return;
    b->CreateRain(center, rad, speed, time);
}

void BulletManager::CreateDiffusion(Vector2F pos, AsoUtility::DIRECTION dir, float spreadAngle, float speed) {
    Bullet* b = new Bullet(gInst);
    b->SystemInit();
    b->GameInit();
    b->CreateDiffusionTripleDir(pos, dir, spreadAngle, speed);
    diffusionBullets.push_back(b);
}

void BulletManager::CheckCollisionWithEnemies(std::vector<Enemy*>& enemys) {
    auto allBullets = [&]() -> std::vector<Bullet*> {
        std::vector<Bullet*> tmp;
        tmp.insert(tmp.end(), normalBullets.begin(), normalBullets.end());
        tmp.insert(tmp.end(), orbitBullets.begin(), orbitBullets.end());
        tmp.insert(tmp.end(), rainBullets.begin(), rainBullets.end());
        tmp.insert(tmp.end(), diffusionBullets.begin(), diffusionBullets.end());
        return tmp;
        };

    auto bulletsVec = allBullets();

    for (auto& e : enemys) {
        if (!e->GetAlive()) continue;
        Vector2 ePos = AsoUtility::Round(e->GetEnemyPos());
        Vector2 eSize = e->GetEnemySize();

        for (auto& b : bulletsVec) {
            if (!b->IsShotState()) continue;
            Vector2 bPos = AsoUtility::Round(b->GetBulletPos());
            Vector2 bSize = { Bullet::BULLET_SIZE_WID, Bullet::BULLET_SIZE_HIG };
            if (CollisionCheckRectCenter(bPos, bSize, ePos, eSize)) {
                e->SetDamege(4);
                b->BlastOn(b->GetBulletPos());
            }
        }
    }
}

static bool CollisionCheckRectCenter(Vector2 centerPos1, Vector2 size1, Vector2 centerPos2, Vector2 size2) {
    Vector2 stPos1 = centerPos1, edPos1 = centerPos1;
    stPos1.x -= size1.x / 2; stPos1.y -= size1.y / 2;
    edPos1.x += size1.x / 2; edPos1.y += size1.y / 2;

    Vector2 stPos2 = centerPos2, edPos2 = centerPos2;
    stPos2.x -= size2.x / 2; stPos2.y -= size2.y / 2;
    edPos2.x += size2.x / 2; edPos2.y += size2.y / 2;

    return stPos1.x < edPos2.x && edPos1.x > stPos2.x &&
        stPos1.y < edPos2.y && edPos1.y > stPos2.y;
}