#pragma once
//--------------
//列挙型
//--------------
//シーン定義
enum E_SCENE_ID {
	E_SCENE_NON = -1,
	E_SCENE_TITLE,
	E_SCENE_GAME,
	E_SCENE_GAMEOVER,
	E_SCENE_GAMECLEAR,

	E_SCENE_ID_MAX,
};

// フェードの状態
enum E_FADE_STAT_ID {
	E_STAT_FADE_NON = -1,
	E_STAT_FADE_OUT, // フェードアウト(徐々に暗転する)
	E_STAT_FADE_IN, // フェードイン(徐々に明転する)

	E_STAT_FADE_MAX,
};