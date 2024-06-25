#pragma once

// DirectX
#include <dxgidebug.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

// Lib
#include "ComPtr.h"

// DXObject
#include "DXDevice.h"
#include "DXSwapChain.h"
#include "DXDescriptor.h"

// パイプラインの種類
enum PipelineType {

	Primitive,     // 単色 テクスチャを使用しない
	Texture,       // テクスチャ使用 Lightingの有無あり
};

// パイプラインタイプの数
static const uint32_t pipelineTypeNum = 2;