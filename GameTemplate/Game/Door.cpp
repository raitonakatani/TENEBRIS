#include "stdafx.h"
#include "Door.h"
#include "Player.h"
#include "Kisi.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Enemy4.h"

Door::Door()
{

}

Door::~Door()
{

}

bool Door::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/door/door.idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/door/door.open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Close].Load("Assets/animData/door/door.close.tka");
	m_animationClips[enAnimationClip_Close].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/door/door.tkm", m_animationClips, enAnimationClip_Num);
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	m_player = FindGO<Player>("player");

	//���f���̍X�V�B
	m_modelRender.Update();
	//�����蔻����쐬����B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	
	return true;
}

void Door::Update()
{
	//�A�j���[�V�������Đ�����B
	PlayAnimation();
	//�X�e�[�g�Ǘ��B
	ManageState();


	//�v���C���[���灙�Ɍ������x�N�g�����v�Z�B
	Vector3 diff = m_player->GetPosition() - m_position;
	//�x�N�g���̒�����120.0f��菬����������B
	if (diff.LengthSq() <= 500.0f * 500.0f)
	{
		if (m_doorState == enDoorState_Idle)
		{
			door = true;

			//if (m_player->GetKAGI() == 1)
			//{
				m_fontRender.SetText(L"�����J����B\nA�{�^��");
				//�\��������W��ݒ肷��B
				m_fontRender.SetPosition({ -100.0f,300.0f,0.0f });
				//�����̑傫����ς���B
				//fontRender.SetScale(1.5f);
				//�\������F��ݒ肷��B
				m_fontRender.SetColor(g_vec4White);


				if (g_pad[0]->IsTrigger(enButtonA))
				{
					NotifyOpen();
				}
				//}
		/*	else if (m_player->GetKAGI() == 0)
			{
				m_fontRender2.SetText(L"�J�M���������Ă��܂���B");
				//�\��������W��ݒ肷��B
				m_fontRender2.SetPosition({ -100.0f,300.0f,0.0f });
				//�����̑傫����ς���B
				//fontRender.SetScale(1.5f);
				//�\������F��ݒ肷��B
				m_fontRender2.SetColor(g_vec4Black);
			}
			*/
		}
		else
		{
			door = false;
		}
	}
	else {
		door = false;
	}

	//���f���̍X�V�B
	m_modelRender.Update();
}

void Door::NotifyOpen()
{
	//�I�[�v���X�e�[�g�ɑJ�ڂ���B
	m_doorState = enDoorState_Open;
}

void Door::NotifyClose()
{
	//�N���[�Y�X�e�[�g�ɑJ�ڂ���B
	m_doorState = enDoorState_Close;
}

void Door::PlayAnimation()
{
	switch (m_doorState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enDoorState_Idle:
		//�ҋ@�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//�I�[�v���X�e�[�g�̎��B
	case enDoorState_Open:
		//�I�[�v���A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Open);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//�N���[�Y�X�e�[�g�̎��B
	case enDoorState_Close:
		//�N���[�Y�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		m_modelRender.SetAnimationSpeed(0.6f);
		break;
	default:
		break;
	}
}

void Door::ReleasePhysicsObject()
{
	//�����蔻����J������B
	m_physicsStaticObject.Release();
}

void Door::CreatePhysicsObject()
{
	//�����蔻����쐬����B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void Door::ProcessOpenStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == true) {
		//�����蔻����J������B
		ReleasePhysicsObject();
	}

	//�I�[�v���A�j���[�V�����̍Đ����I��������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�����蔻����J������B
		//ReleasePhysicsObject();
		//�I�[�v���I���X�e�[�g�ɑJ�ڂ���B
		m_doorState = enDoorState_Open_Idle;

		auto enemys = FindGOs<Kisi>("enemy");
		for (auto enemy : enemys)
		{
			for (int i = 0;i < 3;i++) {
				//�R�m�������B
				if (m_doorNumber == enemy->GetkisiNumber())
				{

					enemy->MODEL();
					break;
				}
			}
		}

		auto wizards = FindGOs<Enemy2>("enemy2");
		for (auto wizard : wizards) {
			for (int i = 0;i < 3;i++) {
				//���@�g���������B
				if (m_doorNumber == wizard->GetwizardNumber())
				{
					wizard->MODEL();
					break;
				}
			}
		}

		auto bosss = FindGOs<Enemy3>("enemy3");
		for (auto boss : bosss) {

			//�{�X�������B
			if (m_doorNumber == boss->GetbossNumber())
			{
				boss->MODEL();
				break;
			}
		}

		/*	//���{�X�������B
			if (m_doorNumber == tyuuboss->GettyuubossNumber())
			{
				tyuuboss->MODEL();
			}
		*/
	}
}

void Door::ProcessCloseStateTransition()
{
	//�N���[�Y�A�j���[�V�������I��������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�����蔻����쐬����B
		CreatePhysicsObject();
		//�N���[�Y�I���X�e�[�g�ɑJ�ڂ���B
		m_doorState = enDoorState_Close_Idle;
	}
}

void Door::ManageState()
{
	switch (m_doorState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case Door::enDoorState_Idle:

		break;
		//�I�[�v���X�e�[�g�̎��B
	case Door::enDoorState_Open:
		//�I�[�v���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessOpenStateTransition();
		break;
		//�I�[�v���I���X�e�[�g�̎��B
	case Door::enDoorState_Open_Idle:

		break;
		//�N���[�Y�X�e�[�g�̎��B
	case Door::enDoorState_Close:
		//�N���[�Y�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessCloseStateTransition();
		break;
		//�N���[�Y�I���X�e�[�g�̎��B
	case Door::enDoorState_Close_Idle:

		break;
	default:
		break;
	}
}

void Door::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);

	//�h�A�ɋ߂Â����Ƃ��B
	if (door == true)
	{
		//������`�ʂ���B
		m_fontRender.Draw(rc);

	}
	else {
		return;
	}
}

