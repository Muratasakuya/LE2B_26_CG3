#pragma once

// DirectX
#include "DXInclude.h"

// Lib
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

// c++
#include <vector>
#include <memory>
#include <string>

// Transform構造体
struct Transform {

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// 頂点データ構造体
struct VertexData {

	Vector4 pos;
	Vector2 texcoord;
	Vector3 normal;
};

// マテリアル構造体
struct Material {

	Vector4 color;
	int32_t enableLighting;
	int32_t enableHalfLambert;
	float padding[2];
	Matrix4x4 uvTransform;
};

// PhongReflectionマテリアル
struct PhongRefMaterial {

	Vector4 color;
	int32_t enableLighting;
	int32_t enablePhongReflection;
	int32_t enableBlinnPhongReflection;
	float padding;
	Matrix4x4 uvTransform;
	Vector3 specularColor;
	float shininess;
};

// Matrix構造体
struct TransformationMatrix {

	Matrix4x4 World;
	Matrix4x4 WVP;
	Matrix4x4 WorldInverseTranspose;
};

// パーティクル用のMatrix構造体
struct ParticleForGPU {

	Matrix4x4 World;
	Matrix4x4 WVP;
	Vector4 color;
};

// パーティクル用構造体
struct ParticleData {

	Transform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

// パーティクルエミッタ構造体
struct Emitter {

	Transform transform;
	uint32_t count;
	float frequency;
	float frequencyTime;
};

// AABB構造体
struct AABB {

	Vector3 min;
	Vector3 max;
};

// 加速フィールド構造体
struct AccelerationField {

	Vector3 acceleration;
	AABB area;
};

// 平行光源構造体
struct DirectionalLight {

	Vector4 color;
	Vector3 direction;
	float intensity;
};

// 点光源構造体
struct PointLight {

	Vector4 color;
	Vector3 pos;
	float intensity;
	float radius;
	float decay;
	float padding[2];
};

// スポットライト構造体 懐中電灯のようなもの
struct SpotLight {

	Vector4 color;
	Vector3 pos;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosAngle;
	float padding[2];
};

// カメラワールド座標構造体
struct CameraForGPU {

	Vector3 worldPosition;
};

// モデルマテリアルデータ構造体
struct ModelMaterialData {

	std::string textureFilePath;
};

// モデルデータ構造体
struct ModelData {

	std::vector<VertexData> vertices;
	ModelMaterialData material;
};

// CBマテリアルデータ
struct CBMaterialData {

	// 頂点マテリアル
	ComPtr<ID3D12Resource> resource;

	// マテリアルマップ
	Material* data = nullptr;
};

// CBPhongReflectionマテリアルデータ
struct CBPhongRefMaterialData {

	// 頂点マテリアル
	ComPtr<ID3D12Resource> resource;

	// マテリアルマップ
	PhongRefMaterial* data = nullptr;
};

// CBトランスフォームデータ
struct CBTransformData {

	// WVP
	ComPtr<ID3D12Resource> resource;

	// 行列
	TransformationMatrix* data = nullptr;
};

//CBパーティクル用トランスフォームデータ
struct CBParticleTransformData {

	// WVP
	ComPtr<ID3D12Resource> resource;

	ParticleForGPU* particleData = nullptr;
};

// CBライトデータ
struct CBLightData {

	// Light
	ComPtr<ID3D12Resource> resource;

	// Light
	DirectionalLight* data = nullptr;
};

// CBポイントライトデータ
struct CBPointLightData {

	// PointLight
	ComPtr<ID3D12Resource> resource;

	// PointLight
	PointLight* data = nullptr;
};

// CBスポットライトデータ
struct CBSpotLightData {

	// SpotLight
	ComPtr<ID3D12Resource> resource;

	// SpotLight
	SpotLight* data = nullptr;
};

// CBカメラデータ
struct CBCameraData {

	// Camera
	ComPtr<ID3D12Resource> resource;

	// Camera
	CameraForGPU* data = nullptr;
};

// CBデータ
struct CBufferData {

	std::unique_ptr<CBMaterialData> material;
	std::unique_ptr<CBPhongRefMaterialData> phongRefMaterial;
	std::unique_ptr<CBTransformData> matrix;
	std::unique_ptr<CBParticleTransformData> particleMatrix;
	std::unique_ptr<CBLightData> light;
	std::unique_ptr<CBPointLightData> pointLight;
	std::unique_ptr<CBSpotLightData> spotLight;
	std::unique_ptr<CBCameraData> camera;
};