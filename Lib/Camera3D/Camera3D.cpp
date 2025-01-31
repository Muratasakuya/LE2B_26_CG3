#include "Camera3D.h"

#include "ImGuiManager.h"



// viewMatrix_ getter
Matrix4x4 Camera3D::GetViewMatrix() const {

	return viewMatrix_;
}

// projectionMatrix_ getter
Matrix4x4 Camera3D::GetProjectionMatrix() const {

	return projectionMatrix_;
}



/*////////////////////////////////////////////////////////////////////////////////

*									初期化

////////////////////////////////////////////////////////////////////////////////*/
void Camera3D::Initialize() {

	// アフィン
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,-10.0f };

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	projectionMatrix_ =
		Matrix4x4::MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
}



/*////////////////////////////////////////////////////////////////////////////////

*								  更新処理

////////////////////////////////////////////////////////////////////////////////*/
void Camera3D::Update() {

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);
}



/*////////////////////////////////////////////////////////////////////////////////

*								 ImGui描画処理

////////////////////////////////////////////////////////////////////////////////*/
void Camera3D::ImGuiDraw() {

	ImGui::Begin("Camera3D");

	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::SliderFloat3("translate", &transform_.translate.x, -20.0f, 50.0f);

	ImGui::End();
}