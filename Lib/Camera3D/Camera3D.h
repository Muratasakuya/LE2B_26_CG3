#pragma once

// 3D
#include "Object3D.h"

// Lib
#include "Matrix4x4.h"

/*////////////////////////////////////////////////////////////////////////////////
*
*								Camera3D Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Camera3D {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void Initialize();
	void Update();
	void ImGuiDraw();

	// getter

	Matrix4x4 GetCameraMatrix() const;
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetProjectionMatrix() const;

public:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	Matrix4x4 cameraMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;

	Transform transform_;
};
