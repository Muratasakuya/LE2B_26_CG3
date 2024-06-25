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

	void Initilize(Camera3D* camera);

	void Update(Camera3D* camera);

	void Draw(PipelineType pipelineType, const std::string& textureName, const std::optional<std::string>& modelName = std::nullopt);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	Object3DType objectType_{};

};