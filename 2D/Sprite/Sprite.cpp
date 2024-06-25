#include "Sprite.h"

#include "DXCommon.h"



// sprite_ getter
Sprite::SpriteMeshData* Sprite::GetSprite() const {

	return sprite_.get();
}



// メッシュカウントのリセット
void Sprite::ResetMeshCount() {

	indexSprite_ = 0;
}



/*////////////////////////////////////////////////////////////////////////////////

*							スプライトメッシュの生成

////////////////////////////////////////////////////////////////////////////////*/
void Sprite::CreateMesh() {

	UINT spriteVertexCount = kMaxSpriteNum_ * kSpriteIndexNum_;
	UINT spriteIndexCount = kMaxSpriteNum_ * kSpriteVertexNum_;

	// スプライト
	sprite_ = CreateSpriteMesh(spriteVertexCount, spriteIndexCount);
}

/*////////////////////////////////////////////////////////////////////////////////

*							スプライトメッシュの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<Sprite::SpriteMeshData> Sprite::CreateSpriteMesh(UINT vertexCount, UINT indexCount) {

	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<SpriteMeshData> mesh = std::make_unique<SpriteMeshData>();

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

*							スプライト頂点データの作成

////////////////////////////////////////////////////////////////////////////////*/
void Sprite::SetSpriteData(VertexData* vertexData, uint32_t* indexData) {

#pragma region // VertexData //
	// 1=3、2=5、頂点4つで描画
	// 左下
	vertexData[0].pos = { 0.0f,360.0f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };
	// 左上
	vertexData[1].pos = { 0.0f,0.0f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };
	// 右下
	vertexData[2].pos = { 640.0f,360.0f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
	// 右上
	vertexData[3].pos = { 640.0f,0.0f,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };
#pragma endregion

#pragma region // IndexData //
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;
#pragma endregion
}



/*////////////////////////////////////////////////////////////////////////////////

*								  スプライト描画

////////////////////////////////////////////////////////////////////////////////*/
void Sprite::SpriteDrawCall(ID3D12GraphicsCommandList* commandList) {

	commandList->DrawIndexedInstanced(kSpriteVertexNum_, 1, static_cast<INT>(indexSprite_), 0, 0);

	// 使用カウント上昇
	indexSprite_++;
}