#include "stdafx.h"
#include "Fire.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

Fire::Fire()
{

}

Fire::~Fire()
{

}

bool Fire::Start()
{
	//���̃��f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/Fire/fire.tkm");
	//m_modelRender.SetScale(100.0f, 100.0f, 100.0f);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	// �ԍ����w�肵�āAefk�t�@�C����ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/effect/efk/fire.efk");

	m_pointLight = g_sceneLight->NewPointLight();

	m_pointLight->SetColor(Vector3(1.0f,0.0f,0.0f));
	m_pointLight->SetAffectPowParam(0.4f);
	m_pointLight->SetRange(500.0f);
	m_pointLight->SetPosition(m_position + Vector3(0.0f, 125.0f, 0.0f));

	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(4);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale(Vector3(10.0f,8.0f,10.0f));
	Vector3 effectposition = m_position;
	effectposition.y += 125.0f;
	m_effectEmitter->SetPosition(effectposition);
	Quaternion rotation;
	rotation = m_rotation;
	m_effectEmitter->SetRotation(rotation);
	m_effectEmitter->Play();

	return(true);
}

void Fire::Update()
{

	//�G�`������̍X�V�����B
	//m_modelRender.Update();

	//�G�`������ɍ��W��������B
	//m_modelRender.SetPosition(m_position);
}

void Fire::Render(RenderContext& rc)
{
	//����`�悷��B
	m_modelRender.Draw(rc);
}