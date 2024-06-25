#pragma once

// Base
#include "Object.h"

class Camera2D;

/*////////////////////////////////////////////////////////////////////////////////
*
*								Object2D Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Object2D :
	public Object {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	Object2D();
	~Object2D();

	void Initilize(Camera2D* camera);

	void Update(Camera2D* camera);

	void Draw(const std::string textureName);

};