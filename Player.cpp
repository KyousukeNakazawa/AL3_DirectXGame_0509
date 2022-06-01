#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	//�X�P�[�����O
	worldTransform_.scale_ = { 1, 1, 1};

	//�p�x
	worldTransform_.rotation_ = { 0, 0, 0 };

	//���W
	worldTransform_.translation_ = { 0, 0, 0 };

	worldTransform_.UpdateMatrix();
}

void Player::Update() {
	worldTransform_.UpdateMatrix();

	//�ړ�����
	Player::Move();
	//�ړ����E
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

void Player::Draw(ViewProjection viewProjection_) {
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

void Player::Move() {
	Vector3 move = { 0, 0, 0 };

	//�L�����N�^�[�̈ړ����x
	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��x�N�g���ύX
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//�����_�ړ�
	worldTransform_.translation_ += move;

	//�f�o�b�N�p�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"Root:(%f, %f, %f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}