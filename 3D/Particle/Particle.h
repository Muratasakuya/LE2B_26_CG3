#pragma once

// DirectX
#include "DXInclude.h"

// Base
#include "Object.h"

class Camera3D;

static const uint32_t instanceCount_ = 10;;

/*////////////////////////////////////////////////////////////////////////////////
*
*								Particle Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Particle
	:public Object {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	Particle();
	~Particle();

	void Initialize(Camera3D* camera);
	void InitializeDXSrvDesc();

	void Update(Camera3D* camera);

	void Draw();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	Transform transforms_[instanceCount_];

	ModelData modelData_;

	std::string modelName_;
	std::string textureName_;

	BlendMode blendMode_;

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};
};