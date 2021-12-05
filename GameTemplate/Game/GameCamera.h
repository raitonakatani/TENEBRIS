#pragma once

//�΂˃J�������g�p�������ꍇ�́ASpringCamera���C���N���[�h����B
#include "camera/SpringCamera.h"

class Player;

//�Q�[�����̃J�����𐧌䂷��B
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	/// <summary>
/// ���W��ݒ�B
/// </summary>
/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_Pos = position;
	}
	/////////////////////////////////////
	//�����o�ϐ�
	/////////////////////////////////////
private:
	Player* m_player;	//�v���C���[�B
	Vector3 m_toCameraPos;	//�����_���王�_�Ɍ������x�N�g���B
	Vector3 m_Pos;	//�����_���王�_�Ɍ������x�N�g���B
	SpringCamera m_springCamera;	//�΂˃J�����B
};