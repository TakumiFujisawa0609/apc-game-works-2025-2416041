#include "Slot.h"
#include <DxLib.h>
#include"../Application.h"

Slot::Slot()
    : state_(State::Idle)
    , mode_(Mode::Normal)
    , spinTimer_(0)
    , isSmallHit_(false)
    , isBigHit_(false)
    , isVisible_(false)
    , resultTimer_(0)
    , lastHitType_(HitType::None)
    , hitUsed_(true) 
{
    result_.fill(0);

    for (int i = 0; i < SYMBOL_COUNT; ++i) {
        symbolImg_[i] = -1;
    }
}

Slot::~Slot()
{
}

void Slot::Init()
{
    const char* fileNames[SYMBOL_COUNT] = {
        "slot_7.png",
        "slot_bar.png",
        "slot_bell.png",
        "slot_cherry.png",
    };

    for (int i = 0; i < SYMBOL_COUNT; ++i) {
        // ここで PATH_SLOT と結合する
        std::string path = Application::PATH_SLOT + fileNames[i];
        symbolImg_[i] = LoadGraph(path.c_str());
    }


    // SetRandSeed( /* 適当な値 */ );
    state_ = State::Idle;
    mode_ = Mode::Normal;
    spinTimer_ = 0;
    isSmallHit_ = false;
    isBigHit_ = false;
    result_.fill(0);

    isVisible_ = false;
    resultTimer_ = 0;

    lastHitType_ = HitType::None;
    hitUsed_ = true;

}

void Slot::Start()
{
    // すでに回転中なら無視
    if (state_ == State::Spinning) return;

    // 結果表示中でも、Start されたら新しく回す
    state_ = State::Spinning;
    spinTimer_ = 0;
	resultTimer_ = 0;
	isVisible_ = true;
    isSmallHit_ = false;
    isBigHit_ = false;

    lastHitType_ = HitType::None;
    hitUsed_ = true;

    // 回っている間の見た目用に、とりあえずランダムに回し始める
    for (int i = 0; i < REEL_COUNT; ++i)
    {
        result_[i] = GetRand(SYMBOL_COUNT - 1);
    }
}

void Slot::Update()
{
	if (!isVisible_) return;//表示OFF中は何もしない

    switch (state_)
    {
    case State::Idle:
        // 何もしない（Start待ち）
        break;

    case State::Spinning:
        // 回転中：適当にグルグルさせる
        spinTimer_++;

        for (int i = 0; i < REEL_COUNT; ++i)
        {
            // ぐるぐる回ってる感を出すために、毎フレームランダム
            result_[i] = GetRand(SYMBOL_COUNT - 1);
        }

        // 一定時間経ったら止まる
        if (spinTimer_ >= SPIN_DURATION)
        {
            DecideResult(); // 最終的な出目を決定
            JudgeHit();     // 当たり判定

            state_ = State::Result;
			resultTimer_ = 0;
        }
        break;

    case State::Result:
        resultTimer_++;
        if (resultTimer_ >= RESULT_DISPLAY_TIME) {
            //3秒経過したらスロットを非表示に
            isVisible_ = false;
            state_ = State::Idle;
        }
        break;
	default:
        break;
    }
}

void Slot::Draw(const Vector2& screenPos)
{
    if (!isVisible_) return;

    const int totalWidth = REEL_COUNT * SYMBOL_W + (REEL_COUNT - 1) * SYMBOL_MARGIN;

    int baseCenterX = screenPos.x;
    int baseY = screenPos.y - SYMBOL_H - 16;   // 頭から 1シンボル+少し上に

    // スロット全体の左端（中央揃え）
    int baseX = baseCenterX - totalWidth / 2;

    const unsigned int colWhite = GetColor(255, 255, 255);
    const unsigned int colYellow = GetColor(255, 255, 0);
    const unsigned int colRed = GetColor(255, 0, 0);

    // タイトル
    DrawString(baseX, baseY - 20, "SLOT", colWhite);

    // 各リールのシンボル
    for (int i = 0; i < REEL_COUNT; ++i) {
        int symbolIndex = result_[i];
        if (symbolIndex < 0 || symbolIndex >= SYMBOL_COUNT) continue;

        int handle = symbolImg_[symbolIndex];
        int x = baseX + i * (SYMBOL_W + SYMBOL_MARGIN);
        int y = baseY;

        if (handle != -1) {
            DrawGraph(x, y, handle, TRUE);
        }
        else {
            DrawFormatString(x, y + SYMBOL_H / 2, colWhite, "%d", symbolIndex);
        }
    }

    // 状態表示（シンボルのすぐ下あたりに）
    int stateY = baseY + SYMBOL_H + 4;
    switch (state_)
    {
    case State::Idle:
        DrawString(baseX, stateY, "IDLE", colWhite);
        break;
    case State::Spinning:
        DrawString(baseX, stateY, "SPINNING...", colWhite);
        break;
    case State::Result:
        DrawString(baseX, stateY, "RESULT", colWhite);
        break;
    }

    // 当たり表示
    int hitY = stateY + 20;
    if (isBigHit_) {
        DrawString(baseX, hitY, "大当たり", colRed);
    }
    else if (isSmallHit_) {
        DrawString(baseX, hitY, "小当たり", colYellow);
    }

    // 確変中表示
    if (mode_ == Mode::Kakuhen)
    {
        DrawString(baseX, hitY + 20, "確変", colYellow);
    }
}
void Slot::Relese()
{
}
bool Slot::IsSpinning() const
{
    return state_ == State::Spinning;
}

bool Slot::IsResult() const
{
    return state_ == State::Result;
}

std::array<int, 3> Slot::GetResult() const
{
    return result_;
}

bool Slot::IsAnyHit() const
{
    return isSmallHit_ || isBigHit_;
}

bool Slot::IsSmallHit() const
{
    return isSmallHit_;
}

bool Slot::IsBigHit() const
{
    return isBigHit_;
}

bool Slot::IsInKakuhen() const
{
    return mode_ == Mode::Kakuhen;
}

Slot::HitType Slot::FetchHitType()
{
    if (state_ == State::Result && !hitUsed_)
    {
        hitUsed_ = true;
        return lastHitType_;
    }
    return HitType::None;
}

// 最終的な出目を決める
void Slot::DecideResult()
{
    // ここをいじれば「確変中は当たりやすくする」などの拡張がしやすい
    for (int i = 0; i < REEL_COUNT; ++i)
    {
        // 通常は完全ランダム
        // 確変中は重みを変える、などもここでできるようにしておく
        result_[i] = GetRand(SYMBOL_COUNT - 1);
    }
}

// 当たり判定
void Slot::JudgeHit()
{
    isSmallHit_ = false;
    isBigHit_ = false;
    lastHitType_ = HitType::None;
    hitUsed_ = false; 

    int a = result_[0];
    int b = result_[1];
    int c = result_[2];

    if (a == b && b == c)
    {
        // 3つ揃い → 大当たり
        isBigHit_ = true;

        lastHitType_ = HitType::Big;

        // 将来の確変のためのフラグ
        mode_ = Mode::Kakuhen;
    }
    else if (a == b || b == c || a == c)
    {
        // どこか2つだけ揃い → 小当たり
        isSmallHit_ = true;
        lastHitType_ = HitType::Small;
    }
    else
    {
        // ハズレ
    }
}