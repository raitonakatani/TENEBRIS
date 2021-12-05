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
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_modelRender.SetPosition({ 0.0f, 250.0f, -16500.0f });
	//warp
	//�v���C���[�̃I�u�W�F�N�g��T���Ď����Ă���B
	m_fastplayer = FindGO<fastPlayer>("fastplayer");

	return(true);
}

void warp::Update()
{

	//��]�����B
	Rotation();

	//�G�`������̍X�V�����B
	m_modelRender.Update();

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);

	m_fastplayer = FindGO<fastPlayer>("fastplayer");

	warpCounter = FindGO<WarpCounter>("WarpCounter");

	//�v���C���[���灙�Ɍ������x�N�g�����v�Z�B
	Vector3 diff = m_fastplayer->GetPosition() - m_position;
	//�x�N�g���̒�����120.0f��菬����������B
	if (diff.Length() <= 120.0f)
	{
		//�J�E���g��+1����B
		//m_fastplayer->PlusWarp();

		warpCounter->warpCounter = 1;

		g_soundEngine->ResistWaveFileBank(4, "Assets/sound/4get.wav");
		//���ʉ����Đ�����B
		SoundSource* Getse = NewGO<SoundSource>(0);
		Getse->Init(4);
		Getse->Play(false);
		Getse->SetVolume(1.5f);

		//���M���폜����B
		DeleteGO(this);

	}
}

void warp::Rotation()
{
	//��]�����Z����B
	rotation.AddRotationDegY(2.0f);

	//�G�`������ɉ�]��������B
	m_modelRender.SetRotation(rotation);
}

void warp::Render(RenderContext& rc)
{
	//����`�悷��B
	m_modelRender.Draw(rc);
}