#pragma once


class TIMER :public IGameObject
{
public:
	TIMER();
	~TIMER();
	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);

	//�������烁���o�ϐ��B
	ModelRender m_modelRender;            //���f�������_�[�B
	FontRender fontRender2;               //�����B
	float m_timer = 0.0f;                  //�^�C�}�[�B
};
