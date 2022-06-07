#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "MathUtility.h"
#include <cassert>

//Matrix4 MathUtility::Matrix4Identity() {
//	Matrix4 matrix;
//	matrix.m[0][0] = 1;
//	matrix.m[1][1] = 1;
//	matrix.m[2][2] = 1;
//	matrix.m[3][3] = 1;
//
//	return matrix;
//}

Matrix4 MathUtility::Matrix4Scaling(Vector3 scale) {
	Matrix4 matScale = MathUtility::Matrix4Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 MathUtility::Matrix4Rotation(Vector3 rotation) {
	//合成用回転行列を宣言
	Matrix4 matRot = MathUtility::Matrix4Identity();

	//各軸用回転行列を宣言
	Matrix4 matRotX, matRotY, matRotZ;

	matRotX = MathUtility::Matrix4Identity();
	matRotY = MathUtility::Matrix4Identity();
	matRotZ = MathUtility::Matrix4Identity();

	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);

	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);

	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);

	matRot = matRotZ * matRotX * matRotY;

	return matRot;
}

Matrix4 MathUtility::Matrix4Translation(Vector3 translation) {
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = translation.x;
	matTrans.m[3][1] = translation.y;
	matTrans.m[3][2] = translation.z;

	return matTrans;
}