#include "Enemy.h"

void Enemy::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("kabotya.png");

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	//スケーリング
	worldTransform_.scale_ = { 1, 1, 1 };

	//角度
	worldTransform_.rotation_ = { 0, 0, 0 };

	//座標
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