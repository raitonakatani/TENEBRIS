#pragma once

//�N���X�錾�B
class Player;
class Game;

/// <summary>
/// �G�l�~�[�B
/// </summary>
class Enemy2 : public IGameObject
{
public:
	//�G�l�~�[�X�e�[�g�B
	enum enEnemy2State {
		enEnemy2State_Idle,					//�ҋ@�B
		enEnemy2State_Chase,					//�ǐՁB
		enEnemy2State_MagicAttack,				//�U���B
		enEnemy2State_ReceiveDamage,			//��_���[�W�B
		enEnemy2State_Down,					//�_�E���B
		enEnemy2State_model
	};
public:
	Enemy2();
	~Enemy2();
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
	/// ���@�g���̔ԍ���ݒ肷��B
	/// </summary>
	/// <param name="wizardNumber">	���@�g���̔ԍ��B</param>
	void SetwizardNumber(const int wizardNumber)
	{
		m_wizardNumber = wizardNumber;
	}
	/// <summary>
	/// �h�A�̔ԍ����擾����B
	/// </summary>
	/// <returns>�h�A�̔ԍ��B</returns>
	const int GetwizardNumber() const
	{
		return m_wizardNumber;
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
	/// �v���C���[�̍U���Ƃ̓����蔻�菈���B
	/// </summary>
	void Collision();
	/// <summary>
	/// �v���C���[��T������B
	/// </summary>
	/// <returns>�v���C���[������������true�B</returns>
	const bool SearchPlayer() const;
	/// <summary>
	/// �t�@�C���[�{�[�����쐬����B
	/// </summary>
	void MakeFireBall();
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
	/// �ǐՃX�e�[�g�̔w�J�ڏ����B
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// ���@�U���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessMagicAttackStateTransition();
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
		enAnimationClip_MagicAttack,			//�U���A�j���[�V�����B
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
	EffectEmitter*				m_effectEmitter = nullptr;					//�G�t�F�N�g�B
	enEnemy2State				m_enemy2State = enEnemy2State_Idle;			//�G�l�~�[�X�e�[�g�B
	Game*						m_game = nullptr;
	SpriteRender				m_spriteRender;								//�摜�B
	bool						m_isUnderAttack = false;					//�U�������H
	float						m_hp = 3;									//HP�B
	int							m_swordBoneId = -1;							//���̃{�[����ID�B
	Player*						m_player = nullptr;							//�v���C���[�B
	float						m_chaseTimer = 0.0f;						//�ǐՃ^�C�}�[�B
	float						m_idleTimer = 0.0f;							//�ҋ@�^�C�}�[�B
	float						m_sibouTimer = 0.0f;						//�ҋ@�^�C�}�[�B
	float						a = 0.0f;			//�g�嗦
	bool						m_isShowHPBar = false;
	bool model = false;
	float modeltimer = 0.0f;
	int m_model = 0;
	bool sibou = false;
	int karyoku = 1;
	int m_wizardNumber = 0;
};