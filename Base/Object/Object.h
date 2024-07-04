#pragma once

// DirectX
#include "DXInclude.h"
#include "CBufferStructure.h"
#include "VertexResource.h"

// c++
#include <memory>
#include <string>
#include <optional>
#include <numbers>
#include <filesystem>

// 3Dオブジェクトタイプ
enum class Object3DType {

	Triangle,
	Sphere,
	Model,
	GS
};

/*////////////////////////////////////////////////////////////////////////////////
*
*								Object Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Object{
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	Object() {}
	virtual ~Object() {}

	// setter

	/*--------------------------------------------------------------------------*/
	/// LightingEnableSetting ↓

	void SetEnableLighting(bool enableLighting);
	void SetEnableHalfLambert(bool enableHalfLambert);
	void SetEnablePhongReflection(bool enablePhongReflection);
	void SetEnableBlinnPhongReflection(bool enableBlinnPhongReflection);

	/*--------------------------------------------------------------------------*/
	/// LightSetting ↓

	void SetPointLight(PointLight pointLight);
	void SetSpotLight(SpotLight spotLight);

	/*--------------------------------------------------------------------------*/
	/// TransformSetting ↓

	void SetScale(Vector3 scale);
	void SetRotate(Vector3 rotate);
	void SetTranslate(Vector3 translate);
	void SetTransform(Transform transform);

	/*--------------------------------------------------------------------------*/

protected:
	/*-----------------------------*/
	///			アクセス修飾子
	/*-----------------------------*/

	// CreateBuffer
	VertexResource vertexResource_;

	// AffineMatrix
	TransformationMatrix matrix_{};
	// GS
	GSMatrix4x4 gsMat{};

	// SRT
	Transform transform_{};

	// Material
	Material material_{};
	// PhongReflectionMaterial
	PhongRefMaterial phongRef_{};

#pragma region ///*  LightVariable  *///

	// 平行光源
	DirectionalLight directionlLight_{};
	// ポイントライト
	PointLight pointLight_{};
	// スポットライト
	SpotLight spotLight_{};

	// 全てのライト
	PunctualLight light_{};

#pragma endregion

	// カメラワールド座標
	Vector3 cameraWorldPos_{};

	// オブジェクトの名前
	std::string name_;

};