#pragma once

// DirectX
#include "DXInclude.h"
#include "CBufferStructure.h"
#include "VertexResource.h"

// 3D
#include "Model.h"

// c++
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <filesystem>
#include <cassert>

/*////////////////////////////////////////////////////////////////////////////////
*
*							ModelManager Class
*
////////////////////////////////////////////////////////////////////////////////*/
class ModelManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void Initialize();

	void LoadModel(const std::string& directoryPath, const std::string& filename);
	void MakeModel(ModelData modelData, const std::string& modelName);

	ModelMaterialData LoadMaterialTemplateFile(const std::string& directorypath, const std::string& filename);
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	void VertexBufferMemcpy(const std::string& modelName);
	void IASetVertexBuffers(ID3D12GraphicsCommandList* commandList, const std::string& modelName);
	void ModelDrawCall(ID3D12GraphicsCommandList* commandList, const std::string& modelName);

	// singleton
	static ModelManager* GetInstance();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	VertexResource vertexResource_;

	std::unordered_map<std::string, ModelData> models_;

	std::unique_ptr<Model> modelMesh_;
	
	ModelManager() = default;
	~ModelManager() = default;
	// コピー禁止
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
};