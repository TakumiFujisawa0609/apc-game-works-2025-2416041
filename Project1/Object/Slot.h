#pragma once
#include <array>

// DxLib を使う前提のスロットクラス
class Slot
{
public:
    Slot();
    ~Slot();

    // 最初に一回呼ぶ
    void Init();
    // 毎フレーム呼ぶ
    void Update();
    // 毎フレーム呼ぶ（描画）
    void Draw();
    // 解放
    void Relese();

    // スロットを回し始める
    void Start();

    // スロットが回転中か？
    bool IsSpinning() const;
    // 結果待ち状態か？（止まって結果が出ている）
    bool IsResult() const;

    // 出目を取得（0?SYMBOL_COUNT-1 の数字）
    std::array<int, 3> GetResult() const;

    // 当たり系
    bool IsAnyHit() const;      // 小当たり or 大当たり
    bool IsSmallHit() const;    // 小当たり
    bool IsBigHit() const;      // 大当たり

    // 確変モード中か？
    bool IsInKakuhen() const;

private:
    // スロットの状態
    enum class State
    {
        Idle,      // 待機（Start待ち）
        Spinning,  // 回転中
        Result     // 止まって結果表示中
    };

    // 通常 or 確変
    enum class Mode
    {
        Normal,
        Kakuhen,
    };

    // 内部処理
    void DecideResult();  // 最終的な出目を決める
    void JudgeHit();      // 当たり判定を行う

    // 定数
    static constexpr int REEL_COUNT = 3; // リール数
    static constexpr int SYMBOL_COUNT = 4; // シンボルの種類数（0?3）
    static constexpr int SPIN_DURATION = 60; // 何フレーム回すか

    // 描画用（シンボル画像サイズと間隔）
    static constexpr int SYMBOL_W = 64;  // 画像幅（実際の画像に合わせて調整）
    static constexpr int SYMBOL_H = 64;  // 画像高さ
    static constexpr int SYMBOL_MARGIN = 8; // リール間の隙間


    State state_;
    Mode  mode_;

    std::array<int, REEL_COUNT> result_;   // 出目（各リールのシンボル番号）

    int spinTimer_;    // 何フレーム回っているか

    bool isSmallHit_;
    bool isBigHit_;

    // シンボルごとの画像ハンドル
    int symbolImg_[SYMBOL_COUNT];
};

