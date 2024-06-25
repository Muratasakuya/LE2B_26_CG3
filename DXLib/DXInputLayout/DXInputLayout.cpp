#include "DXInputLayout.h"



// inputLayoutDesc_ getter
D3D12_INPUT_LAYOUT_DESC DXInputLayout::GetInputLayoutDesc(PipelineType pipelineType) {

	return inputLayoutDesc_[pipelineType];
}



/*////////////////////////////////////////////////////////////////////////////////

*								DXInputLayoutの生成

////////////////////////////////////////////////////////////////////////////////*/
void DXInputLayout::Create(PipelineType pipelineType) {

	if (pipelineType == Primitive) {

		static D3D12_INPUT_ELEMENT_DESC primitiveInputElementDescs[1]{};

		primitiveInputElementDescs[0].SemanticName = "POSITION";
		primitiveInputElementDescs[0].SemanticIndex = 0;
		primitiveInputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		primitiveInputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_[pipelineType] = {};
		inputLayoutDesc_[pipelineType].pInputElementDescs = primitiveInputElementDescs;
		inputLayoutDesc_[pipelineType].NumElements = _countof(primitiveInputElementDescs);
	} else if (pipelineType == Texture) {

		static D3D12_INPUT_ELEMENT_DESC textureInputElementDescs[3]{};

		textureInputElementDescs[0].SemanticName = "POSITION";
		textureInputElementDescs[0].SemanticIndex = 0;
		textureInputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureInputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		textureInputElementDescs[1].SemanticName = "TEXCOORD";
		textureInputElementDescs[1].SemanticIndex = 0;
		textureInputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		textureInputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		textureInputElementDescs[2].SemanticName = "NORMAL";
		textureInputElementDescs[2].SemanticIndex = 0;
		textureInputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		textureInputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_[pipelineType] = {};
		inputLayoutDesc_[pipelineType].pInputElementDescs = textureInputElementDescs;
		inputLayoutDesc_[pipelineType].NumElements = _countof(textureInputElementDescs);
	}
}