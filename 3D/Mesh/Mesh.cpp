#include "Mesh.h"

#include "DXCommon.h"



// triangle_ getter
Mesh::TriangleMeshData* Mesh::GetTriangle() const {

	return triangle_.get();
}

// triangle_ getter
Mesh::SphereMeshData* Mesh::GetSphere() const {

	return sphere_.get();
}


// メッシュカウントのリセット
void Mesh::ResetMeshCount() {

	indexTriangle_ = 0;
	indexSphere_ = 0;
}



/*////////////////////////////////////////////////////////////////////////////////

*								各メッシュの生成

////////////////////////////////////////////////////////////////////////////////*/
void Mesh::CreateMeshes() {

	UINT triangleVertexCount = kMaxTriangleNum_ * kTriangleVertexNum_;

	// 三角形
	triangle_ = CreateTriangleMesh(triangleVertexCount);

	UINT sphereVertexCount = kMaxSphereNum_ * kSphereIndexNum_;
	UINT sphereIndexCount = kMaxSphereNum_ * kSphereVertexNum_;

	// 球
	sphere_ = CreateSphereMesh(sphereVertexCount, sphereIndexCount);

}



/*////////////////////////////////////////////////////////////////////////////////

*								三角メッシュの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<Mesh::TriangleMeshData> Mesh::CreateTriangleMesh(UINT vertexCount) {

	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<TriangleMeshData> mesh = std::make_unique<TriangleMeshData>();

	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertexCount);

		// 頂点バッファの生成
		mesh->vertexResource = vertexResource_.CreateBufferResource(dxCommon->GetDevice(), sizeVB);

		// 頂点バッファビューの作成
		mesh->vertexBufferView.BufferLocation = mesh->vertexResource->GetGPUVirtualAddress();
		mesh->vertexBufferView.SizeInBytes = sizeVB;
		mesh->vertexBufferView.StrideInBytes = sizeof(VertexData);

		// 頂点データのマッピング
		hr = mesh->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&mesh->data));
		assert(SUCCEEDED(hr));
	}

	return mesh;
}



/*////////////////////////////////////////////////////////////////////////////////

*								 球メッシュの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<Mesh::SphereMeshData> Mesh::CreateSphereMesh(UINT vertexCount, UINT indexCount) {

	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<SphereMeshData> mesh = std::make_unique<SphereMeshData>();


	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertexCount);

		// 頂点バッファの生成
		mesh->vertexResource = vertexResource_.CreateBufferResource(dxCommon->GetDevice(), sizeVB);

		// 頂点バッファビューの作成
		mesh->vertexBufferView.BufferLocation = mesh->vertexResource->GetGPUVirtualAddress();
		mesh->vertexBufferView.SizeInBytes = sizeVB;
		mesh->vertexBufferView.StrideInBytes = sizeof(VertexData);

		// 頂点データのマッピング
		hr = mesh->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&mesh->data));
		assert(SUCCEEDED(hr));
	}

	if (indexCount > 0) {

		// インデックスデータのサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * indexCount);
		// インデックスバッファの生成
		mesh->indexResource = vertexResource_.CreateBufferResource(dxCommon->GetDevice(), sizeIB);

		// インデックスバッファビューの作成
		mesh->indexBufferView.BufferLocation = mesh->indexResource->GetGPUVirtualAddress();
		mesh->indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		mesh->indexBufferView.SizeInBytes = sizeIB;

		// インデックスバッファのマッピング
		hr = mesh->indexResource->Map(0, nullptr, reinterpret_cast<void**>(&mesh->index));
		assert(SUCCEEDED(hr));
	}

	return mesh;
}



/*////////////////////////////////////////////////////////////////////////////////

*								頂点バッファへデータ転送

////////////////////////////////////////////////////////////////////////////////*/
void Mesh::VertexBufferMemcpy() {

	// 法線の計算
	Vector3 normal =
		Vector3::CalculateTriangleNormal({ -1.0f,-1.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f,1.0f }, { 1.0f,-1.0f,0.0f,1.0f });

	// 頂点データ
	std::array vertices = {

		VertexData{.pos{-1.0f,-1.0f,0.0f,1.0f},.texcoord{0.0f,1.0f},.normal = normal},
		VertexData{.pos{0.0f,1.0f,0.0f,1.0f},.texcoord{0.5f,0.0f},.normal = normal},
		VertexData{.pos{1.0f,-1.0f,0.0f,1.0f},.texcoord{1.0f,1.0f},.normal = normal}
	};

	// 三角形使用数に応じた頂点数の計算
	indexTriangleVertex_ = indexTriangle_ * kTriangleVertexNum_;

	assert(vertices.size() <= kTriangleVertexNum_);

	// 頂点バッファへデータ転送
	std::memcpy(&triangle_->data[indexTriangleVertex_], vertices.data(), sizeof(vertices[0]) * vertices.size());
}



/*////////////////////////////////////////////////////////////////////////////////

*								球頂点データの作成

////////////////////////////////////////////////////////////////////////////////*/
void Mesh::SetSphereData(VertexData* vertexData, uint32_t* indexData) {

#pragma region // VertexData //
	// 経度分割1つ分の角度
	const float kLonEvery = std::numbers::pi_v<float> *2.0f / float(kSphereSubdivision);
	// 緯度分割1つ分の角度
	const float kLatEvery = std::numbers::pi_v<float> / float(kSphereSubdivision);

	// 緯度方向に分割
	for (uint32_t latIndex = 0; latIndex < kSphereSubdivision; latIndex++) {

		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;

		// 経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSphereSubdivision; lonIndex++) {

			uint32_t start = (latIndex * kSphereSubdivision + lonIndex) * 4;
			float lon = lonIndex * kLonEvery;

			float u0 = static_cast<float>(lonIndex) / kSphereSubdivision;
			float u1 = static_cast<float>(lonIndex + 1) / kSphereSubdivision;
			float v0 = 1.0f - static_cast<float>(latIndex) / kSphereSubdivision;
			float v1 = 1.0f - static_cast<float>(latIndex + 1) / kSphereSubdivision;

			/*=========================================================================================*/
			// 各頂点の値を書き込む
			// 基準点 a
			vertexData[start].pos.x =
				std::cos(lat) * std::cos(lon);

			vertexData[start].pos.y =
				std::sin(lat);

			vertexData[start].pos.z =
				std::cos(lat) * std::sin(lon);

			vertexData[start].pos.w = 1.0f;

			vertexData[start].texcoord = { u0, v0 };

			vertexData[start].normal.x = vertexData[start].pos.x;
			vertexData[start].normal.y = vertexData[start].pos.y;
			vertexData[start].normal.z = vertexData[start].pos.z;

			/*=========================================================================================*/
			// 基準点 b
			vertexData[start + 1].pos.x =
				std::cos(lat + kLatEvery) * std::cos(lon);

			vertexData[start + 1].pos.y =
				std::sin(lat + kLatEvery);

			vertexData[start + 1].pos.z =
				std::cos(lat + kLatEvery) * std::sin(lon);

			vertexData[start + 1].pos.w = 1.0f;

			vertexData[start + 1].texcoord = { u0, v1 };

			vertexData[start + 1].normal.x = vertexData[start + 1].pos.x;
			vertexData[start + 1].normal.y = vertexData[start + 1].pos.y;
			vertexData[start + 1].normal.z = vertexData[start + 1].pos.z;

			/*=========================================================================================*/
			// 基準点 c
			vertexData[start + 2].pos.x =
				std::cos(lat) * std::cos(lon + kLonEvery);

			vertexData[start + 2].pos.y =
				std::sin(lat);

			vertexData[start + 2].pos.z =
				std::cos(lat) * std::sin(lon + kLonEvery);

			vertexData[start + 2].pos.w = 1.0f;

			vertexData[start + 2].texcoord = { u1, v0 };

			vertexData[start + 2].normal.x = vertexData[start + 2].pos.x;
			vertexData[start + 2].normal.y = vertexData[start + 2].pos.y;
			vertexData[start + 2].normal.z = vertexData[start + 2].pos.z;

			/*=========================================================================================*/
			// 基準点 d
			vertexData[start + 3].pos.x =
				std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);

			vertexData[start + 3].pos.y =
				std::sin(lat + kLatEvery);

			vertexData[start + 3].pos.z =
				std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);

			vertexData[start + 3].pos.w = 1.0f;

			vertexData[start + 3].texcoord = { u1, v1 };

			vertexData[start + 3].normal.x = vertexData[start + 3].pos.x;
			vertexData[start + 3].normal.y = vertexData[start + 3].pos.y;
			vertexData[start + 3].normal.z = vertexData[start + 3].pos.z;
		}
	}
#pragma endregion

#pragma region // indexData //
	for (uint32_t latIndex = 0; latIndex <= kSphereSubdivision; latIndex++) {
		for (uint32_t lonIndex = 0; lonIndex <= kSphereSubdivision; lonIndex++) {
			uint32_t baseIndex = (latIndex * (kSphereSubdivision)+lonIndex) * 4;

			indexData[6 * (latIndex * kSphereSubdivision + lonIndex)] = baseIndex;         // 0
			indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 1] = baseIndex + 1; // 1
			indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 2] = baseIndex + 2; // 2
			indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 3] = baseIndex + 2; // 3
			indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 4] = baseIndex + 1; // 4
			indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 5] = baseIndex + 3; // 5
		}
	}
#pragma endregion

}



/*////////////////////////////////////////////////////////////////////////////////

*									三角形描画

////////////////////////////////////////////////////////////////////////////////*/
void Mesh::TriangleDrawCall(ID3D12GraphicsCommandList* commandList) {

	commandList->DrawInstanced(kTriangleVertexNum_, 1, static_cast<INT>(indexTriangleVertex_), 0);

	// 使用カウント上昇
	indexTriangle_++;
}



/*////////////////////////////////////////////////////////////////////////////////

*									  球描画

////////////////////////////////////////////////////////////////////////////////*/
void Mesh::SphereDrawCall(ID3D12GraphicsCommandList* commandList) {

	commandList->DrawIndexedInstanced(kSphereVertexNum_, 1, static_cast<INT>(indexSphere_), 0, 0);

	// 使用カウント上昇
	indexSphere_++;
}