#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position,
	const Vector3& velocity) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	//������
	worldTransform_.Initialize();

	//�����Ŏ󂯎�������l���Z�b�g
	worldTransform_.translation_ = position;
	//worldTransform_.rotation_ = rotation;
	velocity_ = velocity;

}

void EnemyBullet::Update() {
	worldTransform_.UpdateMatrix();

	//���W���ړ�������
	worldTransform_.translation_ += velocity_;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}