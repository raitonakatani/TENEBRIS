#pragma once

//�N���X�錾�B
class Player;
class Game;

/// <summary>
/// �G�l�~�[�B
/// </summary>
class Kisi : public IGameObject
{
public:
	//�G�l�~�[�X�e�[�g�B
	enum EnEnemyState {
		enEnemyState_Idle,					//�ҋ@�B
		enEnemyState_Chase,					//�ǐՁB
		enEnemyState_Attack,				//�U���B
		enEnemyState_ReceiveDamage,			//��_���[�W�B
		enEnemyState_Down,					//�_�E���B
		enEnemyState_model
	};
public:
	Kisi();
	~Kisi();
	bool Start();
	void Update();
	void MODEL();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// HP��ݒ肷��B
	/// </summary>
	/// <param name="hp">HP�B</param>
	void SetHP(const float hp)
	{
		m_hp = hp;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �R�m�̔ԍ���ݒ肷��B
	/// </summary>
	/// <param name="kisiNumber">�R�m�̔ԍ��B</param>
	void SetkisiNumber(const int kisiNumber)
	{
		m_kisiNumber = kisiNumber;
	}
	/// <summary>
	/// �R�m�̔ԍ����擾����B
	/// </summary>
	/// <returns>�R�m�̔ԍ��B</returns>
	const int GetkisiNumber() const
	{
		return m_kisiNumber;
	}



private:
	/// <summary>
	/// �ǐՏ����B
	/// </summary>
	void Chase();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �U�������B
	/// </summary>
	void Attack();
	/// <summary>
	/// �v���C���[�̍U���Ƃ̓����蔻�菈���B
	/// </summary>
	void Collision();
	/// <summary>
	/// �v���C���[��T������B
	/// </summary>
	/// <returns>�v���C���[������������true�B</returns>
	const bool SearchPlayer() const;
	/// <summary>
	/// �U���p�̓����蔻��R���W�������쐬����B
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// �A�j���[�V�����̍Đ��B
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ManageState();
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// �ǐՃX�e�[�g�̔w�J�ڏ����B
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// �U���ł��鋗�����ǂ������ׂ�B
	/// </summary>
	/// <returns>�U���ł���Ȃ�true�B</returns>
	const bool IsCanAttack() const;
	/// <summary>
   /// �A�j���[�V�����C�x���g�p�̊֐��B
   /// </summary>
   /// <param name="clipName">�A�j���[�V�����̖��O�B</param>
   /// <param name="eventName">�A�j���[�V�����C�x���g�̃L�[�̖��O�B</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	enum EnAnimationClip {						//�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Walk,					//�����A�j���[�V�����B
		enAnimationClip_Attack,					//�U���A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Down,					//�_�E���A�j���[�V�����B
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender					m_modelRender;								//���f�������_�[�B
	Vector3						m_position;									//���W�B
	Vector3						m_moveSpeed;								//�ړ����x�B
	Vector3						m_forward = Vector3::AxisZ;					//�G�l�~�[�̐��ʃx�N�g���B
	Quaternion					m_rotation;									//��]�B
	Vector3						m_scale = Vector3(3.0f, 3.0f, 3.0f);		//�傫���B
	CharacterController			m_charaCon;									//�L�����R���B
	EnEnemyState				m_enemyState = enEnemyState_Idle;			//�G�l�~�[�X�e�[�g�B
	EffectEmitter* m_effectEmitter = nullptr;			//�G�t�F�N�g�B
	Game* m_game = nullptr;
	SpriteRender				m_spriteRender;								//�摜�B
	bool						m_isUnderAttack = false;					//�U�������H
	float							m_hp = 1;									//HP�B
	int						  m_swordBoneId = -1;                 //���̃{�[����ID�B
	Player* m_player = nullptr;							//�v���C���[�B
	float						m_chaseTimer = 0.0f;						//�ǐՃ^�C�}�[�B
	float						m_idleTimer = 0.0f;							//�ҋ@�^�C�}�[�B
	float modeltimer = 0.0f;
	float						a = 0.0f;			//�g�嗦
	bool						m_isShowHPBar = false;
	bool model = false;
	int karyoku = 1;
	int m_kisiNumber = 0;
	int m_model = 0;
};