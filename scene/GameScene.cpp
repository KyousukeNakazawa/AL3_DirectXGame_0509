#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert>
#include <random>

float Radian(float n) {
	return n * 3.14 / 180;
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

	for (WorldTransform& worldTransform_ : worldTransform_) {
		//ワールドトランスフォームの初期化
		worldTransform_.Initialize();
		// x,y,zのスケーリング設定
		worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };

		//Matrix4 matScale;

	//matScale.m[0][0] = worldTransform_.scale_.x;
	//matScale.m[1][1] = worldTransform_.scale_.x;
	//matScale.m[2][2] = worldTransform_.scale_.y;
	//matScale.m[3][3] = 1;

	//worldTransform_.matWorld_ *= matScale;

	//worldTransform_.TransferMatrix();

		// x,y,zの軸周りの回転角を設定
		worldTransform_.rotation_ = { rotDist(engine), rotDist(engine)  ,rotDist(engine) };

		//合成用回転行列を宣言
		Matrix4 matRot;

		//各軸用回転行列を宣言
		Matrix4 matRotX, matRotY, matRotZ;

		matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
		matRotX.m[3][3] = 1;

		matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
		matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
		matRotY.m[1][1] = 1;
		matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
		matRotY.m[3][3] = 1;

		//各軸の回転行列を合成
		//matRot = matRotZ * matRotX * matRotY;

		//worldTransform_.matWorld_ = matRotZ;
		worldTransform_.matWorld_ *= matRotZ;
		worldTransform_.matWorld_ *= matRotX;
		worldTransform_.matWorld_ *= matRotY;

		worldTransform_.TransferMatrix();

		// x,y,z軸周りの平行移動を設定
		worldTransform_.translation_ = { posDist(engine), posDist(engine), posDist(engine) };

		Matrix4 matTrans = MathUtility::Matrix4Identity();

		matTrans.m[0][0] = 1;
		matTrans.m[1][1] = 1;
		matTrans.m[2][2] = 1;
		matTrans.m[3][3] = 1;
		matTrans.m[3][0] = worldTransform_.translation_.x;
		matTrans.m[3][1] = worldTransform_.translation_.y;
		matTrans.m[3][2] = worldTransform_.translation_.z;

		worldTransform_.matWorld_ *= matTrans;

		worldTransform_.TransferMatrix();
	}

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

	////視点移動処理
	//{
	//	//視点の移動ベクトル
	//	Vector3 move = { 0, 0, 0 };

	//	//視点の移動速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move.z += kEyeSpeed;
	//	}
	//	else if (input_->PushKey(DIK_S)) {
	//		move.z -= kEyeSpeed;
	//	}

	//	//視点移動（ベクトルの加算）
	//	viewProjection_.eye += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック表示用
	//	debugText_->SetPos(50, 50);
	//	debugText_->Printf("eye:(%f, %f, %f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//}

	////注視点移動処理
	//{
	//	//注視点の移動ベクトル
	//	Vector3 move = { 0, 0, 0 };

	//	//注視点の移動速さ
	//	const float kTargetSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move = { -kTargetSpeed, 0, 0 };
	//	}
	//	else if (input_->PushKey(DIK_RIGHT)) {
	//		move = { kTargetSpeed, 0, 0 };
	//	}

	//	//注視点移動（ベクトルの加算）
	//	viewProjection_.target += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック表示用
	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf("targe:(%f, %f, %f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	//}

	////上方向回転処理
	//{
	//	//上方向の回転速さ[ラジアン/frame]
	//	const float kUpRotSpeed = 0.05f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_SPACE)) {
	//		viewAngle += kUpRotSpeed;
	//		//2πを超えたら0に戻す
	//		viewAngle = fmodf(viewAngle, Radian(360));
	//	}

	//	//上方向ベクトルを計算（半径1の円周上の座標）
	//	viewProjection_.up = { cosf(viewAngle), sinf(viewAngle), 0.0f };

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバック表示用
	//	debugText_->SetPos(50, 90);
	//	debugText_->Printf("up:(%f, %f, %f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	//}

	//Fov変更処理
	{
		//Wキーで視野角が広がる
		if (input_->PushKey(DIK_W)) {
			viewProjection_.fovAngleY += 0.01f;
			viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, Radian(180));
		}
		//Sキーで視野角がが狭まる
		else if (input_->PushKey(DIK_S)) {
			viewProjection_.fovAngleY -= 0.01f;
			viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngleY(Degree):%f", viewProjection_.fovAngleY);
	}

	//クリップ距離変更処理
	{
		//上下キーでニアクリップ距離を増減
		if (input_->PushKey(DIK_UP)) {
			viewProjection_.nearZ += 0.1f;
		}
		else if (input_->PushKey(DIK_DOWN)) {
			viewProjection_.nearZ -= 0.1f;
		}

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 130);
		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
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