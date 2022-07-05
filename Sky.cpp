#include "Sky.h"

void Sky::Initialize(Model* model) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	//�X�P�[�����O
	worldTransform_.scale_ = { 40, 40, 40 };

	//�p�x
	worldTransform_.rotation_ = { 0, 0, 0 };

	//���W
	worldTransform_.translation_ = { 0, 0, 0 };

	worldTransform_.UpdateMatrix();
}

void Sky::Update() {

}

void Sky::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}