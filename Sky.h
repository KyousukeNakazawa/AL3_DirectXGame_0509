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

class Sky
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	
	//model_->Draw(worldTransform_, viewProjection_);
};

