#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include <DirectXMath.h>
#include "EnemyBullet.h"
#include "TimedCall.h"
#include <memory>
#include <list>


//自機クラスの前方宣言
class Player;

class Enemy
{

	//行動フェーズ
	enum class Phase {
		Approach,	//接近する
		Leave,		//離脱する
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	Vector3 GetWorldPos();

	void SetPlayer(Player* player) { player_ = player; }

private:
	//変数
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//スポーンタイム
	static const int32_t kSpawnTime = 60 * 2;
	//スポーンタイマー
	int32_t spawnTimer_ = kSpawnTime;
	//スポーンフラグ
	bool isSpawn_ = true;

	//自キャラ
	Player* player_ = nullptr;

	//関数
	//移動
	void Move();

	//接近
	void Approach();

	//離脱
	void Leave();

	//射撃
	void Fire();

	//メンバ関数ポインタのテーブル
	static void (Enemy::* spMoveTable[])();
};

