#pragma once

// DirectX
#include "DXInclude.h"
#include "CBufferStructure.h"
#include "VertexResource.h"

// c++
#include <memory>
#include <string>
#include <cassert>
#include <unordered_map>

/*////////////////////////////////////////////////////////////////////////////////
*
*									Model Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Model {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// モデルデータ
	struct ModelMeshData {

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexResource;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// 頂点バッファデータ
		VertexData* data = nullptr;
	};

	void CreateModelMesh(const std::string modelName, UINT vertexCount);

	// getter

	ModelMeshData* GetModel(const std::string& modelName) const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// CreateBuffer
	VertexResource vertexResource_;

	/*--------------------------------------------------------------------*/
	/// モデル

	// モデルメッシュデータ
	std::unordered_map<std::string, std::unique_ptr<ModelMeshData>> models_;

	/*--------------------------------------------------------------------*/

};