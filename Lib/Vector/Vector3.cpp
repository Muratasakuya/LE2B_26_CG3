#include "Vector3.h"

/*-------------------------------------------------------------*/
/// 算術演算子

// +
Vector3 Vector3::operator+(const Vector3& other) const {
	return { x + other.x, y + other.y, z + other.z };
}
// -
Vector3 Vector3::operator-(const Vector3& other) const {
	return { x - other.x, y - other.y, z - other.z };
}
// *
Vector3 Vector3::operator*(const Vector3& other) const {
	return { x * other.x, y * other.y, z * other.z };
}
// /
Vector3 Vector3::operator/(const Vector3& other) const {
	return { x / other.x, y / other.y, z / other.z };
}

// +=
Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

// -=
Vector3& Vector3::operator-=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

/*-------------------------------------------------------------*/
/// 関数

// 三角形の頂点から法線の取得
Vector3 Vector3::CalculateTriangleNormal(const Vector4& v0, const Vector4& v1, const Vector4& v2) {

	// 頂点間のベクトルを計算
	Vector3 vec1 = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
	Vector3 vec2 = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

	// 外積を計算して法線ベクトルを得る
	Vector3 normal;
	normal.x = vec1.y * vec2.z - vec1.z * vec2.y;
	normal.y = vec1.z * vec2.x - vec1.x * vec2.z;
	normal.z = vec1.x * vec2.y - vec1.y * vec2.x;

	// 法線ベクトルを正規化する
	float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	normal.x /= length;
	normal.y /= length;
	normal.z /= length;

	return normal;
}