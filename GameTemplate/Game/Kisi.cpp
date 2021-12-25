#include "stdafx.h"
#include "Kisi.h"

#include "Game.h"
#include "Player.h"
#include"Portion.h"
#include <stdlib.h>

//CollisionObject���g�p���������߁A�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"



/// <summary>
/// �G�l�~�[�P�B
/// �R�m�B
/// �����U���̂݁B
/// �h�������邩���B�i����̐i�݋�ɂ��j
/// </summary>

Kisi::Kisi()
{

}

Kisi::~Kisi()
{

}

bool Kisi::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy3/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy3/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy3/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy3/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy3/sibou.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/Enemy/enemy2.tkm", m_animationClips, enAnimationClip_Num);

	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/enemy.efk");

	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/efk/Magic.efk");

	//���W��ݒ肷��B
	Vector3 position = m_position;
	//position.y += 2.5f;
	m_modelRender.SetPosition(position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		40.0f,			//���a�B
		140.0f,			//�����B
		m_position		//���W�B
	);


	m_player = FindGO<Player>("player");

	m_spriteRender.Init("Assets/sprite/HP.dds", 64.0f, 32.0f);
	//�\��������W��ݒ肷��B
	m_spriteRender.SetPosition({ 0.0f,1450.0f ,0.0f });

	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);


	//���̃{�[����ID���擾����B
	m_swordBoneId = m_modelRender.FindBoneID(L"Sword_joint");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	return true;
}

void Kisi::Update()
{
	if (m_player->GetPlayerHP() != 0)
	{
		//�ǐՏ����B
		Chase();
		//��]�����B
		Rotation();
		//�����蔻��B
		Collision();
		//�U�������B
		Attack();
		//�A�j���[�V�����̍Đ��B
		PlayAnimation();
		//�X�e�[�g�̑J�ڏ����B
		ManageState();
	}

	modeltimer += g_gameTime->GetFrameDeltaTime();

	if (m_model == 1)
	{
		if (model == false) {
			//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
			m_effectEmitter = NewGO <EffectEmitter>(0);
			m_effectEmitter->Init(1);
			m_effectEmitter->SetPosition(m_position);
			//�G�t�F�N�g�̑傫����ݒ肷��B
			m_effectEmitter->SetScale(m_scale * 15.0f);
			m_effectEmitter->Play();

			modeltimer = 0.0f;

		}
		model = true;
	}
	else {
		model = false;
	}

	if (m_hp <= 0)
	{
		m_hp = 0;
	}

	//HP�o�[
	a = m_hp / 2;
	m_spriteRender.SetScale({ a, 0.5f, 0.0f });

	Vector3 diff = m_player->GetPosition() - m_position;

	//�x�N�g���̒�����700.0f��菬����������B
	if (diff.LengthSq() <= 800.0f * 800.0f)
	{
		//���[���h���W�ɕϊ��B
		//���W���G�l�~�[�̏�����ɐݒ肷��B
		Vector4 worldPos = Vector4(m_position.x, m_position.y + 250.0f, m_position.z, 1.0f);

		Matrix matrix;
		matrix.Multiply(g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());

		matrix.Apply(worldPos);

		//�J�����̃r���[�s����|����B
		//�J�������W�ɕϊ��B
		worldPos.x = (worldPos.x / worldPos.w);
		worldPos.y = (worldPos.y / worldPos.w);

		//�J�����̃v���W�F�N�V�����s����|����B
		//�X�N���[�����W�ɕϊ��B
		worldPos.x *= FRAME_BUFFER_W / 2;
		worldPos.y *= FRAME_BUFFER_H / 2;

		//�|�W�V�����̐ݒ�B
		m_spriteRender.SetPosition(Vector3(worldPos.x, worldPos.y, 0.0f));
		//HP�o�[��\������B
		m_isShowHPBar = true;
	}
	else
	{
		//HP�o�[��\�����Ȃ��B
		m_isShowHPBar = false;
	}
	m_spriteRender.Update();

	//�ΉB
	if (m_player->level == 2)
	{
		karyoku = 2;
	}
	if (m_player->level == 3)
	{
		karyoku = 3;
	}

	//���f���̍X�V�B
	m_modelRender.Update();
}

void Kisi::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Kisi::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}

	//�G�l�~�[���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		//�d�͂�0�ɂ���B
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	//���W��ݒ肷��B
	m_modelRender.SetPosition(modelPosition);

}

void Kisi::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= karyoku;

			//����ǂݍ��ށB
			g_soundEngine->ResistWaveFileBank(3, "Assets/sound/3damage.wav");
			//���ʉ����Đ�����B
			SoundSource* damagese = NewGO<SoundSource>(0);
			damagese->Init(3);
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//����HP��0����Ȃ�B
			if (m_hp > 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//HP��0�Ȃ�B
			else if (m_hp <= 0)
			{
				//�_�E���X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Down;
			}
			return;
		}
	}

}

void Kisi::Attack()
{
	//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}

	//�U�����ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
}

const bool Kisi::SearchPlayer() const
{
	//�G�l�~�[����v���C���[�Ɍ������x�N�g�������߂�B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�ɂ�����x�߂�������B
	if (diff.LengthSq() <= 800.0 * 800.0f)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g���𐳋K��(�傫����1)����B
		diff.Normalize();
		//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
		//����(cos��)�����߂�B
		float cos = m_forward.Dot(diff);
		//����(cos��)����p�x(��)�����߂�B
		float angle = acosf(cos);
		//�p�x(��)��120��(����p)��菬������΁B
		if (angle <= (Math::PI / 180.0f) * 135.0f)
		{
			//�v���C���[���������I
			return true;
		}
	}
	//�v���C���[���������Ȃ������B
	return false;
}

void Kisi::MakeAttackCollision()
{
	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(200.0f, 15.0f, 15.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}

void Kisi::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
   //�ҋ@���ԂƒǐՎ��Ԃ𐧌����邽�߁B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[����������B
	if (SearchPlayer() == true)
	{
		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 250.0f;

		Vector3 toPlayerDir = diff;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);

		if (m_position.y >= 0.001f)
		{
			m_moveSpeed.y = -1000.0f;
		}
		if (m_position.y <= 0.0f)
		{
			m_moveSpeed.y = 0.0f;
		}
		//�ʏ�U���ł��Ȃ������Ȃ�B
		if (IsCanAttack() == false)
		{
			//�����ɂ���āA�ǐՂ����邩���@�U�������邩���肷��B	
			int ram = rand() % 100;
			//������40�ȏ�Ȃ�B
			if (ram > 40)
			{
				//�ǐՃX�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Chase;
				return;
			}
			//������40�����Ȃ�B
			else {
				//���@�U���X�e�[�g�ɑJ�ڂ���B
				//�ǐ�
				m_enemyState = enEnemyState_Chase;
				//m_enemyState = enEnemyState_Attack;
				return;
			}
		}
		//�U���ł��鋗���Ȃ�B
		else
		{
			//�����ɂ���āA�ҋ@�����邩�ʏ�U�������邩���肷��B	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//�U���X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Attack;
				return;
			}
			else
			{
				//�ҋ@�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_enemyState = enEnemyState_Idle;
		return;

	}
}

void Kisi::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.5f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}

}

void Kisi::ProcessWalkStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Kisi::ProcessRunStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Kisi::ProcessChaseStateTransition()
{
	//�U���ł��鋗���Ȃ�B
	if (IsCanAttack() == true)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����B
	if (m_chaseTimer >= 0.5f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Kisi::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Kisi::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_enemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 250.0f;
	}
}

void Kisi::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		int radm = rand() % 100;
		if (radm >= 70) {
			Portion* portion = NewGO<Portion>(0, "portion");
			portion->SetPosition(m_position);
		}

		//����ǂݍ��ށB
		g_soundEngine->ResistWaveFileBank(1, "Assets/sound/1sibouzi.wav");
		//���ʉ����Đ�����B
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->Play(false);
		se->SetVolume(1.5f);
		//�|���ꂽ�G�l�~�[�̐���+1����B
		game->AddDefeatedEnemyNumber();
		game->SetnumEnemydesu();

		/*		if (radm > 20)
				{
					Portion* portion = NewGO<Portion>(0, "portion");
				}
		*/
		//���g���폜����B
		DeleteGO(this);
	}
}

void Kisi::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�U���������true�ɂ���B
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U���������false�ɂ���B
		m_isUnderAttack = false;
	}
}

void Kisi::ManageState()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
	}
}

void Kisi::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.0f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.0f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

const bool Kisi::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƃv���C���[�̋������߂�������B
	if (diff.LengthSq() <= 180.0f * 180.0f)
	{
		//�U���ł���I
		return true;
	}
	//�U���ł��Ȃ��B
	return false;
}


void Kisi::MODEL()
{
	m_model = 1;

}

void Kisi::Render(RenderContext& rc)
{
	if (model == true && modeltimer > 2.5f)
	{
		//���f����`�悷��B
		m_modelRender.Draw(rc);
	}
	else {
		return;
	}

	if (m_player->GetPlayerHP() != 0)
	{
		if (m_isShowHPBar == true) {
			m_spriteRender.Draw(rc);
		}
		else {
			return;
		}
	}
}