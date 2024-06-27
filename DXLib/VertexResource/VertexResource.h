#pragma once

// DirectX
#include "DXInclude.h"
#include "CBufferStructure.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*
*								VertexResource Class
*
////////////////////////////////////////////////////////////////////////////////*/
class VertexResource{
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// マテリアル生成
	std::unique_ptr<CBMaterialData> CreateMaterial();

	// PhongReflectionマテリアル生成
	std::unique_ptr<CBPhongRefMaterialData> CreatePhongRefMaterial();

	// WVP生成
	std::unique_ptr<CBTransformData> CreateWVP();

	// パーティクル用WVP生成
	std::unique_ptr<CBParticleTransformData> CreateParticleWVP(const uint32_t instanceNum);

	// Light生成
	std::unique_ptr<CBLightData> CreateLight();

	// PointLight生成
	std::unique_ptr<CBPointLightData> CreatePointLight();

	// Camera生成
	std::unique_ptr<CBCameraData> CreateCamera();

	// BufferResource生成
	ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

};