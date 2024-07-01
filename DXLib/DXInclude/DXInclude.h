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

	Primitive,       // 単色 テクスチャを使用しない
	Texture,         // テクスチャ使用 Lightingの有無あり
	pParticle,       // パーティクル用
	PhongReflection, // 鏡面反射
	GS               // ジオメトリシェーダ
};

// ブレンドモード
enum BlendMode {

	kBlendModeNone,     // ブレンド無し
	kBlendModeNormal,   // 通常αブレンド
	kBlendModeAdd,      // 加算
	kBlendModeSubtract, // 減算
	kBlendModeMultiply,  // 乗算
	kBlendModeScreen,   // スクリーン
};

// パイプラインタイプの数
static const uint32_t pipelineTypeNum = 5;

// ブレンドモードの数
static const uint32_t blendModeNum = 6;