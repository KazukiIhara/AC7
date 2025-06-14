#pragma once

// C++
#include <cstdint>

/// <summary>
/// ポストエフェクトパイプラインの種類
/// </summary>
enum class PostEffectType {
	Copy, // ポストエフェクトなし
	Grayscale, // グレースケール
	Vignette, // ビネット
	GaussianX, // X軸ガウスぼかし
	GaussianY, // Y軸ガウスぼかし
	RadialBlur, // 集中ぼかし

	Num,
};

// パイプラインの種類の数 
inline constexpr uint32_t kPostEffectPipelineStateNum = static_cast<uint32_t>(PostEffectType::Num);
