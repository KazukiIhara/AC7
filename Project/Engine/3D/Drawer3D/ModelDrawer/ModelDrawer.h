#pragma once

// C++
#include <memory>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "3D/Drawer3D/MeshDrawer/MeshDrawer.h"

/// <summary>
/// モデル描画用クラス
/// </summary>
class ModelDrawer {
public:
	ModelDrawer(const ModelData& modelData);
	~ModelDrawer();

	void AddDrawCommand(const Matrix4x4& worldMatrix, const ModelMaterial& material);
	void Update();
	void Draw(BlendMode mode);
	void DrawShadow(BlendMode mode);

private:
	// インスタンスの最大数
	static const uint32_t kNumMaxInstance = 65536;

	// メッシュ
	std::vector<std::unique_ptr<MeshDrawer>> meshes_;

	// instancing描画用のリソース
	ComPtr<ID3D12Resource> instancingResource_[static_cast<uint32_t>(BlendMode::Num)];
	// instancing描画用のデータ
	ModelDataForGPU* instancingData_[static_cast<uint32_t>(BlendMode::Num)];
	// instancingSrvIndex
	uint32_t instancingSrvIndex_[static_cast<uint32_t>(BlendMode::Num)];


	// instance描画の際に使う変数
	uint32_t instanceCount_[static_cast<uint32_t>(BlendMode::Num)];
	// 現在のインデックス
	uint32_t currentIndex_[static_cast<uint32_t>(BlendMode::Num)];
};