#include "Camera3D.h"

#include "ImGuiManager.h"



// transform_.translate getter
Vector3 Camera3D::GetWorldPos() const {

	return transform_.translate;
}

// cameraMatrix_ getter
Matrix4x4 Camera3D::GetCameraMatrix() const {

	return cameraMatrix_;
}

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
	transform_.rotate = { std::numbers::pi_v<float> / 10.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,4.0f,-12.0f };

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

	ImGui::Begin("3DCamera");

	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);

	ImGui::End();
}