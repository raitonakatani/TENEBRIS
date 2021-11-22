#pragma once

#include "Level3DRender/LevelRender.h"

//#include "nature/SkyCube.h"


//クラス宣言。
class Player;
class GameCamera;
class BackGround;
class Enemy;
class Enemy2;
class TIMER;
class Fade;
class Door;
class KAGI;
//class SkyCube;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	/// <summary>
	/// ゲームクリアーを通知する。
	/// </summary>
	void NotifyGameClear();
	/// <summary>
	/// ゲームオーバーを通知する。
	/// </summary>
	void NotifyGameOver();
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

private:
	Player*						m_player;						//プレイヤー。
	Door*						m_door;							//ドア。
	SkyCube*					m_skycube;						//スカイキューブ。
	TIMER*						m_TIMER;						//タイマー。
	GameCamera*					m_gameCamera;					//ゲームカメラ。
	BackGround*					m_background;					//ステージ。
	ModelRender					m_modelRender;					//モデルレンダー。
	SpriteRender				m_spriteRender;					//画像。
	FontRender					m_fontRender;					//文字。
	SoundSource*				GameBGM;
	std::vector<Door*>			m_doorVector;					//ドアの配列。
	Vector3						m_position;						//座標。
	LevelRender					m_levelRender;					//レベルレンダー。
	Fade*						m_fade = nullptr;				//フェード。
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
};

