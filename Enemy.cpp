#include "Enemy.h"

void Enemy::Initialize(Model* model) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("kabotya.png");

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	//�X�P�[�����O
	worldTransform_.scale_ = { 1, 1, 1 };

	//�p�x
	worldTransform_.rotation_ = { 0, 0, 0 };

	//���W
	worldTransform_.translation_ = { 0, 5, 50 };
}

void Enemy::Update() {
	worldTransform_.UpdateMatrix();

	Move();
}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

void Enemy::Move() {
	const float kEnemySpeed = 0.2f;

	worldTransform_.translation_.z -= kEnemySpeed;
}