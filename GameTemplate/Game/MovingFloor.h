#pragma once

class fastPlayer;

class MovingFloor : public IGameObject
{
public:
	MovingFloor();
	~MovingFloor();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
private:
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	Vector3					m_position;
	Vector3					m_firstPosition;
	ModelRender				m_modelRender;
	fastPlayer* m_player = nullptr;
	PhysicsStaticObject		m_physicsStaticObject;				//�ÓI�����I�u�W�F�N�g�B
	CollisionObject* m_collisionObject = nullptr;		//�R���W�����I�u�W�F�N�g�B
	enum enMovingFloorState
	{
		enMovingFloorState_MovingRight,
		enMovingFloorState_MovingLeft
	};
	enMovingFloorState		m_MovingFloorState = enMovingFloorState_MovingRight;

};
