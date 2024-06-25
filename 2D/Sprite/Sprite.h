#pragma once

// DirectX
#include "DXInclude.h"
#include "CBufferStructure.h"
#include "VertexResource.h"

// c++
#include <memory>
#include <array>
#include <cassert>

/*////////////////////////////////////////////////////////////////////////////////
*
*									Sprite Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Sprite{
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// スプライトデータ
	struct SpriteMeshData {

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexResource;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// インデックス頂点バッファ
		ComPtr<ID3D12Resource> indexResource;
		// インデックス頂点バッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView{};

		// 頂点バッファデータ
		VertexData* data = nullptr;
		// インデックスバッファデータ
		uint32_t* index = nullptr;
	};

	void CreateMesh();

	void SetSpriteData(VertexData* vertexData, uint32_t* indexData);

	void ResetMeshCount();

	void SpriteDrawCall(ID3D12GraphicsCommandList* commandList);


	// getter

	SpriteMeshData* GetSprite() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// CreateBuffer
	VertexResource vertexResource_;

	/*--------------------------------------------------------------------*/
	/// スプライト

	// スプライト最大数 srvDescriptorの数と同じ
	static const uint32_t kMaxSpriteNum_ = 128;
	// スプライトの頂点数 Vertex
	static const UINT kSpriteVertexNum_ = 6;
	// スプライトの頂点数　Index
	static const UINT kSpriteIndexNum_ = 4;
	// スプライト使用数
	uint32_t indexSprite_ = 0;

	// スプライトメッシュデータ
	std::unique_ptr<SpriteMeshData> sprite_;
	// スプライトメッシュ生成
	std::unique_ptr<SpriteMeshData> CreateSpriteMesh(UINT vertexCount, UINT indexCount);

	/*--------------------------------------------------------------------*/

};