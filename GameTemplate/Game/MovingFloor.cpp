#include "stdafx.h"
#include "MovingFloor.h"

#include "fastPlayer.h"

namespace
{
	Vector3	SCALE = Vector3(3.0f, 1.0, 3.0f);
	float SPEED = 200.0f;
	float LIMITED = 300.0f;
	Vector3 COLLISION_HEIGHT = Vector3(0.0f, 50.0f, 0.0f);
	Vector3	COLLISION_SIZE = Vector3(365.0f, 5.0f, 225.0f);
}

MovingFloor::MovingFloor()
{

}

MovingFloor::~MovingFloor()
{

}

bool MovingFloor::Start()
{
	m_modelRender.Init("Assets/modelData/rin.tkm");
	m_modelRender.SetScale({ 5.0f,5.0f,5.0f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//	m_collisionObject = NewGO<CollisionObject>(0, "collisionObject");

		//コリジョンオブジェクトを。
		//動く床の上に設置する(キャラクターが上に乗ったら反応するようにしたいため)。
	/*	m_collisionObject->CreateBox(
			m_position + COLLISION_HEIGHT,
			Quaternion::Identity,
			COLLISION_SIZE
		);
	*/
	//	m_collisionObject->SetIsEnableAutoDelete(false);
	m_firstPosition = m_position;

	m_player = FindGO<fastPlayer>("player");
	return true;
}

void MovingFloor::Update()
{
	if (m_player == nullptr)
	{
		m_player = FindGO<fastPlayer>("player");
		return;
	}


	Move();

	m_modelRender.Update();

	m_physicsStaticObject.SetPosition(m_position);
	//	m_collisionObject->SetPosition(m_position + COLLISION_HEIGHT);
}

void MovingFloor::Move()
{

	Vector3 diff = m_player->GetPosition() - m_position;

	if (diff.Length() <= 300.0f)
	{

		if (g_pad[0]->IsPress(enButtonRB2))
		{
			m_position += diff;
			m_modelRender.SetPosition(m_position);
			m_modelRender.Update();
		}
	}

	m_modelRender.SetPosition(m_position);

}

void MovingFloor::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}