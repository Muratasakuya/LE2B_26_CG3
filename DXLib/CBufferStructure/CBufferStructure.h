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

// Matrix構造体
struct TransformationMatrix {

	Matrix4x4 World;
	Matrix4x4 WVP;
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
	float respawnTime;
	float respawnDelay;
};

// 平行光源構造体
struct DirectionalLight {

	Vector4 color;
	Vector3 direction;
	float intensity;
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

// CBデータ
struct CBufferData {

	std::unique_ptr<CBMaterialData> material;
	std::unique_ptr<CBTransformData> matrix;
	std::unique_ptr<CBParticleTransformData> particleMatrix;
	std::unique_ptr<CBLightData> light;
};