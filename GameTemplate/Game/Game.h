#pragma once

#include "Level3DRender/LevelRender.h"

//#include "nature/SkyCube.h"


//�N���X�錾�B
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
	/// �Q�[���N���A�[��ʒm����B
	/// </summary>
	void NotifyGameClear();
	/// <summary>
	/// �Q�[���I�[�o�[��ʒm����B
	/// </summary>
	void NotifyGameOver();
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

private:
	Player*						m_player;						//�v���C���[�B
	Door*						m_door;							//�h�A�B
	SkyCube*					m_skycube;						//�X�J�C�L���[�u�B
	TIMER*						m_TIMER;						//�^�C�}�[�B
	GameCamera*					m_gameCamera;					//�Q�[���J�����B
	BackGround*					m_background;					//�X�e�[�W�B
	ModelRender					m_modelRender;					//���f�������_�[�B
	SpriteRender				m_spriteRender;					//�摜�B
	FontRender					m_fontRender;					//�����B
	SoundSource*				GameBGM;
	std::vector<Door*>			m_doorVector;					//�h�A�̔z��B
	Vector3						m_position;						//���W�B
	LevelRender					m_levelRender;					//���x�������_�[�B
	Fade*						m_fade = nullptr;				//�t�F�[�h�B
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
};

