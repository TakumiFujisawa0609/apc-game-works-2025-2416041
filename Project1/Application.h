#pragma once
#include <string>
class SceneManager;

class Application
{
public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 800; // ゲーム画面の横サイズ
	static constexpr int SCREEN_SIZE_Y = 600; // ゲーム画面の縦サイズ

	static constexpr float FRAME_RATE = 1000.0f / 60.0f;

	int currentTime = 0;
	int lastFrameTime = 0;

	int frameCnt = 0;
	int updateFrameRateTime = 0;

	float frameRate = 0.0f;

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_DATA;
	static const std::string PATH_OBJECT;
	static const std::string PATH_SCENE;
	static const std::string PATH_STAGE;
	static const std::string PATH_UI;
	static const std::string PATH_IMAGE;
	static const std::string PATH_SOUND;
	static const std::string PATH_MOVIE; // 動画のパス（必要なら追加）
	//static const std::string PATH_EFFECT;
	//static const std::string PATH_MAP_DATA;
	//------------------------------------------

	Application(void); // コンストラクタ
	~Application(void); // デストラクタ
	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void Run(void); // ゲーム起動
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)

private:

	SceneManager* sceneMana; // シーンマネージャーのインスタンスのポインタ
};

