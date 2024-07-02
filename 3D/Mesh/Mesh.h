#pragma once

// DirectX
#include "DXInclude.h"
#include "CBufferStructure.h"
#include "VertexResource.h"

// c++
#include <memory>
#include <array>
#include <cassert>
#include <numbers>

/*////////////////////////////////////////////////////////////////////////////////
*
*									Mesh Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Mesh {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// GS
	struct GSPointData {

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexResource;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// 頂点バッファデータ
		VertexPos* data = nullptr;
	};

	// 三角形メッシュデータ
	struct TriangleMeshData {

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexResource;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// 頂点バッファデータ
		VertexData* data = nullptr;
	};

	// 球メッシュデータ
	struct SphereMeshData {

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

	void CreateMeshes();

	void ResetMeshCount();

	void VertexBufferMemcpy();
	void SetSphereData(VertexData* vertexData, uint32_t* indexData);

	void TriangleDrawCall(ID3D12GraphicsCommandList* commandList);
	void SphereDrawCall(ID3D12GraphicsCommandList* commandList);

	// getter

	TriangleMeshData* GetTriangle() const;
	SphereMeshData* GetSphere() const;
	GSPointData* GetGSPoint() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// CreateBuffer
	VertexResource vertexResource_;

	/*--------------------------------------------------------------------*/
	/// 三角形

	// 三角形の最大数
	static const uint32_t kMaxTriangleNum_ = 3;
	// 三角形の頂点数
	static const UINT kTriangleVertexNum_ = 3;
	// 三角形使用数
	uint32_t indexTriangle_ = 0;
	size_t indexTriangleVertex_ = 0;

	// 三角形メッシュデータ
	std::unique_ptr<TriangleMeshData> triangle_;
	// メッシュ生成
	std::unique_ptr<TriangleMeshData> CreateTriangleMesh(UINT vertexCount);

	/*--------------------------------------------------------------------*/
	/// 球

	// 球の分割数
	static const uint32_t kSphereSubdivision = 16;

	// 球の最大数
	static const uint32_t kMaxSphereNum_ = 3;
	// 球の頂点数 Vertex1536
	static const UINT kSphereVertexNum_ = kSphereSubdivision * kSphereSubdivision * 6;
	// 球の頂点数 Index1024
	static const UINT kSphereIndexNum_ = kSphereSubdivision * kSphereSubdivision * 4;
	// 球使用数
	uint32_t indexSphere_ = 0;

	// 球メッシュデータ
	std::unique_ptr<SphereMeshData> sphere_;
	// 球メッシュ生成
	std::unique_ptr<SphereMeshData> CreateSphereMesh(UINT vertexCount, UINT indexCount);

	/*--------------------------------------------------------------------*/
	/// GS

	// 頂点座標
	Vector3 gsVertexPos{};
	// GS頂点データ
	std::unique_ptr<GSPointData> gsPoint_;
	// GS頂点生成
	std::unique_ptr<GSPointData> CreateGSPoint();

};