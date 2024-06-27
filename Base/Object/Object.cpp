#include "Object.h"


// enableLighting_ = enableLighting setter
void Object::SetEnableLighting(bool enableLighting) {

	enableLighting_ = enableLighting;
}

// enableHalfLambert_ = enableHalfLambert setter
void Object::SetEnableHalfLambert(bool enableHalfLambert) {

	enableHalfLambert_ = enableHalfLambert;
}

// enablePhongReflection_ = enablePhongReflection setter
void Object::SetEnablePhongReflection(bool enablePhongReflection) {

	enablePhongReflection_ = enablePhongReflection;
}

// enableBlinnPhongReflection_ = enableBlinnPhongReflection setter
void Object::SetEnableBlinnPhongReflection(bool enableBlinnPhongReflection) {

	enableBlinnPhongReflection_ = enableBlinnPhongReflection;
}

// pointLightPos_ = pointLightPos setter
void Object::SetPointLightPos(Vector3 pointLightPos) {

	pointLightPos_ = pointLightPos;
}

// pointLightRadius_ = pointLightRadius setter
void Object::SetPointLightRadius(float pointLightRadius) {

	pointLightRadius_ = pointLightRadius;
}

// pointLightDecay_ = pointLightDeacy setter
void Object::SetPointLightDecay(float pointLightDeacy) {

	pointLightDecay_ = pointLightDeacy;
}

// transform_.scale = scale setter
void Object::SetScale(Vector3 scale) {

	transform_.scale = scale;
}

// transform_.rotate = rotate setter
void Object::SetRotate(Vector3 rotate) {

	transform_.rotate = rotate;
}

// transform_.translate = translate setter
void Object::SetTranslate(Vector3 translate) {

	transform_.translate = translate;
}

// transform_ = transform setter   {scale,rotate,translate}
void Object::SetTransform(Transform transform) {

	transform_ = transform;
}