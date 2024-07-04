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
	Normal,          // テクスチャ使用 Lightingの有無あり
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

// Engine描画関数
enum DrawFuncType {

	DTriangle,
	DSprite,
	DSphere,
	DModel,
	DParticle,
	DGSModel
};

// パイプラインタイプの数
static const uint32_t pipelineTypeNum = 5;

// ブレンドモードの数
static const uint32_t blendModeNum = 6;