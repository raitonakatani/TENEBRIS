#pragma once

	//�N���X�錾�B
	//class Lever;
	class Collision;
	class Game;
	class WarpCounter;

	/// <summary>
	/// �v���C���[�B
	/// </summary>
	class fastPlayer : public IGameObject
	{
	public:
		//�v���C���[�̃X�e�[�g�B
		enum EnPlayerState {
			enPlayerState_Idle,					//�ҋ@�B
			enPlayerState_Run,					//����B
			enPlayerState_Walk,					//����B
			enPlayerState_ReceiveDamage,		//��_���[�W�B
			enPlayerState_Attack,				//�U���B
		};
	public:
		fastPlayer();
		~fastPlayer();
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
		void SetScale(const Vector3& scale)
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
		/// <summary>
		/// ��������(�X�e�[�g)���ǂ������擾�B
		/// </summary>
		/// <returns>��������(�X�e�[�g)�Ȃ�true�B</returns>
		bool IsEnableMove2() const
		{
			return m_playerState != enPlayerState_ReceiveDamage;
		}
		/// <summary>
		/// HP��ݒ�
		/// </summary>
		/// <param name="hp">HP�B</param>
		void SetPlayerHP(const float& hp)
		{
			m_hp = hp;
		}
		/// <summary>
		/// HP���擾�B
		/// </summary>
		/// <param name="hp">HP�B</param>
		const float& GetPlayerHP()const
		{
			return m_hp;
		}

		/// <summary>
		/// �|�[�V�������v���X����B
		/// </summary>
		void PlusPortion()
		{
			m_portion++;
		}
		/// <summary>
		/// �|�[�V�������擾����B
		/// </summary>
		/// <returns>�|�[�V�����B</returns>
		int GetPortion()
		{
			return m_portion;
		}

		void PlusBOSS()
		{
			m_boss++;
		}

		int GetBOSS()
		{
			return m_boss;
		}

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
		/// �v���C���[�̍U���Ƃ̓����蔻�菈���B
		/// </summary>
		void Collision();
		/// <summary>
		/// �U���̍ۂ̓����蔻��p�̃R���W�������쐬����B
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
		void ProcessAttackStateTransition();
		/// </summary>
		/// ��_���[�W�X�e�[�g�̑J�ڏ����B
		/// </summary>
		void ProcessDamageStateTransition();
		/// <summary>
	//	void ProcessAttackState2Transition();
		/// <summary>
		// �A�j���[�V�����C�x���g�p�̊֐��B
		void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

		//�A�j���[�V�����B
		enum EnAnimationClip {
			enAnimationClip_Idle,				//�ҋ@�A�j���[�V�����B	
			enAnimationClip_Run,				//����A�j���[�V�����B
			enAnimationClip_Walk,				//����A�j���[�V�����B
			enAnimationClip_Attack,				//�U���A�j���[�V�����B
			enAnimationClip_Attack2,			//�U���A�j���[�V����2�B
			enAnimationClip_Heal,				//�񕜃A�j���[�V�����B
			enAnimationClip_Damage,				//��_���[�W�A�j���[�V�����B
			enAnimationClip_Num,				//�A�j���[�V�����̐��B
		};

		WarpCounter* warpCounter = nullptr;

		AnimationClip			m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
		ModelRender				m_modelRender;								//���f�������_�\�B
		Vector3					m_position;									//���W�B
		Vector3					m_moveSpeed;								//�ړ����x�B
		Vector3					m_forward = Vector3::AxisZ;					//�v���C���[�̐��ʃx�N�g���B
		Vector3					m_scale;									//�傫���B
		Quaternion				m_rotation;									//�N�H�[�^�j�I���B
		CharacterController		m_charaCon;									//�L�����N�^�[�R���g���[���[�B
		EnPlayerState			m_playerState = enPlayerState_Idle;			//�v���C���[�X�e�[�g�B
		SpriteRender			m_HPRender;						        //�摜�B
		SpriteRender m_staminaRender;

		SpriteRender			m_HPberRender;						        //�摜�B
		SpriteRender m_stmnberRender;


		SpriteRender			m_portionRender;							//�|�[�V�����B
		FontRender				m_fontRender;								//�����@HP�̕\���B
		FontRender				m_staminaRender2;								//�����@stamina�̕\���B
		FontRender				m_fontRender2;					            //�����@Level�̕\���B
		FontRender				m_fontRender3;					            //�����@���̏������̕\���B
		EffectEmitter* m_effectEmitter = nullptr;					//�G�t�F�N�g�B
		Game* m_game = nullptr;							//�Q�[���B
		bool					m_isUnderAttack = false;					//�U�����Ȃ�true�B
		bool					m_isUnderHeal = false;						//�U�����Ȃ�true�B
		float					m_hp = 100.0f;								//�v���C���[��HP�B
		float					life = 100.0f;								//�t��HP�o�[
		int                     m_sword_jointBoneId = -1;                   //�uSword�v�{�[����ID�B
		float m_sutamina = 150.0f;		//�X�^�~�i�B
		float m_hurusutamina = 150.0f;	//�t���X�^�~�i�B

		float hpber = 1.0f;
		float stmnber = 1.0f;

		float					m_timer = 0.0f;								//�^�C�}�[�B
		int						m_portion = 0;								//�|�[�V�����B
		int						m_boss = 0;									//�{�X�B
		float cooltime = 0.0f;
		bool COOLtime = false;
		bool kaihuku = false;
		float genHP = 0.0f;
		float timer = 0.0f;
	};
