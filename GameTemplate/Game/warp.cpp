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
	//���̃��f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/Enemy/enemy2.tkm");
	m_modelRender.SetScale({ 2.5f,2.5f,2.5f });
	m_modelRender.SetPosition({ 0.0f, 250.0f, -16500.0f });
	//warp
	//�v���C���[�̃I�u�W�F�N�g��T���Ď����Ă���B
	m_fastplayer = FindGO<fastPlayer>("fastplayer");
	warpCounter = FindGO<WarpCounter>("WarpCounter");


	return(true);
}

void warp::Update()
{
	if (warpCounter->warpCounter == 0)
	{
		//��]�����B
		Rotation();

		//�G�`������̍X�V�����B
		m_modelRender.Update();

		//�G�`������ɍ��W��������B
		m_modelRender.SetPosition(m_position);




		//�v���C���[���灙�Ɍ������x�N�g�����v�Z�B
		Vector3 diff = m_fastplayer->GetPosition() - m_position;
		//�x�N�g���̒�����120.0f��菬����������B
		if (diff.Length() <= 300.0f)
		{
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//�J�E���g��+1����B
				warpCounter->warpCounter = 1;

				//����ǂݍ��ށB
				g_soundEngine->ResistWaveFileBank(0, "Assets/sound/0titlebutton.wav");
				//���ʉ����Đ�����B
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(0);
				se->Play(false);
				se->SetVolume(0.8f);

				//���M���폜����B
				DeleteGO(this);
			}
			/*g_soundEngine->ResistWaveFileBank(4, "Assets/sound/4get.wav");
			//���ʉ����Đ�����B
			SoundSource* Getse = NewGO<SoundSource>(0);
			Getse->Init(4);
			Getse->Play(false);
			Getse->SetVolume(1.5f);*/


		}
	}
}

void warp::Rotation()
{
		//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
		//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
		//atan2���g�p���āA�p�x�����߂Ă���B
		//���ꂪ��]�p�x�ɂȂ�B

		Vector3 diff = m_fastplayer->GetPosition() - m_position;

		float angle = atan2(-diff.x, diff.z);
		//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
		//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
		m_rotation.SetRotationY(-angle);


		//��]��ݒ肷��B
		m_modelRender.SetRotation(m_rotation);
}

void warp::Render(RenderContext& rc)
{
	//����`�悷��B
	m_modelRender.Draw(rc);

}