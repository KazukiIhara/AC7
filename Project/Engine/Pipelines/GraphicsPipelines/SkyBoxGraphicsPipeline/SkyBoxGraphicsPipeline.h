#pragma once

#include "GraphicsPipelines/BaseGraphicsPipeline/BaseGraphicsPipeline.h"

// 前方宣言
class DXGI;
class ShaderCompiler;

/// <summary>
/// スカイボックス描画パイプライン
/// </summary>
class SkyBoxGraphicsPipeline :public BaseGraphicsPipeline {
public:
	SkyBoxGraphicsPipeline(DXGI* dxgi, ShaderCompiler* shaderCompiler);
	~SkyBoxGraphicsPipeline()override;

private:
	// ルートシグネチャを作成する
	void CreateRootSignature()override;
	// シェーダーをコンパイルする
	void CompileShaders()override;
	// グラフィックスパイプラインオブジェクトを作成する
	void CreateGraphicsPipelineObject()override;
	// BlendStateの設定を行う
	D3D12_BLEND_DESC BlendStateSetting(uint32_t blendModeNum)override;
	// DepthStencilStateの設定を行う
	D3D12_DEPTH_STENCIL_DESC DepthStecilDescSetting()override;
	// InputLayoutの設定を行う
	D3D12_INPUT_LAYOUT_DESC InputLayoutSetting()override;
	// RasterizerStateの設定を行う
	D3D12_RASTERIZER_DESC RasterizerStateSetting()override;
};