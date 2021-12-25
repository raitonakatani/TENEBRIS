#pragma once

class fastPlayer;
class WarpCounter;
//class Player;
class warp : public IGameObject
{
public:
	warp();
	~warp();
	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//��]�����B
	void Rotation();

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

private:
	//�������烁���o�ϐ��B


	ModelRender				m_modelRender;			//���f�������_�[�B
	Vector3					m_position;				//���W�B
	Vector3					m_firstPosition;		//�ŏ��̍��W�B
	int						moveState = 0;			//��Ɉړ������Ɉړ����B
	Quaternion				m_rotation;				//�N�H�[�^�j�I���B
	fastPlayer*				m_fastplayer;			//�v���C���[�B
	WarpCounter*			warpCounter = nullptr;
	//bool mozi = false;
	//Player* m_player;
	//SoundSource* se;                    //se
};


