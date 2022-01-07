#pragma once

/// <summary>
/// �h�A�B
/// </summary>

class Player;
class Game;

class Door : public IGameObject
{
public:
	//�h�A�X�e�[�g�B
	enum EnDoorState {
		enDoorState_Idle,				//�ҋ@�B
		enDoorState_Open,				//�I�[�v���B
		enDoorState_Open_Idle,			//�I�[�v�����I������B
		enDoorState_Close,				//�N���[�Y�B
		enDoorState_Close_Idle			//�N���[�Y���I������B
	};
public:
	Door();
	~Door();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// �I�[�v����ʒm����B
	/// </summary>
	void NotifyOpen();
	/// <summary>
	/// �N���[�Y��ʒm����B
	/// </summary>
	void NotifyClose();
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
	/// �h�A�̔ԍ���ݒ肷��B
	/// </summary>
	/// <param name="doorNumber">�h�A�̔ԍ��B</param>
	void SetDoorNumber(const int doorNumber)
	{
		m_doorNumber = doorNumber;
	}
	/// <summary>
	/// �h�A�̔ԍ����擾����B
	/// </summary>
	/// <returns>�h�A�̔ԍ��B</returns>
	const int GetDoorNumber() const
	{
		return m_doorNumber;
	}

	bool IsRange() const
	{
		return  m_range;
	}
private:
	/// <summary>
	/// �A�j���[�V�������Đ�����B
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �t�B�W�b�N�X�I�u�W�F�N�g�̓����蔻����J������B
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
	/// �t�B�W�b�N�X�I�u�W�F�N�g�̓����蔻����쐬����B
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// �X�e�[�g�Ǘ��B
	/// </summary>
	void ManageState();
	/// <summary>
	/// �I�[�v���X�e�[�g�̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessOpenStateTransition();
	/// <summary>
	/// �N���[�Y�X�e�[�g�̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessCloseStateTransition();
private:
	ModelRender					m_modelRender;							//���f�������_�\�B
	Vector3						m_position;								//���W�B
	Quaternion					m_rotation;								//��]�B
	Vector3						m_scale = Vector3::One;					//�傫���B
	FontRender					m_fontRender;							//�J�M���������Ă���Ƃ��B
	SpriteRender				m_telopRender;							//�e���b�v
	SpriteRender				m_kaiwa1Render;							//����
	enum EnAnimationClip {												//�A�j���[�V�����B
		enAnimationClip_Idle,											//�ҋ@�B	
		enAnimationClip_Open,											//�I�[�v���B
		enAnimationClip_Close,											//�N���[�Y�B
		enAnimationClip_Num,											//�A�j���[�V�����̐��B
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];	//�A�j���[�V�����N���b�v�B
	PhysicsStaticObject			m_physicsStaticObject;					//�t�B�W�N�X�X�^�e�B�b�N�I�u�W�F�N�g�B	
	EnDoorState					m_doorState = enDoorState_Idle;			//�h�A�X�e�[�g�B
	Player*						m_player;								//�v���C���[�B
	Game*						m_game;
	bool						door = false;							//�h�A�Ƃ̋����B
	int							m_doorNumber = 0;						//�h�A�̔ԍ��B
	bool model = false;



	bool m_range = false;

	int m_DoorCount = 0;
};

