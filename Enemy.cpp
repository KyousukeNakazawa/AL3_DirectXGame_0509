#include "Enemy.h"

void Enemy::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");

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

	(this->*spMoveTable[static_cast<size_t>(phase_)])();

	//Move();
}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

void Enemy::Move() {
	const float kEnemySpeed = 0.2f;

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}
}

void Enemy::Approach() {
	worldTransform_.translation_.z -= 0.2f;
	//特定の位置で離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	worldTransform_.translation_.x -= 0.1f;
	worldTransform_.translation_.y += 0.1f;
}

void (Enemy::* Enemy::spMoveTable[])() = {
	&Enemy::Approach,
	&Enemy::Leave 
};