#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position,
	const Vector3& velocity) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("red.png");

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

void EnemyBullet::OnCollision() {
	isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPos() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}