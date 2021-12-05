#pragma once

#include "Level3DRender/LevelRender.h"

//#include "nature/SkyCube.h"


//�N���X�錾�B
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
	/// �G�l�~�[��S�ł������H
	/// </summary>
	/// <returns>�G�l�~�[���S�ł��Ă�����true�B</returns>
	const bool IsWannihilationEnemy() const
	{
		if (m_numEnemy == 0)
		{
			return false;
		}
		return m_numEnemy == m_numDefeatedEnemy;
	}
	/// <summary>
	/// �|���ꂽ�G�̐���+1����B
	/// </summary>
	void AddDefeatedEnemyNumber()
	{
		m_numDefeatedEnemy++;
	}
	//�|�����G�l�~�[��
	void SetnumEnemydesu()
	{
		m_sibouEnemy++;
	}

	int GetnumEnemydesu()
	{
		return m_sibouEnemy;
	}
	//�G�l�~�[��
	int GetnumEnemy() 
	{
		return m_numEnemy;
	}
	//�X�e�[�g�Ǘ��B
	//void ManageState();
	void Render(RenderContext& rc);

	bool IsMove() const
	{
		return m_menu == true;
	}


private:
	Player*						m_player;						//�v���C���[�B
	fastPlayer* m_fastplayer;
	Fire* m_fire;
	Door*						m_door;							//�h�A�B
	SkyCube*					m_skycube;						//�X�J�C�L���[�u�B
	TIMER*						m_TIMER;						//�^�C�}�[�B
	GameCamera* m_gameCamera = nullptr;					//�Q�[���J�����B
	CAMERA* m_camera;
	BackGround*					m_background;					//�X�e�[�W�B
	BackGround2*				m_background2;					//�X�e�[�W�B
	ModelRender					m_modelRender;					//���f�������_�[�B
	SpriteRender				m_spriteRender;					//�摜�B
	FontRender					m_fontRender;					//�����B
	SoundSource*				GameBGM;
	std::vector<Door*>			m_doorVector;					//�h�A�̔z��B
	Vector3						m_position;						//���W�B
	LevelRender					m_levelRender;					//���x�������_�[�B
	Fade*						m_fade = nullptr;				//�t�F�[�h�B
	Menu* menu=nullptr;
	warp* m_Warp;
	ClearCounter* clearCounter=nullptr;
	Portion* portion;
	bool						m_isWaitFadeout = false;
	bool						m_menu = false;
	int							m_sibouEnemy = 5;
	int							m_numEnemy = 2;					//�G�l�~�[�̐��B
	int							m_numDefeatedEnemy = 0;			//�|���ꂽ�G�l�~�[�̐��B
	float						a = 0.0f;						//�g�嗦
	float						t = 30.0f;						//�^�C�}�[�B
	float alpha = 0.002f;

	bool sibou = false;
	float siboutimer = 0.0f;

	bool WARP = false;

	bool KISI = false;
	bool MAHOU = false;
	bool m_BOSS = false;
	bool DOOR = false;
};

