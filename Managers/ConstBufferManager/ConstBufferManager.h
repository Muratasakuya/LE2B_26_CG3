#pragma once

// DirectX
#include "DXInclude.h"
#include "CBufferStructure.h"
#include "VertexResource.h"

// c++
#include <memory>
#include <unordered_map>
#include <string>


/*////////////////////////////////////////////////////////////////////////////////
*
*							ConstBufferManager Class
*
////////////////////////////////////////////////////////////////////////////////*/
class ConstBufferManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// ConstBufferの生成
	void CreateConstBufferData(const std::string& name);

	// ConstBufferの初期化
	void InitializeConstBuffer(
		const std::string& name, const Material& material, const PhongRefMaterial& phongRef_,
		const TransformationMatrix& matrix, const GSMatrix4x4& gsMat, const PunctualLight& light,
		const Vector3& cameraWorldPos, const PipelineType& drawType, bool isUseGLTFModel);

	// ConstBufferの更新
	void UpdateConstBuffer(
		const std::string& name, const Material& material, const PhongRefMaterial& phongRef_,
		const TransformationMatrix& matrix, const GSMatrix4x4& gsMat, const PunctualLight& light,
		const Vector3& cameraWorldPos, const PipelineType& drawType, bool isUseGLTFModel);

	// DrawCommand
	void SetConstBuffer(
		ID3D12GraphicsCommandList* commandList,const PipelineType& pipelineType,
		const DrawFuncType& funcName, const std::string& name);

	// singleton
	static ConstBufferManager* GetInstance();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	VertexResource vertexResource_;

	std::unordered_map<std::string, std::unique_ptr<CBufferData>> cBufferData_;

	ConstBufferManager() = default;
	~ConstBufferManager() = default;
	// コピー禁止
	ConstBufferManager(const ConstBufferManager&) = delete;
	ConstBufferManager& operator=(const ConstBufferManager&) = delete;
};