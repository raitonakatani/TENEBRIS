#include "stdafx.h"
#include "Portion.h"
#include "Player.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Portion::Portion()
{

}

Portion::~Portion()
{

}

bool Portion::Start()
{
	//���̃��f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/rin.tkm");
	//Portion
	//�v���C���[�̃I�u�W�F�N�g��T���Ď����Ă���B
	m_player = FindGO<Player>("player");

	return(true);
}

void Portion::Update()
{

	//��]�����B
	Rotation();

	//�G�`������̍X�V�����B
	m_modelRender.Update();

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);

	//�v���C���[���灙�Ɍ������x�N�g�����v�Z�B
	Vector3 diff = m_player->GetPosition() - m_position;
	//�x�N�g���̒�����120.0f��菬����������B
	if (diff.Length() <= 120.0f)
	{
		//�J�E���g��+1����B
		m_player->PlusPortion();

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

void Portion::Rotation()
{
	//��]�����Z����B
	rotation.AddRotationDegY(2.0f);

	//�G�`������ɉ�]��������B
	m_modelRender.SetRotation(rotation);
}

void Portion::Render(RenderContext& rc)
{
	//����`�悷��B
	m_modelRender.Draw(rc);
}