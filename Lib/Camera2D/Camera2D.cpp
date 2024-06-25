#include "Camera2D.h"

#include "ImGuiManager.h"



// viewMatrix_ getter
Matrix4x4 Camera2D::GetViewMatrix() const {

	return viewMatrix_;
}

// orthoMatrix_ getter
Matrix4x4 Camera2D::GetOrthoMatrix() const {

	return orthoMatrix_;
}



/*////////////////////////////////////////////////////////////////////////////////

*									初期化

////////////////////////////////////////////////////////////////////////////////*/
void Camera2D::Initialize() {

	// アフィン
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,-10.0f };

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	orthoMatrix_ =
		Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 100.0f);
}



/*////////////////////////////////////////////////////////////////////////////////

*								  更新処理

////////////////////////////////////////////////////////////////////////////////*/
void Camera2D::Update() {

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);
}



/*////////////////////////////////////////////////////////////////////////////////

*								 ImGui描画処理

////////////////////////////////////////////////////////////////////////////////*/
void Camera2D::ImGuiDraw() {

	ImGui::Begin("Camera2D");

	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::SliderFloat3("translate", &transform_.translate.x, -20.0f, 50.0f);

	ImGui::End();
}