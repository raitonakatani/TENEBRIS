#pragma once

//�N���X�錾�B
//class Lever;
class Collision;
class Game;

/// <summary>
/// �v���C���[�B
/// </summary>
class Player : public IGameObject
{
public:
	//�v���C���[�̃X�e�[�g�B
	enum EnPlayerState {
		enPlayerState_Idle,					//�ҋ@�B
		enPlayerState_Run,					//����B
		enPlayerState_Walk,					//����B
		enPlayerState_ReceiveDamage,			//��_���[�W�B
		enPlayerState_Attack,				//�U���B
		enPlayerState_Attack2				//�U���B
	};
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// �傫����ݒ�B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// ��������(�X�e�[�g)���ǂ������擾�B
	/// </summary>
	/// <returns>��������(�X�e�[�g)�Ȃ�true�B</returns>
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Attack;
	}

	bool IsEnableMove2() const
	{
		return m_playerState != enPlayerState_ReceiveDamage;
	}

	bool IsEnableMove3() const
	{
		return m_playerState != enPlayerState_Attack2;
	}

	void SetPlayerHP(float)
	{
		m_hp;
	}

	int GetPlayerHP()
	{
		return m_hp;
	}

	void PlusKAGI()
	{
		m_kagi++;
	}

	int GetKAGI()
	{
		return m_kagi;
	}

	void PlusBOSS()
	{
		m_boss++;
	}

	int GetBOSS()
	{
		return m_boss;
	}

	int level = 1;

private:
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �U�����̏����B
	/// </summary>
	void Attack();
	/// <summary>
	/// <summary>
	/// �v���C���[�̍U���Ƃ̓����蔻�菈���B
	/// </summary>
	void Collision();
	/// <summary>
	/// �U���̍ۂ̓����蔻��p�̃R���W�������쐬����B
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// ���o�[���������́A�����蔻��p�̃R���W�������쐬����B
	/// </summary>
	void MakePushLeverCollision();
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
	/// ����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	 /// �����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ����B
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	void ProcessAttackStateTransition();
	/// <summary>
	void ProcessAttackState2Transition();
	/// <summary>
	// �A�j���[�V�����C�x���g�p�̊֐��B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	
	//�A�j���[�V�����B
	enum EnAnimationClip {
		enAnimationClip_Idle,				//�ҋ@�A�j���[�V�����B	
		enAnimationClip_Run,				//����A�j���[�V�����B
		enAnimationClip_Walk,				//����A�j���[�V�����B
		enAnimationClip_Attack,				//�U���A�j���[�V�����B
		enAnimationClip_Attack2,				//�U���A�j���[�V����2�B
		enAnimationClip_Damage,				//��_���[�W�A�j���[�V�����B
		enAnimationClip_Num,				//�A�j���[�V�����̐��B
	};
	AnimationClip			m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender				m_modelRender;								//���f�������_�\�B
	Vector3					m_position;									//���W�B
	Vector3					m_moveSpeed;								//�ړ����x�B
	Vector3					m_forward = Vector3::AxisZ;					//�v���C���[�̐��ʃx�N�g���B
	Vector3					m_scale;									//�傫���B
	Quaternion				m_rotation;									//�N�H�[�^�j�I���B
	CharacterController		m_charaCon;									//�L�����N�^�[�R���g���[���[�B
	EnPlayerState			m_playerState = enPlayerState_Idle;			//�v���C���[�X�e�[�g�B
	SpriteRender			m_spriteRender;						        //�摜�B
	FontRender				m_fontRender;									//�����@HP�̕\���B
	FontRender				m_fontRender2;					            //�����@Level�̕\���B
	FontRender				m_fontRender3;					            //�����@���̏������̕\���B
	bool					m_isUnderAttack = false;					//�U�����Ȃ�true�B
	bool					m_isUnderAttack2 = false;					//�U�����Ȃ�true�B
	float						m_hp = 100.0f;							//�v���C���[��HP�B
	Game*					m_game = nullptr;							//�Q�[���B
	int                     m_sword_jointBoneId = -1;                   //�uSword�v�{�[����ID�B
	float					life = 100.0f;								//HP�o�[
	float					m_timer = 0.0f;								//�^�C�}�[�B
	int						m_kagi = 0;									//��
	int						m_boss = 0;									//�{�X�B

	bool dash = false;
	float timer = 0.0f;
};

