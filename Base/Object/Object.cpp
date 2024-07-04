#include "Object.h"


// material_.enableLighting = enableLighting setter
void Object::SetEnableLighting(bool enableLighting) {

	material_.enableLighting = enableLighting;
}
// material_.enableHalfLambert = enableHalfLambert setter
void Object::SetEnableHalfLambert(bool enableHalfLambert) {

	material_.enableHalfLambert = enableHalfLambert;
}
// phongRef_.enablePhongReflection = enablePhongReflection setter
void Object::SetEnablePhongReflection(bool enablePhongReflection) {

	phongRef_.enablePhongReflection = enablePhongReflection;
}
// enableBlinnPhongReflection_ = enableBlinnPhongReflection setter
void Object::SetEnableBlinnPhongReflection(bool enableBlinnPhongReflection) {

	phongRef_.enableBlinnPhongReflection = enableBlinnPhongReflection;
}



// pointLight_ = pointLight setter
void Object::SetPointLight(PointLight pointLight) {

	pointLight_ = pointLight;
}



// spotLight_ = spotLight setter
void Object::SetSpotLight(SpotLight spotLight) {

	spotLight_ = spotLight;
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