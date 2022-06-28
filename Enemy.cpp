#include "Enemy.h"
#include "player.h"

void Enemy::Initialize(Model* model) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	//�X�P�[�����O
	worldTransform_.scale_ = { 1, 1, 1 };

	//�p�x
	worldTransform_.rotation_ = { 0, 0, 0 };

	//���W
	worldTransform_.translation_ = { 5, 5, 50 };
}

void Enemy::Update() {
	//�e�̍폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->IsDead(); });

	worldTransform_.UpdateMatrix();

	//�U������
	Fire();

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�ړ�����
	(this->*spMoveTable[static_cast<size_t>(phase_)])();

	//Move();
}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//�e�`��
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
	//����̈ʒu�ŗ��E
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
		//�e�̑��x
		const float kBulletSpeed = 0.5f;
		//Vector3 velocity(0, 0, -kBulletSpeed);

		//���L�����̃��[���h���W�擾
		Vector3 playerVec = player_->GetWorldPos();

		//�G�L�����̃��[���h���W���擾
		Vector3 enemyVec =  GetWorldPos();

		//�G�L�����Ǝ��L�����̍����x�N�g�������߂�
		Vector3 velocity = { playerVec.x - enemyVec.x, playerVec.y - enemyVec.y, playerVec.z - enemyVec.z };

		//�x�N�g���𐳋K��
		velocity = velocity.Normalize();

		//�x�N�g���̒������A�����ɍ��킹��
		velocity *= kBulletSpeed;


		//���x�x�N�g�������@�̌����ɍ��킹��
		velocity = MathUtility::Vector3TransformNormal(velocity, worldTransform_.matWorld_);

		//�e�𐶐����A������
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//�e��o�^����
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