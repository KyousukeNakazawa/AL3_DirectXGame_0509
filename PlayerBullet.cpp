#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position,
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

void PlayerBullet::Update() {
	worldTransform_.UpdateMatrix();

	//���W���ړ�������
	worldTransform_.translation_ += velocity_;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
