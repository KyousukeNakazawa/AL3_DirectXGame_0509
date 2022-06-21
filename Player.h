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
#include "PlayerBullet.h"
#include <memory>
#include <list>

float Radian(float n);
float Degrees(float n);

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	//ワールド座標取得
	Vector3 GetWorldPos();

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

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//関数
	//移動
	void Move();

	//旋回
	void Rotate();

	void Attack();
};

