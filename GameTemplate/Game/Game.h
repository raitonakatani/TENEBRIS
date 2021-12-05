#pragma once

#include "Level3DRender/LevelRender.h"

//#include "nature/SkyCube.h"


//クラス宣言。
class Player;
class fastPlayer;
class GameCamera;
class CAMERA;
class BackGround;
class BackGround2;
class Enemy;
class Enemy2;
class TIMER;
class Fade;
class warp;
class Door;
class Menu;
class ClearCounter;
class Portion;
class Fire;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	/// <summary>
	/// エネミーを全滅させた？
	/// </summary>
	/// <returns>エネミーが全滅していたらtrue。</returns>
	const bool IsWannihilationEnemy() const
	{
		if (m_numEnemy == 0)
		{
			return false;
		}
		return m_numEnemy == m_numDefeatedEnemy;
	}
	/// <summary>
	/// 倒された敵の数を+1する。
	/// </summary>
	void AddDefeatedEnemyNumber()
	{
		m_numDefeatedEnemy++;
	}
	//倒したエネミー数
	void SetnumEnemydesu()
	{
		m_sibouEnemy++;
	}

	int GetnumEnemydesu()
	{
		return m_sibouEnemy;
	}
	//エネミー数
	int GetnumEnemy() 
	{
		return m_numEnemy;
	}
	//ステート管理。
	//void ManageState();
	void Render(RenderContext& rc);

	bool IsMove() const
	{
		return m_menu == true;
	}


private:
	Player*						m_player;						//プレイヤー。
	fastPlayer* m_fastplayer;
	Fire* m_fire;
	Door*						m_door;							//ドア。
	SkyCube*					m_skycube;						//スカイキューブ。
	TIMER*						m_TIMER;						//タイマー。
	GameCamera* m_gameCamera = nullptr;					//ゲームカメラ。
	CAMERA* m_camera;
	BackGround*					m_background;					//ステージ。
	BackGround2*				m_background2;					//ステージ。
	ModelRender					m_modelRender;					//モデルレンダー。
	SpriteRender				m_spriteRender;					//画像。
	FontRender					m_fontRender;					//文字。
	SoundSource*				GameBGM;
	std::vector<Door*>			m_doorVector;					//ドアの配列。
	Vector3						m_position;						//座標。
	LevelRender					m_levelRender;					//レベルレンダー。
	Fade*						m_fade = nullptr;				//フェード。
	Menu* menu=nullptr;
	warp* m_Warp;
	ClearCounter* clearCounter=nullptr;
	Portion* portion;
	bool						m_isWaitFadeout = false;
	bool						m_menu = false;
	int							m_sibouEnemy = 5;
	int							m_numEnemy = 2;					//エネミーの数。
	int							m_numDefeatedEnemy = 0;			//倒されたエネミーの数。
	float						a = 0.0f;						//拡大率
	float						t = 30.0f;						//タイマー。
	float alpha = 0.002f;

	bool sibou = false;
	float siboutimer = 0.0f;

	bool WARP = false;

	bool KISI = false;
	bool MAHOU = false;
	bool m_BOSS = false;
	bool DOOR = false;
};

