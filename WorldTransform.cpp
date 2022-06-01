#include "DirectXCommon.h"
#include "WorldTransform.h"
#include <cassert>
#include <d3dx12.h>
#include "AxisIndicator.h"

void WorldTransform::UpdateMatrix() {
	Matrix4 matScale, matRot, matTrans;
	matScale = MathUtility::Matrix4Scaling(scale_);
	matRot = MathUtility::Matrix4Rotation(rotation_);
	matTrans = MathUtility::Matrix4Translation(translation_);

	matWorld_ = MathUtility::Matrix4Identity();
	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;

	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}

	TransferMatrix();
}