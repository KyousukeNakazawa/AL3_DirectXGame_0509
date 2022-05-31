#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert>
#include <random>

float Radian(float n) {
	return n * 3.14 / 180;
}

float Degrees(float n) {
	return 180 / 3.14 * n;
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲（回転角用）
	std::uniform_real_distribution<float> rotDist(0.0f, Radian(360));
	//乱数範囲（座標用）
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("1-2/mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	//for (WorldTransform& worldTransform_ : worldTransform_) {
	//	//ワールドトランスフォームの初期化
	//	worldTransform_.Initialize();
	//	// x,y,zのスケーリング設定
	//	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };

	//	//Matrix4 matScale;

	////matScale.m[0][0] = worldTransform_.scale_.x;
	////matScale.m[1][1] = worldTransform_.scale_.x;
	////matScale.m[2][2] = worldTransform_.scale_.y;
	////matScale.m[3][3] = 1;

	////worldTransform_.matWorld_ *= matScale;

	////worldTransform_.TransferMatrix();

	//	// x,y,zの軸周りの回転角を設定
	//	worldTransform_.rotation_ = { rotDist(engine), rotDist(engine)  ,rotDist(engine) };

	//	//合成用回転行列を宣言
	//	Matrix4 matRot;

	//	//各軸用回転行列を宣言
	//	Matrix4 matRotX, matRotY, matRotZ;

	//	matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	//	matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	//	matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	//	matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	//	matRotZ.m[2][2] = 1;
	//	matRotZ.m[3][3] = 1;

	//	matRotX.m[0][0] = 1;
	//	matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	//	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	//	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	//	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	//	matRotX.m[3][3] = 1;

	//	matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	//	matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	//	matRotY.m[1][1] = 1;
	//	matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	//	matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	//	matRotY.m[3][3] = 1;

	//	//各軸の回転行列を合成
	//	//matRot = matRotZ * matRotX * matRotY;

	//	//worldTransform_.matWorld_ = matRotZ;
	//	worldTransform_.matWorld_ *= matRotZ;
	//	worldTransform_.matWorld_ *= matRotX;
	//	worldTransform_.matWorld_ *= matRotY;

	//	worldTransform_.TransferMatrix();

	//	// x,y,z軸周りの平行移動を設定
	//	worldTransform_.translation_ = { posDist(engine), posDist(engine), posDist(engine) };

	//	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//	matTrans.m[0][0] = 1;
	//	matTrans.m[1][1] = 1;
	//	matTrans.m[2][2] = 1;
	//	matTrans.m[3][3] = 1;
	//	matTrans.m[3][0] = worldTransform_.translation_.x;
	//	matTrans.m[3][1] = worldTransform_.translation_.y;
	//	matTrans.m[3][2] = worldTransform_.translation_.z;

	//	worldTransform_.matWorld_ *= matTrans;

	//	worldTransform_.TransferMatrix();
	//}

	//キャラクターの大元
	worldTransform_[PartId::kRoot].translation_ = { 0, 0, 0 };
	worldTransform_[PartId::kRoot].rotation_ = { 0, 0.5f, 0 };
	worldTransform_[PartId::kRoot].Initialize();

	//脊椎
	worldTransform_[PartId::kSpine].translation_ = { 0, 3.0f, 0 };
	worldTransform_[PartId::kSpine].parent_ = &worldTransform_[PartId::kRoot];
	worldTransform_[PartId::kSpine].Initialize();

	//上半身
	worldTransform_[PartId::kChest].translation_ = worldTransform_[PartId::kSpine].translation_;
	worldTransform_[PartId::kChest].parent_ = &worldTransform_[PartId::kSpine];

	worldTransform_[PartId::kHead].translation_ = { 0, 3.0f, 0 };
	worldTransform_[PartId::kHead].parent_ = &worldTransform_[PartId::kChest];

	worldTransform_[PartId::kArmL].translation_ = { 3.0f, 0, 0 };
	worldTransform_[PartId::kArmL].parent_ = &worldTransform_[PartId::kChest];

	worldTransform_[PartId::kArmR].translation_ = { -3.0f, 0, 0 };
	worldTransform_[PartId::kArmR].parent_ = &worldTransform_[PartId::kChest];

	//下半身
	worldTransform_[PartId::kHip].translation_ = { 0, 0, 0 };
	worldTransform_[PartId::kHip].parent_ = &worldTransform_[PartId::kSpine];

	worldTransform_[PartId::kLegL].translation_ = { 3.0f, -3.0f, 0 };
	worldTransform_[PartId::kLegL].parent_ = &worldTransform_[PartId::kHip];

	worldTransform_[PartId::kLegR].translation_ = { -3.0f, -3.0f, 0 };
	worldTransform_[PartId::kLegR].parent_ = &worldTransform_[PartId::kHip];

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	////カメラの視点座標を設定
	//viewProjection_.eye = { 0, 0, -50 };

	////カメラ注視点座標を設定
	//viewProjection_.target = { 10, 0, 0 };

	////カメラ上方向ベクトルを設定（右上45度指定）
	//viewProjection_.up = { cosf(Radian(180) / 4.0f), sinf(Radian(180) / 4.0f), 0.0f };

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Radian(10.0f);

	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;

	//ファークリップ距離を設定
	viewProjection_.farZ = 51.0f;
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();

	//大元から順に更新
	for (int i = 0; i < kNumpartId; i++) {
		worldTransform_[i]
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//3Dモデル描画
	for (WorldTransform& worldTransform_ : worldTransform_) {
		model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	}

	//PrimitiveDrawer::GetInstance()->DrawLine3d(start, end, color);


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}