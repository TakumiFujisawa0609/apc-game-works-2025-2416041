#include "Slot.h"
#include <DxLib.h>
#include"../Application.h"

Slot::Slot()
    : state_(State::Idle)
    , mode_(Mode::Normal)
    , spinTimer_(0)
    , isSmallHit_(false)
    , isBigHit_(false)
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
        }
        break;

    case State::Result:
        // 結果を表示しているだけ（外側で Start されるまでこのまま）
        break;
    }
}

void Slot::Draw()
{
    // 簡易表示（あとで画像に差し替えてOK）
    // 位置は適当に調整
    const int baseX = 600;
    const int baseY = 40;

    const unsigned int colWhite = GetColor(255, 255, 255);
    const unsigned int colYellow = GetColor(255, 255, 0);
    const unsigned int colRed = GetColor(255, 0, 0);

    DrawString(baseX, baseY - 20, "SLOT", colWhite);

    // 出目（数字）を表示
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
            // 画像読み込み失敗時の保険：数字で表示
            DrawFormatString(x, y + SYMBOL_H / 2, colWhite, "%d", symbolIndex);
        }
    }

    // 状態表示
    switch (state_)
    {
    case State::Idle:
        DrawString(baseX, baseY + 10, "IDLE", colWhite);
        break;
    case State::Spinning:
        DrawString(baseX, baseY + 10, "SPINNING...", colWhite);
        break;
    case State::Result:
        DrawString(baseX, baseY + 10, "RESULT", colWhite);
        break;
    }

    // 当たり表示
    if (isBigHit_)
    {
        DrawString(baseX, baseY + 30, "大当たり", colRed);
    }
    else if (isSmallHit_)
    {
        DrawString(baseX, baseY + 30, "小当たり", colYellow);
    }

    // 確変中表示
    if (mode_ == Mode::Kakuhen)
    {
        DrawString(baseX, baseY + 50, "確変", colYellow);
    }
}
void Slot::Relese()
{
    // 画像解放
    for (int i = 0; i < SYMBOL_COUNT; ++i) {
        if (symbolImg_[i] != -1) {
            DeleteGraph(symbolImg_[i]);
            symbolImg_[i] = -1;
        }
    }


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

    int a = result_[0];
    int b = result_[1];
    int c = result_[2];

    if (a == b && b == c)
    {
        // 3つ揃い → 大当たり
        isBigHit_ = true;

        // 将来の確変のためのフラグ
        mode_ = Mode::Kakuhen;
    }
    else if (a == b || b == c || a == c)
    {
        // どこか2つだけ揃い → 小当たり
        isSmallHit_ = true;
    }
    else
    {
        // ハズレ
    }
}