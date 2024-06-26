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