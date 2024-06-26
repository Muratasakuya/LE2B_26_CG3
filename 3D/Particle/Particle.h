#pragma once

// DirectX
#include "DXInclude.h"
#include "cBufferStructure.h"

// Base
#include "Object.h"

// c++
#include <random>

class Camera3D;

static const uint32_t instanceMaxCount_ = 10;

// Δt
static const float kDeltaTime = 1.0f / 60.0f;

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

	Particle(Camera3D* camera);
	~Particle();

	void Initialize(Camera3D* camera);
	void InitializeDXSrvDesc();

	void Update(Camera3D* camera);

	void Draw();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	ParticleData particles_[instanceMaxCount_];

	uint32_t numInstance_ = 0;

	ModelData modelData_;

	std::string modelName_;
	std::string textureName_;

	BlendMode blendMode_;

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};

	ParticleData MakeNewParticle(std::mt19937& randomEngine);
};