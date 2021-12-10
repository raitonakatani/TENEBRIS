#pragma once

class Player;
class fastPlayer;
class WarpCounter;

//���N���X�B
class Fire : public IGameObject
{
public:
	Fire();
	~Fire();
	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	void CreateFireEffect();
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	//��]��ݒ�B
	void SetRotation(const Quaternion rotation)
	{
		m_rotation = rotation;
	}

	//�傫����ݒ�B
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}

private:
	//�������烁���o�ϐ��B
	ModelRender m_modelRender;            //���f�������_�[�B
	Vector3 m_position;                  //���W�B
	int moveState = 0;                   //��Ɉړ������Ɉړ����B
	Player* m_player;                   //�v���C���[�B
	fastPlayer* m_fastplayer;
	SoundSource* se;                    //se
	EffectEmitter* m_effectEmitter = nullptr;					//�G�t�F�N�g�B
	Quaternion		m_rotation;
	PhysicsStaticObject m_physicsStaticObject;      //�ÓI�����I�u�W�F�N�g�B
	Vector3			m_scale;
	PointLight* m_pointLight = nullptr;
	WarpCounter* warpCounter = nullptr;
	float m_timer = 0.0f;
	bool model = true;
};



