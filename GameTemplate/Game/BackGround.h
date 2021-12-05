#pragma once


//背景
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();

	//描画処理。
	bool Start();
	void Render(RenderContext& rc);

	//座標を設定。
	void SetPosition(const Vector3 position)
	{
		m_position = position;
	}

	//回転を設定。
	void SetRotation(const Quaternion rotation)
	{
		m_rotation = rotation;
	}

	//大きさを設定。
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}

private:
	ModelRender m_modelRender;                      //モデルレンダー
	PhysicsStaticObject m_physicsStaticObject;      //静的物理オブジェクト。
	Vector3			m_position;
	Quaternion		m_rotation;
	Vector3			m_scale;
};