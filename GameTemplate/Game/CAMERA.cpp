#include "stdafx.h"
#include "CAMERA.h""
#include "fastPlayer.h"
#include"Player.h"
#include"WarpCounter.h"

CAMERA::CAMERA()
{
}


CAMERA::~CAMERA()
{
}
bool CAMERA::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(m_Pos);
	//プレイヤーのインスタンスを探す。
	m_fastplayer = FindGO<fastPlayer>("fastplayer");

	m_player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		5000.0f,			//カメラの移動速度の最大値。
		true,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		5.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);
	m_springCamera.SetNear(1.0f);
	m_springCamera.SetFar(40000.0f);

	warpCounter = FindGO<WarpCounter>("WarpCounter");


//	if (warpCounter->warpCounter == 0)
//	{
		//注視点から視点までのベクトルを設定。
		m_toCameraPos.Set(0.1f, 200.0f, -200.0f);
//	}
	

		target = m_player->GetPosition();
		//プレイヤの足元からちょっと上を注視点とする。
		target.y += 150.0f;
		target += g_camera3D->GetForward() * 20.0f;
		pos = target + m_toCameraPos;
		
		//= target + m_toCameraPos;

	return true;
}
void CAMERA::Update()
{

	if (m_player->GetPlayerHP() <= 0)
	{
		//YZIKU++;
		//ZZIKU--;
		//m_toCameraPos.Set(0.0f, YZIKU, ZZIKU);

		//カメラを更新。
		//メインカメラに注視点と視点を設定する。
		m_springCamera.SetTarget(target);

		//for (int i = 0;i < 100;i++)
		//{
			posi = posi * 1.001f;

			//YZIKU++;
			//pos.z--;

		//}
		m_springCamera.SetPosition(posi);

		//カメラの更新。
		m_springCamera.Update();

		//m_springCamera.Update();
	}
	else {
		if (warpCounter->warpCounter == 0)
		{
			//カメラを更新。
			//注視点を計算する。
			target = m_fastplayer->GetPosition();
			//プレイヤの足元からちょっと上を注視点とする。
			target.y += 150.0f;
			target += g_camera3D->GetForward() * 20.0f;

			Vector3 toCameraPosOld = m_toCameraPos;
			//パッドの入力を使ってカメラを回す。
			float x = g_pad[0]->GetRStickXF();
			float y = g_pad[0]->GetRStickYF();
			//Y軸周りの回転
			Quaternion qRot;
			qRot.SetRotationDeg(Vector3::AxisY, 2.3f * x);
			qRot.Apply(m_toCameraPos);
			//X軸周りの回転。
			Vector3 axisX;
			axisX.Cross(Vector3::AxisY, m_toCameraPos);
			axisX.Normalize();
			qRot.SetRotationDeg(axisX, 2.3f * y);
			qRot.Apply(m_toCameraPos);
			//カメラの回転の上限をチェックする。
			//注視点から視点までのベクトルを正規化する。
			//正規化すると、ベクトルの大きさが１になる。
			//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();
			if (toPosDir.y < -0.5f) {
				//カメラが上向きすぎ。
				m_toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > 0.9f) {
				//カメラが下向きすぎ。
				m_toCameraPos = toCameraPosOld;
			}
			//視点を計算する。
			pos = target + m_toCameraPos;
			//メインカメラに注視点と視点を設定する。
			m_springCamera.SetTarget(target);
			m_springCamera.SetPosition(pos);

			//カメラの更新。
			m_springCamera.Update();
		}


		if (warpCounter->warpCounter != 0)
		{
			//カメラを更新。
			//注視点を計算する。
			target = m_player->GetPosition();
			//プレイヤの足元からちょっと上を注視点とする。
			target.y += 150.0f;
			target += g_camera3D->GetForward() * 20.0f;

			Vector3 toCameraPosOld = m_toCameraPos;
			//パッドの入力を使ってカメラを回す。
			float x = g_pad[0]->GetRStickXF();
			float y = g_pad[0]->GetRStickYF();
			//Y軸周りの回転
			Quaternion qRot;
			qRot.SetRotationDeg(Vector3::AxisY, 2.3f * x);
			qRot.Apply(m_toCameraPos);
			//X軸周りの回転。
			Vector3 axisX;
			axisX.Cross(Vector3::AxisY, m_toCameraPos);
			axisX.Normalize();
			qRot.SetRotationDeg(axisX, 2.3f * y);
			qRot.Apply(m_toCameraPos);
			//カメラの回転の上限をチェックする。
			//注視点から視点までのベクトルを正規化する。
			//正規化すると、ベクトルの大きさが１になる。
			//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();
			if (toPosDir.y < -0.5f) {
				//カメラが上向きすぎ。
				m_toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > 0.9f) {
				//カメラが下向きすぎ。
				m_toCameraPos = toCameraPosOld;
			}

			//視点を計算する。
			pos = target + m_toCameraPos;
			//メインカメラに注視点と視点を設定する。
			m_springCamera.SetTarget(target);
			m_springCamera.SetPosition(pos);

			posi = pos;

			//カメラの更新。
			m_springCamera.Update();
		}
	}
}