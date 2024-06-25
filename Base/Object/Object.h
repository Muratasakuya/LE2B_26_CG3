#pragma once

// DirectX
#include "DXInclude.h"
#include "CBufferStructure.h"
#include "VertexResource.h"

// c++
#include <memory>
#include <string>
#include <optional>

// Transform構造体
struct Transform {

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// オブジェクトタイプ
enum class Object3DType {

	Triangle,
	Sphere,
	Model
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

	void SetEnableLighting(bool enableLighting);
	void SetEnableHalfLambert(bool enableHalfLambert);
	void SetScale(Vector3 scale);
	void SetRotate(Vector3 rotate);
	void SetTranslate(Vector3 translate);
	void SetTransform(Transform transform);

protected:
	/*-----------------------------*/
	///			アクセス修飾子
	/*-----------------------------*/

	// CreateBuffer
	std::unique_ptr<VertexResource> vertexResource_;

	// バッファデータ
	std::unique_ptr<CBufferData> cBuffer_ = nullptr;

	// matrix
	TransformationMatrix matrix_{};

	// アフィン
	Transform transform_{};

	// 色
	Vector4 color_{};

	// ライトの向き
	Vector3 lightDirection_{};
	// Lightingの有無
	bool enableLighting_{};
	// HalfLambertの有無
	bool enableHalfLambert_{};
};