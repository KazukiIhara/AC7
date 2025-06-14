#pragma once

// C++
#include <string>
#include <unordered_map>
#include <memory>

#include "Math/Types/Vector3.h"
#include "Math/Types/Quaternion.h"

// 前方宣言
class Transform3D;
class ModelRenderer;

/// <summary>
/// シーン上のオブジェクト
/// </summary>
class GameObject3D {
public:
	GameObject3D(const std::string& name, const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	GameObject3D(const std::string& name, const Vector3& scale, const Quaternion& rotate, const Vector3& translate);
	GameObject3D(const std::string& name, const Vector3& translate = Vector3(0.0f, 0.0f, 0.0f));

	~GameObject3D() = default;

	void Finalize();

	void AddModelRenderer(std::shared_ptr<ModelRenderer> modelRenderer);

	void SetIsAlive(bool isAlive);
	void SetIsActive(bool isActive);

	[[nodiscard]] const std::string& GetName()const;

	[[nodiscard]] bool GetIsAlive()const;
	[[nodiscard]] bool GetIsActive()const;

	[[nodiscard]] Transform3D* GetTransform();
	[[nodiscard]] std::weak_ptr<ModelRenderer> GetModelRenderer(const std::string& rendererName);
private:
	// 名前
	std::string name_ = "";
	// 生存フラグ
	bool isAlive_ = true;
	// 有効フラグ
	bool isActive_ = true;

	//=======================
	// コンポーネント
	//=======================

	// トランスフォーム
	Transform3D* transformComponent_;
	// モデル描画コンポーネント
	std::unordered_map<std::string, std::weak_ptr<ModelRenderer>> modelRendererComponents_;

};