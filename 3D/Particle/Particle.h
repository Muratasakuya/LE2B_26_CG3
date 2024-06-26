#pragma once

// DirectX
#include "DXInclude.h"
#include "cBufferStructure.h"

// Base
#include "Object.h"

// c++
#include <random>
#include <list>

class Camera3D;

static const uint32_t instanceMaxCount_ = 100;

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

	std::list<ParticleData> particles_;
	Emitter emitter_{};
	AccelerationField accelerationField_;
	
	Matrix4x4 worldMatrix_;
	Matrix4x4 wvpMatrix_;

	uint32_t numInstance_ = 0;

	ModelData modelData_;

	std::string modelName_;
	std::string textureName_;

	BlendMode blendMode_;

	bool useBillboard_;

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};

	ParticleData MakeNewParticle(std::mt19937& randomEngine,const Vector3& translate);
	std::list<ParticleData> Emit(const Emitter& emitter, std::mt19937& randomEngine);

	bool IsCollision(const AABB& aabb, const Vector3& point);
};