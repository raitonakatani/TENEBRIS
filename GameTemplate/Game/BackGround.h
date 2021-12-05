#pragma once


//�w�i
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();

	//�`�揈���B
	bool Start();
	void Render(RenderContext& rc);

	//���W��ݒ�B
	void SetPosition(const Vector3 position)
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
	ModelRender m_modelRender;                      //���f�������_�[
	PhysicsStaticObject m_physicsStaticObject;      //�ÓI�����I�u�W�F�N�g�B
	Vector3			m_position;
	Quaternion		m_rotation;
	Vector3			m_scale;
};