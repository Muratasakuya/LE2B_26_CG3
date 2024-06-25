#include "Model.h"

#include "DXCommon.h"



// models_.find(modelName) it->second.get() getter
Model::ModelMeshData* Model::GetModel(const std::string& modelName) const {

	auto it = models_.find(modelName);

	if (it != models_.end()) {

		return it->second.get();
	} else {

		return nullptr;
	}
}



/*////////////////////////////////////////////////////////////////////////////////

*							  モデルメッシュの生成

////////////////////////////////////////////////////////////////////////////////*/
void Model::CreateModelMesh(const std::string modelName, UINT vertexCount) {

	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	models_[modelName] = std::make_unique<ModelMeshData>();

	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertexCount);

		// 頂点バッファの生成
		models_[modelName]->vertexResource = vertexResource_.CreateBufferResource(dxCommon->GetDevice(), sizeVB);

		// 頂点バッファビューの作成
		models_[modelName]->vertexBufferView.BufferLocation = models_[modelName]->vertexResource->GetGPUVirtualAddress();
		models_[modelName]->vertexBufferView.SizeInBytes = sizeVB;
		models_[modelName]->vertexBufferView.StrideInBytes = sizeof(VertexData);

		// 頂点データのマッピング
		hr = models_[modelName]->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&models_[modelName]->data));
		assert(SUCCEEDED(hr));
	}
}