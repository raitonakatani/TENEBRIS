#include "stdafx.h"
#include "Magic.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//CollisionObjectを使用したいため、ファイルをインクルードする。
#include "collision/CollisionObject.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"


Magic::Magic()
{

}

Magic::~Magic()
{
	//エフェクトを停止する。
	m_effectEmitter->Stop();
	//コリジョンオブジェクトを削除する。
	DeleteGO(m_collisionObject);
	DeleteGO(m_effectEmitter);
}

bool Magic::Start()
{
	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/efk/Magic.efk");

	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/enemy.efk");

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(0);
		//エフェクトの大きさを設定する。
		//m_effectEmitter->SetScale(m_scale * 20.0f);
	//移動速度を計算。
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 1200.0f;
	m_rotation.AddRotationDegY(360.0f);
	//回転を設定する。
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();


	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/5mahou.wav");
	//効果音を再生する。
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(5);
	se->Play(false);
	se->SetVolume(0.5f);


	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);

	if (m_enMagician == enMagician_Enemy2)
	{
		//エフェクトの大きさを設定する。
		m_effectEmitter->SetScale(m_scale * 20.0f);

		//球状のコリジョンを作成する。
		m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 50.0f * m_scale.z);
	}

	else if(m_enMagician == enMagician_Enemy3)
	{
		//エフェクトの大きさを設定する。
		m_effectEmitter->SetScale(m_scale * 30.0f);

		//球状のコリジョンを作成する。
		m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 75.0f * m_scale.z);
	}

	else if (m_enMagician == enMagician_Enemy4)
	{
		//エフェクトの大きさを設定する。
		m_effectEmitter->SetScale(m_scale * 20.0f);

		//球状のコリジョンを作成する。
		m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 50.0f * m_scale.z);
	}


	//名前をenemy_fireballにする。
	m_collisionObject->SetName("enemy_Magic");

	//懲り所オブジェクトが自動で削除されないようにする。
	m_collisionObject->SetIsEnableAutoDelete(false);
	return true;
}

void Magic::Update()
{
	//座標を移動させる。
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//エフェクトの座標を設定する。
	m_effectEmitter->SetPosition(m_position);
	//コリジョンオブジェクトに座標を設定する。
	m_collisionObject->SetPosition(m_position);

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();

	//タイマーが1.0f以上だったら。
	if (m_timer >= 1.0f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}