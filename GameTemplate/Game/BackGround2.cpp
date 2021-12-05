#include "stdafx.h"
#include "BackGround2.h"

BackGround2::BackGround2()
{

}

BackGround2::~BackGround2()
{

}

bool BackGround2::Start()
{
	m_modelRender.Init("Assets/modelData/RPG2.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}


void BackGround2::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}