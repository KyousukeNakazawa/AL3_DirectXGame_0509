#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <cassert>
#include <random>



GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete skymodel_;
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
	skymodel_ = Model::CreateFromOBJ("kamata", true);

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

	//初期化
	Player* newPlayer = new Player();
	newPlayer->Initialize(model_, textureHandle_);
	player_.reset(newPlayer);

	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_);
	enemy_.reset(newEnemy);

	Sky* newSky = new Sky();
	newSky->Initialize(skymodel_);
	sky_.reset(newSky);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());
}

void GameScene::Update() {
	//#ifdef _DEBUG
	//	if (input_->TriggerKey(DIK_P)) {
	//		isDebugCameraActive_ = true;
	//	}
	//#endif
	//
	//	if (isDebugCameraActive_) {
	//		//デバッグカメラの更新
	//		debugCamera_->Update();
	//		viewProjection_.matView = debugCamera_->GetViewProjection();
	//		viewProjection_.matProjection = debugCamera_.m
	//	}
		//デバッグカメラの更新
	debugCamera_->Update();

	//自キャラの更新
	player_->Update();

	//敵キャラ更新
	if (enemy_) {
		enemy_->Update();
	}
	CheckAllCollisions();
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

	sky_->Draw(debugCamera_->GetViewProjection());

	//3Dモデル描画
	if (enemy_) {
		enemy_->Draw(debugCamera_->GetViewProjection());
	}

	player_->Draw(debugCamera_->GetViewProjection());

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

void GameScene::CheckAllCollisions() {
	//判定対象の座標
	Vector3 posA, posB;

	//自弾リスト取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	//自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPos();

	//当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPos();
		//距離を求める
		float d = MathUtility::Distance(posA, posB);
		float r = (1 + 1) * (1 + 1);
		if (d <= r) {
			player_->OnCollision();

			bullet->OnCollision();
		}
	}

	//自弾と敵キャラの当たり判定
	posA = enemy_->GetWorldPos();

	//当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPos();
		//距離を求める
		float d = MathUtility::Distance(posA, posB);
		float r = (1 + 1) * (1 + 1);
		if (d <= r) {
			enemy_->OnCollision();

			bullet->OnCollision();
		}
	}

	//自弾と敵弾の当たり判定
	//当たり判定
	for (const std::unique_ptr<PlayerBullet>& bulletP : playerBullets) {
		posA = bulletP->GetWorldPos();
		for (const std::unique_ptr<EnemyBullet>& bulletE : enemyBullets) {
			//敵弾の座標
			posB = bulletE->GetWorldPos();
			//距離を求める
			float d = MathUtility::Distance(posA, posB);
			float r = (1 + 1) * (1 + 1);
			if (d <= r) {
				bulletP->OnCollision();

				bulletE->OnCollision();
			}
		}
	}
}