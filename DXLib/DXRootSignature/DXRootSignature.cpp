#include "DXRootSignature.h"

#include "DXCommon.h"



// rootSignature_ getter
ID3D12RootSignature* DXRootSignature::GetRootSignature(PipelineType pipelineType) const {

	return rootSignature_[pipelineType].Get();
}



/*////////////////////////////////////////////////////////////////////////////////

*							DXRootSignatureの生成

////////////////////////////////////////////////////////////////////////////////*/
void DXRootSignature::Create(DXCommon* dxCommon, PipelineType pipelineType) {

	HRESULT hr;

	if (pipelineType == Primitive) {

		D3D12_ROOT_SIGNATURE_DESC primitiveDescriptionRootSignature{};

		primitiveDescriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// RootParameter作成
		D3D12_ROOT_PARAMETER primitiveRootParameters[2]{};

		primitiveRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		primitiveRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		primitiveRootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		primitiveRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		primitiveRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		primitiveRootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		primitiveDescriptionRootSignature.pParameters = primitiveRootParameters;
		primitiveDescriptionRootSignature.NumParameters = _countof(primitiveRootParameters);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&primitiveDescriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == Texture) {

		D3D12_ROOT_SIGNATURE_DESC textureDescriptionRootSignature{};

		textureDescriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER textureRootParameters[4]{};

		textureRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		textureRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		textureRootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		textureRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		textureRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		textureRootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		textureRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		textureRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		textureRootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		textureRootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		textureRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		textureRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		textureRootParameters[3].Descriptor.ShaderRegister = 1;                      // レジスタ番号0とバインド

		textureDescriptionRootSignature.pParameters = textureRootParameters;
		textureDescriptionRootSignature.NumParameters = _countof(textureRootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		textureDescriptionRootSignature.pStaticSamplers = staticSamplers;
		textureDescriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&textureDescriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
			assert(false);
		}

		hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	}
}