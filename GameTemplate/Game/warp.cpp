#include "stdafx.h"
#include "warp.h"
#include "fastPlayer.h"
#include "Player.h"
#include "WarpCounter.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

warp::warp()
{

}

warp::~warp()
{

}

bool warp::Start()
{
	//☆のモデルを読み込む。
	m_modelRender.Init("Assets/modelData/Enemy/enemy2.tkm");
	m_modelRender.SetScale({ 2.5f,2.5f,2.5f });
	m_modelRender.SetPosition({ 0.0f, 250.0f, -16500.0f });
	//warp
	//プレイヤーのオブジェクトを探して持ってくる。
	m_fastplayer = FindGO<fastPlayer>("fastplayer");
	warpCounter = FindGO<WarpCounter>("WarpCounter");


	return(true);
}

void warp::Update()
{
	if (warpCounter->warpCounter == 0)
	{
		//回転処理。
		Rotation();

		//絵描きさんの更新処理。
		m_modelRender.Update();

		//絵描きさんに座標を教える。
		m_modelRender.SetPosition(m_position);




		//プレイヤーから☆に向かうベクトルを計算。
		Vector3 diff = m_fastplayer->GetPosition() - m_position;
		//ベクトルの長さが120.0fより小さかったら。
		if (diff.Length() <= 300.0f)
		{
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//カウントを+1する。
				warpCounter->warpCounter = 1;

				//音を読み込む。
				g_soundEngine->ResistWaveFileBank(0, "Assets/sound/0titlebutton.wav");
				//効果音を再生する。
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(0);
				se->Play(false);
				se->SetVolume(0.8f);

				//自信を削除する。
				DeleteGO(this);
			}
			/*g_soundEngine->ResistWaveFileBank(4, "Assets/sound/4get.wav");
			//効果音を再生する。
			SoundSource* Getse = NewGO<SoundSource>(0);
			Getse->Init(4);
			Getse->Play(false);
			Getse->SetVolume(1.5f);*/


		}
	}
}

void warp::Rotation()
{
		//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
		//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
		//atan2を使用して、角度を求めている。
		//これが回転角度になる。

		Vector3 diff = m_fastplayer->GetPosition() - m_position;

		float angle = atan2(-diff.x, diff.z);
		//atanが返してくる角度はラジアン単位なので
		//SetRotationDegではなくSetRotationを使用する。
		m_rotation.SetRotationY(-angle);


		//回転を設定する。
		m_modelRender.SetRotation(m_rotation);
}

void warp::Render(RenderContext& rc)
{
	//☆を描画する。
	m_modelRender.Draw(rc);

}