#pragma once

// DirectX
#include "DXInclude.h"

// Lib
#include "Vector3.h"
#include "Vector4.h"

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
	float cosFalloffStart;
	float padding[2];
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