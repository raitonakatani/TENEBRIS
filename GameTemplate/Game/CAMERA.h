#pragma once

//ばねカメラを使用したい場合は、SpringCameraをインクルードする。
#include "camera/SpringCamera.h"

class fastPlayer;
class Player;
class WarpCounter;

//ゲーム中のカメラを制御する。
class CAMERA : public IGameObject
{
public:
	CAMERA();
	~CAMERA();
	bool Start();
	void Update();

	/// <summary>
/// 座標を設定。
/// </summary>
/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_Pos = position;
	}
	/////////////////////////////////////
	//メンバ変数
	/////////////////////////////////////
private:
	fastPlayer* m_fastplayer;	//プレイヤー。
	Player* m_player;
	WarpCounter* warpCounter = nullptr;
	Vector3 m_toCameraPos;	//注視点から視点に向かうベクトル。
	Vector3 m_Pos;	//注視点から視点に向かうベクトル。
	SpringCamera m_springCamera;	//ばねカメラ。

	Vector3 target;
	Vector3 pos;
	Vector3 posi;

	float YZIKU = 200.0f;
	float ZZIKU = -200.0f;
};