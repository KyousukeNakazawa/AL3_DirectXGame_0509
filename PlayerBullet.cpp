#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& rotation) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	//������
	worldTransform_.Initialize();
	
	//�����Ŏ󂯎�������l���Z�b�g
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

}

void PlayerBullet::Update() {
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
