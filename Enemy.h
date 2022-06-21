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
#include "EnemyBullet.h"
#include "TimedCall.h"
#include <memory>
#include <list>


//���@�N���X�̑O���錾
class Player;

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

	Vector3 GetWorldPos();

	void SetPlayer(Player* player) { player_ = player; }

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

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//�X�|�[���^�C��
	static const int32_t kSpawnTime = 60 * 2;
	//�X�|�[���^�C�}�[
	int32_t spawnTimer_ = kSpawnTime;
	//�X�|�[���t���O
	bool isSpawn_ = true;

	//���L����
	Player* player_ = nullptr;

	//�֐�
	//�ړ�
	void Move();

	//�ڋ�
	void Approach();

	//���E
	void Leave();

	//�ˌ�
	void Fire();

	//�����o�֐��|�C���^�̃e�[�u��
	static void (Enemy::* spMoveTable[])();
};

