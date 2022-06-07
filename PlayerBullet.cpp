#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();
	
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
