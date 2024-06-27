#pragma once

// Base
#include "Object.h"

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

	Object3D(Object3DType objectType);
	~Object3D();

	void Initialize(Camera3D* camera);

	void UpdateImGui(const std::string& objectName);

	void Update(Camera3D* camera);

	void Draw(PipelineType pipelineType, BlendMode blendMode, const std::string& textureName, const std::optional<std::string>& modelName = std::nullopt);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	Object3DType objectType_{};

	Vector3 specularColor_{};
	float shininess_{};

};