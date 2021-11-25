#pragma once

class Player;

//���N���X�B
class Portion : public IGameObject
{
public:
	Portion();
	~Portion();
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

private:
	//�������烁���o�ϐ��B
	ModelRender m_modelRender;            //���f�������_�[�B
	Vector3 m_position;                  //���W�B
	Vector3 m_firstPosition;             //�ŏ��̍��W�B
	int moveState = 0;                   //��Ɉړ������Ɉړ����B
	Quaternion rotation;                //�N�H�[�^�j�I���B
	Player* m_player;                   //�v���C���[�B
	SoundSource* se;                    //se
};

