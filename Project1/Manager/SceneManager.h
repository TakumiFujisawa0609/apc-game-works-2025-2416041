#include "../StDefineData.h"
class Fader;
class TitleScene;
class GameScene;
class GameOverScene;
class GameClear;
class SceneManager
{
public:
	SceneManager(void); // コンストラクタ
	~SceneManager(void); // デストラクタ
	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void GameInit(void); // ゲーム起動・再開時に必ず呼び出す処理
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)

	bool IsGameEnd() const { return isGameEnd; }
private:
	Fader* fader; // フェードクラスのインスタンスのポインタ
	TitleScene* titleInst; // タイトルシーンクラスのインスタンスのポインタ
	GameScene* gameInst; // ゲームシーンクラスのインスタンスのポインタ
	GameOverScene* gameover; // ゲームオーバーシーンクラスのインスタンスのポインタ
	GameClear* gameclear; // ゲームクリアシーンクラスのインスタンスのポインタ

	E_SCENE_ID scene_ID; // 現在のシーンID
	E_SCENE_ID waitScene; // シーンチェンジで次に遷移するシーンのID
	bool sceneChangeFlg; // シーンチェンジ実行中フラグ

	// シーン遷移処理
	bool ChangeScene(E_SCENE_ID id);
	void ReleaseScene(E_SCENE_ID id);

	enum class ON_OFF
	{
		GAME,
		PAUSE
	};

	enum class PAUSE_SELECT
	{
		RESUME = 0,   // ゲームに戻る
		EXIT_GAME,    // ゲーム終了
		MAX           // 個数
	};

	ON_OFF gamePause;
	PAUSE_SELECT pauseSelect;  
	bool isGameEnd;    

	void DrawPauseMenu();
};