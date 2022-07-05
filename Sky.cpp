#include "Sky.h"

void Sky::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	//スケーリング
	worldTransform_.scale_ = { 40, 40, 40 };

	//角度
	worldTransform_.rotation_ = { 0, 0, 0 };

	//座標
	worldTransform_.translation_ = { 0, 0, 0 };

	worldTransform_.UpdateMatrix();
}

void Sky::Update() {

}

void Sky::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}