#include "BreakEffect.h"

using namespace MAGIMath;
using namespace MAGIUtility;

BreakEffect::BreakEffect(Vector3 worldPosition) {

	//===================================
	// コアの設定
	//===================================

	corePosition_ = worldPosition;
	currentState_ = State::Electric;
	timer_ = 0.0f;

	//===================================
	// 靄のデータを初期化
	//===================================
	hazeEmitter_ = MAGISYSTEM::FindEmitter3D("Haze");
	hazeParticle_ = MAGISYSTEM::FindParticleGroup3D("Haze");
	hazeEmitter_->SetEmitPosition(corePosition_);

	hazeEmitter_->AddParticleGroup(hazeParticle_);

	hazeSetting_.emitType = EmitType::Random;
	hazeSetting_.minColor = Color::DarkOrange;
	hazeSetting_.maxColor = Color::DarkOrange;

	hazeSetting_.minScale = 3.0f;
	hazeSetting_.maxScale = 3.0f;
	hazeSetting_.minLifeTime = 1.0f;
	hazeSetting_.maxLifeTime = 2.0f;
	hazeSetting_.count = 20;
	hazeSetting_.frequency = 0.1f;
	hazeSetting_.minVelocity = { -3.5f,-2.5f,-2.5f };
	hazeSetting_.maxVelocity = { 3.5f,2.5f,2.5f };

	hazeEmitter_->GetEmitterSetting() = hazeSetting_;
	hazeParticle_->GetBlendMode() = BlendMode::Add;

	//===================================
	// フラッシュのデータを初期化
	//===================================

	flashPlaneMaterial_.blendMode = BlendMode::Add;
	flashPlaneMaterial_.textureName = "Circle2.png";
	flashPlaneMaterial_.baseColor = Color::Blue;

	flashScaleAnimation_ = std::make_unique<SimpleAnimation<float>>(1.0f, 30.0f, EasingType::EaseOutQuart);
	flashAlphaAnimation_ = std::make_unique<SimpleAnimation<float>>(1.0f, 0.0f, EasingType::EaseInQuart);

	//===================================
	// リングのデータを初期化
	//===================================
	for (size_t i = 0; i < 4; i++) {
		ringDatas_[i] = RingData3D{};
	}
	ringMaterial_.blendMode = BlendMode::Normal;
	ringMaterial_.textureName = "gradation.png";
	ringMaterial_.baseColor = { 1.0f,0.5f,0.0f,1.0f };

	ringRotates_[0] = { 0.7f,1.2f,-0.43f };
	ringRotates_[1] = { 1.11f,0.0f,0.0f };
	ringRotates_[2] = { 0.0f,1.4f,-0.9f };
	ringRotates_[3] = { 0.0f,-1.17f,0.92f };


	ringOuterAnimation_[0] = std::make_unique<SimpleAnimation<float>>(1.0f, 3.0f, EasingType::EaseOutQuart);
	ringInnerAnimation_[0] = std::make_unique<SimpleAnimation<float>>(0.8f, 2.0f, EasingType::EaseOutQuart);

	ringOuterAnimation_[1] = std::make_unique<SimpleAnimation<float>>(1.0f, 5.0f, EasingType::EaseOutQuart);
	ringInnerAnimation_[1] = std::make_unique<SimpleAnimation<float>>(0.8f, 4.0f, EasingType::EaseOutQuart);

	//===================================
	// 粒子のデータを初期化
	//===================================
	starEmitter_ = MAGISYSTEM::FindEmitter3D("Star");
	starParticle_ = MAGISYSTEM::FindParticleGroup3D("Star");
	starEmitter_->SetEmitPosition(corePosition_);

	starEmitter_->AddParticleGroup(starParticle_);

	starSetting_.emitType = EmitType::Random;
	starSetting_.minColor = Color::SkyBlue;
	starSetting_.maxColor = Color::SkyBlue;

	starSetting_.minLifeTime = 2.5f;
	starSetting_.maxLifeTime = 3.0f;
	starSetting_.count = 1000;

	starSetting_.minTranslate = { -2.0f,-2.0f,-2.0f };
	starSetting_.maxTranslate = { 2.0f,2.0f,2.0f };

	starSetting_.minVelocity = { -1.0f,-1.0f,-1.0f };
	starSetting_.maxVelocity = { 1.0f,1.0f,1.0f };

	starEmitter_->GetEmitterSetting() = starSetting_;
	starParticle_->GetBlendMode() = BlendMode::Normal;

	//===================================
	// ブラー用のデータを初期化
	//===================================
	radialBlurScaleAni_ = std::make_unique<SimpleAnimation<float>>(0.0f, 0.01f, EasingType::EaseInOutSine, true, LoopType::PingPong);
}

BreakEffect::~BreakEffect() {

}

void BreakEffect::SetRingRotates(std::array<Vector3, 4> rotates) {
	ringRotates_ = rotates;
}

void BreakEffect::SetRingColor(Vector4 color) {
	ringMaterial_.baseColor = color;
}

void BreakEffect::Update() {
	switch (currentState_) {
		case State::Electric:
			UpdateElectric();
			break;
		case State::Haze:
			UpdateHaze();
			break;
		case State::Explosion:
			UpdateExplosion();
			break;
		case State::Finish:
			UpdateFinish();
			break;
	}
}

void BreakEffect::Draw() {
	switch (currentState_) {
		case State::Electric:
			DrawElectric();
			break;
		case State::Haze:
			DrawHaze();
			break;
		case State::Explosion:
			DrawExplosion();
			break;
		case State::Finish:
			DrawFinish();
			break;
	}
}

bool BreakEffect::IsFinished() {
	return isFinished_;
}

void BreakEffect::UpdateElectric() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	if (timer_ >= electricTime_) {
		timer_ = 0.0f;
		currentState_ = State::Haze;
		hazeEmitter_->GetEmitterSetting().isRepeat = true;
	}
}

void BreakEffect::UpdateHaze() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	if (timer_ >= electricTime_) {
		timer_ = 0.0f;
		currentState_ = State::Explosion;
		starEmitter_->EmitAll();
		hazeEmitter_->GetEmitterSetting().isRepeat = false;
		MAGISYSTEM::ShakeCurrentCamera3D(0.5f, 0.8f);
	}
}

void BreakEffect::UpdateExplosion() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	float t = timer_ / explosionTime_;

	radialBlurScale_ = radialBlurScaleAni_->GetValue(t * 2.0f);

	MAGISYSTEM::ApplyPostEffectRadialBlur(Vector2(0.5f, 0.5f), radialBlurScale_);

	// リングの処理
	for (uint32_t i = 0; i < 2; i++) {
		ringDatas_[i].outerRadius = ringOuterAnimation_[0]->GetValue(t);
		ringDatas_[i].innerRadius = ringInnerAnimation_[0]->GetValue(t);
	}
	for (uint32_t i = 2; i < 4; i++) {
		ringDatas_[i].outerRadius = ringOuterAnimation_[1]->GetValue(t);
		ringDatas_[i].innerRadius = ringInnerAnimation_[1]->GetValue(t);
	}

	flashScale_ = flashScaleAnimation_->GetValue(t);

	// フラッシュの処理
	flashPlaneMaterial_.baseColor.w = flashAlphaAnimation_->GetValue(t);

	ImGui::DragFloat("t", &t);
	ImGui::DragFloat("Alpha", &flashPlaneMaterial_.baseColor.w);

	if (timer_ >= explosionTime_) {
		timer_ = 0.0f;
		currentState_ = State::Finish;
	}
}

void BreakEffect::UpdateFinish() {
	timer_ += MAGISYSTEM::GetDeltaTime();

	ringMaterial_.baseColor.w = 1.0f - timer_ / finishTime_;

	if (timer_ >= finishTime_) {
		timer_ = 0.0f;
		isFinished_ = true;
	}

}

void BreakEffect::DrawElectric() {

}

void BreakEffect::DrawHaze() {

}

void BreakEffect::DrawExplosion() {

	MAGISYSTEM::DrawPlane3D(MakeAffineMatrix(Vector3(flashScale_, flashScale_, 1.0f), Vector3(0.0f, 0.0f, 0.0f), corePosition_), flashPlaneData_, flashPlaneMaterial_);

	for (uint32_t i = 0; i < 4; i++) {
		Matrix4x4 wMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, ringRotates_[i], corePosition_);
		MAGISYSTEM::DrawRing3D(wMat, ringDatas_[i], ringMaterial_);
	}
}

void BreakEffect::DrawFinish() {
	for (uint32_t i = 0; i < 4; i++) {
		Matrix4x4 wMat = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, ringRotates_[i], corePosition_);
		MAGISYSTEM::DrawRing3D(wMat, ringDatas_[i], ringMaterial_);
	}
}
