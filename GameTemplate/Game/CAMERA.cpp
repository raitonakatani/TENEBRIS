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
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos.Set(m_Pos);
	//�v���C���[�̃C���X�^���X��T���B
	m_fastplayer = FindGO<fastPlayer>("fastplayer");

	m_player = FindGO<Player>("player");

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
		5000.0f,			//�J�����̈ړ����x�̍ő�l�B
		true,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		5.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);
	m_springCamera.SetNear(1.0f);
	m_springCamera.SetFar(40000.0f);

	warpCounter = FindGO<WarpCounter>("WarpCounter");


//	if (warpCounter->warpCounter == 0)
//	{
		//�����_���王�_�܂ł̃x�N�g����ݒ�B
		m_toCameraPos.Set(0.1f, 200.0f, -200.0f);
//	}
	

		target = m_player->GetPosition();
		//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
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

		//�J�������X�V�B
		//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
		m_springCamera.SetTarget(target);

		//for (int i = 0;i < 100;i++)
		//{
			posi = posi * 1.001f;

			//YZIKU++;
			//pos.z--;

		//}
		m_springCamera.SetPosition(posi);

		//�J�����̍X�V�B
		m_springCamera.Update();

		//m_springCamera.Update();
	}
	else {
		if (warpCounter->warpCounter == 0)
		{
			//�J�������X�V�B
			//�����_���v�Z����B
			target = m_fastplayer->GetPosition();
			//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
			target.y += 150.0f;
			target += g_camera3D->GetForward() * 20.0f;

			Vector3 toCameraPosOld = m_toCameraPos;
			//�p�b�h�̓��͂��g���ăJ�������񂷁B
			float x = g_pad[0]->GetRStickXF();
			float y = g_pad[0]->GetRStickYF();
			//Y������̉�]
			Quaternion qRot;
			qRot.SetRotationDeg(Vector3::AxisY, 2.3f * x);
			qRot.Apply(m_toCameraPos);
			//X������̉�]�B
			Vector3 axisX;
			axisX.Cross(Vector3::AxisY, m_toCameraPos);
			axisX.Normalize();
			qRot.SetRotationDeg(axisX, 2.3f * y);
			qRot.Apply(m_toCameraPos);
			//�J�����̉�]�̏�����`�F�b�N����B
			//�����_���王�_�܂ł̃x�N�g���𐳋K������B
			//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
			//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();
			if (toPosDir.y < -0.5f) {
				//�J����������������B
				m_toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > 0.9f) {
				//�J�����������������B
				m_toCameraPos = toCameraPosOld;
			}
			//���_���v�Z����B
			pos = target + m_toCameraPos;
			//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
			m_springCamera.SetTarget(target);
			m_springCamera.SetPosition(pos);

			//�J�����̍X�V�B
			m_springCamera.Update();
		}


		if (warpCounter->warpCounter != 0)
		{
			//�J�������X�V�B
			//�����_���v�Z����B
			target = m_player->GetPosition();
			//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
			target.y += 150.0f;
			target += g_camera3D->GetForward() * 20.0f;

			Vector3 toCameraPosOld = m_toCameraPos;
			//�p�b�h�̓��͂��g���ăJ�������񂷁B
			float x = g_pad[0]->GetRStickXF();
			float y = g_pad[0]->GetRStickYF();
			//Y������̉�]
			Quaternion qRot;
			qRot.SetRotationDeg(Vector3::AxisY, 2.3f * x);
			qRot.Apply(m_toCameraPos);
			//X������̉�]�B
			Vector3 axisX;
			axisX.Cross(Vector3::AxisY, m_toCameraPos);
			axisX.Normalize();
			qRot.SetRotationDeg(axisX, 2.3f * y);
			qRot.Apply(m_toCameraPos);
			//�J�����̉�]�̏�����`�F�b�N����B
			//�����_���王�_�܂ł̃x�N�g���𐳋K������B
			//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
			//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
			Vector3 toPosDir = m_toCameraPos;
			toPosDir.Normalize();
			if (toPosDir.y < -0.5f) {
				//�J����������������B
				m_toCameraPos = toCameraPosOld;
			}
			else if (toPosDir.y > 0.9f) {
				//�J�����������������B
				m_toCameraPos = toCameraPosOld;
			}

			//���_���v�Z����B
			pos = target + m_toCameraPos;
			//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
			m_springCamera.SetTarget(target);
			m_springCamera.SetPosition(pos);

			posi = pos;

			//�J�����̍X�V�B
			m_springCamera.Update();
		}
	}
}