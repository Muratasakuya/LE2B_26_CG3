#include "VertexResource.h"

#include "DXCommon.h"



/*////////////////////////////////////////////////////////////////////////////////

*								BufferResourceの生成

////////////////////////////////////////////////////////////////////////////////*/
ComPtr<ID3D12Resource> VertexResource::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {

	HRESULT hr;

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	// リソースのサイズ、今回はVector4を3頂点分
	vertexResourceDesc.Width = sizeInBytes;
	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	ComPtr<ID3D12Resource> bufferResource = nullptr;
	hr = device->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&bufferResource));
	assert(SUCCEEDED(hr));

	return bufferResource;
}



/*////////////////////////////////////////////////////////////////////////////////

*								マテリアルデータの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<CBMaterialData> VertexResource::CreateMaterial() {

	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<CBMaterialData> material = std::make_unique<CBMaterialData>();

	// 頂点マテリアルの生成
	material->resource = CreateBufferResource(dxCommon->GetDevice(), sizeof(Material));

	// マテリアルデータのマッピング
	hr = material->resource->Map(0, nullptr, reinterpret_cast<void**>(&material->data));

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return material;
}



/*////////////////////////////////////////////////////////////////////////////////

*						PhongReflectionマテリアルデータの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<CBPhongRefMaterialData> VertexResource::CreatePhongRefMaterial() {


	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<CBPhongRefMaterialData> material = std::make_unique<CBPhongRefMaterialData>();

	// 頂点マテリアルの生成
	material->resource = CreateBufferResource(dxCommon->GetDevice(), sizeof(PhongRefMaterial));

	// マテリアルデータのマッピング
	hr = material->resource->Map(0, nullptr, reinterpret_cast<void**>(&material->data));

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return material;
}



/*////////////////////////////////////////////////////////////////////////////////

*								 WVPデータの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<CBTransformData> VertexResource::CreateWVP() {

	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<CBTransformData> matrix = std::make_unique<CBTransformData>();

	// WVPの生成
	matrix->resource = CreateBufferResource(dxCommon->GetDevice(), sizeof(TransformationMatrix));

	// WVPデータのマッピング
	hr = matrix->resource->Map(0, nullptr, reinterpret_cast<void**>(&matrix->data));

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return matrix;
}



/*////////////////////////////////////////////////////////////////////////////////

*							 パーティクル用WVPの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<CBParticleTransformData> VertexResource::CreateParticleWVP(const uint32_t instanceNum) {

	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<CBParticleTransformData> matrix = std::make_unique<CBParticleTransformData>();

	// WVPの生成
	matrix->resource = CreateBufferResource(dxCommon->GetDevice(), sizeof(ParticleForGPU) * instanceNum);

	// WVPデータのマッピング
	hr = matrix->resource->Map(0, nullptr, reinterpret_cast<void**>(&matrix->particleData));

	// 単位行列を書き込んでおく
	for (size_t index = 0; index < instanceNum; index++) {

		matrix->particleData[index].World = Matrix4x4::MakeIdentity4x4();
		matrix->particleData[index].WVP = Matrix4x4::MakeIdentity4x4();
		matrix->particleData[index].color = { 1.0f,1.0f,1.0f,1.0f };
	}

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return matrix;
}



/*////////////////////////////////////////////////////////////////////////////////

*								Lightデータの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<CBLightData> VertexResource::CreateLight() {


	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<CBLightData> light = std::make_unique<CBLightData>();

	// Lightの生成
	light->resource = CreateBufferResource(dxCommon->GetDevice(), sizeof(DirectionalLight));

	// Lightデータのマッピング
	hr = light->resource->Map(0, nullptr, reinterpret_cast<void**>(&light->data));

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return light;
}



/*////////////////////////////////////////////////////////////////////////////////

*								PointLightデータの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<CBPointLightData> VertexResource::CreatePointLight() {


	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<CBPointLightData> pointLight = std::make_unique<CBPointLightData>();

	// PointLightの生成
	pointLight->resource = CreateBufferResource(dxCommon->GetDevice(), sizeof(PointLight));

	// PointLightデータのマッピング
	hr = pointLight->resource->Map(0, nullptr, reinterpret_cast<void**>(&pointLight->data));

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return pointLight;
}



/*////////////////////////////////////////////////////////////////////////////////

*								SpotLightデータの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<CBSpotLightData> VertexResource::CreateSpotLight() {


	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<CBSpotLightData> spotLight = std::make_unique<CBSpotLightData>();

	// SpotLightの生成
	spotLight->resource = CreateBufferResource(dxCommon->GetDevice(), sizeof(SpotLight));

	// SpotLightデータのマッピング
	hr = spotLight->resource->Map(0, nullptr, reinterpret_cast<void**>(&spotLight->data));

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return spotLight;
}




/*////////////////////////////////////////////////////////////////////////////////

*								 Cameraデータの生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<CBCameraData> VertexResource::CreateCamera() {

	DXCommon* dxCommon = DXCommon::GetInstance();

	HRESULT hr;
	std::unique_ptr<CBCameraData> camera = std::make_unique<CBCameraData>();

	// Cameraの生成
	camera->resource = CreateBufferResource(dxCommon->GetDevice(), sizeof(CameraForGPU));

	// Cameraデータのマッピング
	hr = camera->resource->Map(0, nullptr, reinterpret_cast<void**>(&camera->data));

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return camera;
}