#include "stdafx.h"
#include "Fire.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

Fire::Fire()
{

}

Fire::~Fire()
{

}

bool Fire::Start()
{
	//☆のモデルを読み込む。
	m_modelRender.Init("Assets/modelData/Fire/fire.tkm");
	//m_modelRender.SetScale(100.0f, 100.0f, 100.0f);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	// 番号を指定して、efkファイルを読み込む。
	EffectEngine::GetInstance()->ResistEffect(4, u"Assets/effect/efk/fire.efk");

	m_pointLight = g_sceneLight->NewPointLight();

	m_pointLight->SetColor(Vector3(1.0f,0.0f,0.0f));
	m_pointLight->SetAffectPowParam(0.4f);
	m_pointLight->SetRange(500.0f);
	m_pointLight->SetPosition(m_position + Vector3(0.0f, 125.0f, 0.0f));

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(4);
	//エフェクトの大きさを設定する。
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

	//絵描きさんの更新処理。
	//m_modelRender.Update();

	//絵描きさんに座標を教える。
	//m_modelRender.SetPosition(m_position);
}

void Fire::Render(RenderContext& rc)
{
	//☆を描画する。
	m_modelRender.Draw(rc);
}