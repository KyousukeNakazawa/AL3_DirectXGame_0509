#include "Player.h"

float Radian(float n) {
	return n * 3.14f / 180;
}

float Degrees(float n) {
	return 180 / 3.14f * n;
}

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
	worldTransform_.scale_ = { 1, 1, 1 };

	//�p�x
	worldTransform_.rotation_ = { 0, 0, 0 };

	//���W
	worldTransform_.translation_ = { 0, 0, 0 };

	worldTransform_.UpdateMatrix();
}

void Player::Update() {
	//�e�̍폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead(); });

	worldTransform_.UpdateMatrix();

	//�ړ�����
	Move();
	//�ړ����E
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	//���񏈗�
	Rotate();

	//�U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�ړ��͈͂𐧌�
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

void Player::Draw(ViewProjection viewProjection_) {
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

Vector3 Player::GetWorldPos() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
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
	debugText_->SetPos(50, 100);
	debugText_->Printf(
		"Pos:(%f, %f, %f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Player::Rotate() {
	Vector3 root = { 0, 0, 0 };

	//�L�����N�^�[�̐��񑬓x
	const float kCharacterRoot = Radian(1.0f);

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kCharacterRoot;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kCharacterRoot;
	}

	//�f�o�b�N�p�\��
	debugText_->SetPos(50, 125);
	debugText_->Printf(
		"Rot:(%f)", worldTransform_.rotation_.y);
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹��
		velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::OnCollision() {

}