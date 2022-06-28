#include "Enemy.h"
#include "player.h"

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
	worldTransform_.translation_ = { 5, 5, 50 };
}

void Enemy::Update() {
	//弾の削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->IsDead(); });

	worldTransform_.UpdateMatrix();

	//攻撃処理
	Fire();

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//移動処理
	(this->*spMoveTable[static_cast<size_t>(phase_)])();

	//Move();
}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

Vector3 Enemy::GetWorldPos() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
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

void Enemy::Fire() {
	if (isSpawn_) {
		//弾の速度
		const float kBulletSpeed = 0.5f;
		//Vector3 velocity(0, 0, -kBulletSpeed);

		//自キャラのワールド座標取得
		Vector3 playerVec = player_->GetWorldPos();

		//敵キャラのワールド座標を取得
		Vector3 enemyVec =  GetWorldPos();

		//敵キャラと自キャラの差分ベクトルを求める
		Vector3 velocity = { playerVec.x - enemyVec.x, playerVec.y - enemyVec.y, playerVec.z - enemyVec.z };

		//ベクトルを正規化
		velocity = velocity.Normalize();

		//ベクトルの長さを、速さに合わせる
		velocity *= kBulletSpeed;


		//速度ベクトルを自機の向きに合わせる
		velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
		isSpawn_ = false;
	}
	else {
		if (--spawnTimer_ < 0) {
			isSpawn_ = true;
			spawnTimer_ = kSpawnTime;
		}
	}
}

void (Enemy::* Enemy::spMoveTable[])() = {
	&Enemy::Approach,
	&Enemy::Leave
};

void Enemy::OnCollision() {

}