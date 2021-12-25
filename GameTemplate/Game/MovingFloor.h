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
	/// 座標を設定。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
private:
	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();
	Vector3					m_position;
	Vector3					m_firstPosition;
	ModelRender				m_modelRender;
	fastPlayer* m_player = nullptr;
	PhysicsStaticObject		m_physicsStaticObject;				//静的物理オブジェクト。
	CollisionObject* m_collisionObject = nullptr;		//コリジョンオブジェクト。
	enum enMovingFloorState
	{
		enMovingFloorState_MovingRight,
		enMovingFloorState_MovingLeft
	};
	enMovingFloorState		m_MovingFloorState = enMovingFloorState_MovingRight;

};
