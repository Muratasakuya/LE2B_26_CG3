#pragma once

// Base
#include "Object.h"

class ConstBufferManager;
class Camera3D;

/*////////////////////////////////////////////////////////////////////////////////
*
*								Object3D Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Object3D :
	public Object {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	Object3D();
	~Object3D();

	void Initialize(
		Camera3D* camera,const Object3DType& objectType,const PipelineType& drawType,const BlendMode& blendMode,
		const std::string& textureName, const std::optional<std::string>& modelName = std::nullopt);
	void InitializeCBuffer(const std::string& name);

	void UpdateImGui(const std::string& objectName);

	void Update(Camera3D* camera);

	void Draw();

	// setter

	void SetIsUseGLTFModel(bool useGltfModel,const std::string& modelName);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	ConstBufferManager* cBuffer_ = nullptr;

	// 3Dオブジェクトタイプ
	Object3DType objectType_{};

	// 描画タイプ
	PipelineType drawType_{};

	// ブレンドモード
	BlendMode blendMode_{};

	// 3Dオブジェクトに使用するもの
	std::string textureName_;
	std::string modelName_;
	std::string gltfModelName_;
	bool isUseGltfModel_{};

};