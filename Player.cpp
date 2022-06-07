#include "Player.h"

float Radian(float n) {
	return n * 3.14f / 180;
}

float Degrees(float n) {
	return 180 / 3.14f * n;
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	//スケーリング
	worldTransform_.scale_ = { 1, 1, 1};

	//角度
	worldTransform_.rotation_ = { 0, 0, 0 };

	//座標
	worldTransform_.translation_ = { 0, 0, 0 };

	worldTransform_.UpdateMatrix();
}

void Player::Update() {
	worldTransform_.UpdateMatrix();

	//移動処理
	Move();
	//移動限界
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	//旋回処理
	Rotate();

	//攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

void Player::Draw(ViewProjection viewProjection_) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::Move() {
	Vector3 move = { 0, 0, 0 };

	//キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトル変更
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

	//注視点移動
	worldTransform_.translation_ += move;

	//デバック用表示
	debugText_->SetPos(50, 100);
	debugText_->Printf(
		"Pos:(%f, %f, %f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Player::Rotate() {
	Vector3 root = { 0, 0, 0 };

	//キャラクターの旋回速度
	const float kCharacterRoot = Radian(1.0f);

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kCharacterRoot;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kCharacterRoot;
	}

	//デバック用表示
	debugText_->SetPos(50, 125);
	debugText_->Printf(
		"Rot:(%f)", worldTransform_.rotation_.y);
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, worldTransform_.rotation_);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
}