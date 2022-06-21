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
#include "PlayerBullet.h"
#include <memory>
#include <list>

float Radian(float n);
float Degrees(float n);

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	//���[���h���W�擾
	Vector3 GetWorldPos();

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

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//�֐�
	//�ړ�
	void Move();

	//����
	void Rotate();

	void Attack();
};

