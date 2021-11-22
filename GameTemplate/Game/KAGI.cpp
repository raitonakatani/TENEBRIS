#include "stdafx.h"
#include "KAGI.h"
#include "Player.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

KAGI::KAGI()
{

}

KAGI::~KAGI()
{

}

bool KAGI::Start()
{
	//☆のモデルを読み込む。
	m_modelRender.Init("Assets/modelData/rin.tkm");

	//プレイヤーのオブジェクトを探して持ってくる。
	m_player = FindGO<Player>("player");

	return(true);
}

void KAGI::Update()
{

	//回転処理。
	Rotation();

	//絵描きさんの更新処理。
	m_modelRender.Update();

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);

	//プレイヤーから☆に向かうベクトルを計算。
	Vector3 diff = m_player->GetPosition() - m_position;
	//ベクトルの長さが120.0fより小さかったら。
	if (diff.Length() <= 120.0f)
	{
		//カウントを+1する。
		m_player->PlusKAGI();

		g_soundEngine->ResistWaveFileBank(4, "Assets/sound/4get.wav");
		//効果音を再生する。
		SoundSource* Getse = NewGO<SoundSource>(0);
		Getse->Init(4);
		Getse->Play(false);
		Getse->SetVolume(1.5f);

		//自信を削除する。
		DeleteGO(this);

	}
}

void KAGI::Rotation()
{
	//回転を加算する。
	rotation.AddRotationDegY(2.0f);

	//絵描きさんに回転を教える。
	m_modelRender.SetRotation(rotation);
}

void KAGI::Render(RenderContext& rc)
{
	//☆を描画する。
	m_modelRender.Draw(rc);
}