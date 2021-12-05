#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	m_modelRender.Init("Assets/modelData/stage/RPG.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}


void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}