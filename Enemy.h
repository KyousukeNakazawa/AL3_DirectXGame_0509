#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <DirectXMath.h>

class Enemy
{

	//�s���t�F�[�Y
	enum class Phase {
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	//�ϐ�
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;



	//�֐�
	//�ړ�
	void Move();

	//�ڋ�
	void Approach();

	//���E
	void Leave();
};

