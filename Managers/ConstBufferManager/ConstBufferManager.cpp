#include "ConstBufferManager.h"

#include "ModelManager.h"



/*////////////////////////////////////////////////////////////////////////////////

*								singleton

////////////////////////////////////////////////////////////////////////////////*/
ConstBufferManager* ConstBufferManager::GetInstance() {

	static ConstBufferManager instance;

	return &instance;
}



/*////////////////////////////////////////////////////////////////////////////////

*								ConstBufferの生成

////////////////////////////////////////////////////////////////////////////////*/
void ConstBufferManager::CreateConstBufferData(const std::string& name) {

	cBufferData_[name] = std::make_unique<CBufferData>();

	// Material
	cBufferData_[name]->material = vertexResource_.CreateMaterial();
	cBufferData_[name]->phongRefMaterial = vertexResource_.CreatePhongRefMaterial();

	// Matrix
	cBufferData_[name]->matrix = vertexResource_.CreateWVP();
	cBufferData_[name]->gsMatrix_ = vertexResource_.CreateGSMatrix();

	// Light
	cBufferData_[name]->light = vertexResource_.CreateLight();
	cBufferData_[name]->pointLight = vertexResource_.CreatePointLight();
	cBufferData_[name]->spotLight = vertexResource_.CreateSpotLight();

	// Camera
	cBufferData_[name]->camera = vertexResource_.CreateCamera();
}



/*////////////////////////////////////////////////////////////////////////////////

*								ConstBufferの初期化

////////////////////////////////////////////////////////////////////////////////*/
void ConstBufferManager::InitializeConstBuffer(
	const std::string& name, const Material& material, const PhongRefMaterial& phongRef_,
	const TransformationMatrix& matrix, const GSMatrix4x4& gsMat, const PunctualLight& light,
	const Vector3& cameraWorldPos, const PipelineType& drawType, bool isUseGLTFModel) {

	/*--------------------------------------------------------------------------------------------------------*/
	// Materialの初期化
	cBufferData_[name]->material->data->color = material.color;
	cBufferData_[name]->material->data->enableLighting = material.enableLighting;
	cBufferData_[name]->material->data->enableHalfLambert = material.enableHalfLambert;
	cBufferData_[name]->material->data->uvTransform = material.uvTransform;

	/*--------------------------------------------------------------------------------------------------------*/
	// PhongReflectionMaterialの初期化
	cBufferData_[name]->phongRefMaterial->data->color = phongRef_.color;
	cBufferData_[name]->phongRefMaterial->data->enableLighting = phongRef_.enableLighting;
	cBufferData_[name]->phongRefMaterial->data->enablePhongReflection = phongRef_.enablePhongReflection;
	cBufferData_[name]->phongRefMaterial->data->enableBlinnPhongReflection = phongRef_.enableBlinnPhongReflection;
	cBufferData_[name]->phongRefMaterial->data->uvTransform = phongRef_.uvTransform;
	cBufferData_[name]->phongRefMaterial->data->specularColor = phongRef_.specularColor;
	cBufferData_[name]->phongRefMaterial->data->shininess = phongRef_.shininess;

	/*--------------------------------------------------------------------------------------------------------*/
	// DirectionalLightの初期化
	cBufferData_[name]->light->data->color = light.directional.color;
	cBufferData_[name]->light->data->direction = light.directional.direction;
	cBufferData_[name]->light->data->intensity = light.directional.intensity;

	/*--------------------------------------------------------------------------------------------------------*/
	// PointLightの初期化
	cBufferData_[name]->pointLight->data->color = light.point.color;
	cBufferData_[name]->pointLight->data->pos = light.point.pos;
	cBufferData_[name]->pointLight->data->intensity = light.point.intensity;
	cBufferData_[name]->pointLight->data->radius = light.point.radius;
	cBufferData_[name]->pointLight->data->decay = light.point.decay;

	/*--------------------------------------------------------------------------------------------------------*/
	// SpotLightの初期化
	cBufferData_[name]->spotLight->data->color = light.spot.color;
	cBufferData_[name]->spotLight->data->pos = light.spot.pos;
	cBufferData_[name]->spotLight->data->direction = light.spot.direction;
	cBufferData_[name]->spotLight->data->distance = light.spot.distance;
	cBufferData_[name]->spotLight->data->intensity = light.spot.intensity;
	cBufferData_[name]->spotLight->data->cosAngle = light.spot.cosAngle;
	cBufferData_[name]->spotLight->data->cosFalloffStart = light.spot.cosFalloffStart;
	cBufferData_[name]->spotLight->data->decay = light.spot.decay;

	/*--------------------------------------------------------------------------------------------------------*/
	// Matrixの初期化
	if (isUseGLTFModel) {

		// GLTFModelのMatrix計算
		cBufferData_[name]->matrix->data->World = Matrix4x4::Multiply
		(ModelManager::GetInstance()->GetModelData(name).rootNode.localMatrix, matrix.World);
		cBufferData_[name]->matrix->data->WVP = Matrix4x4::Multiply
		(ModelManager::GetInstance()->GetModelData(name).rootNode.localMatrix, matrix.WVP);
		cBufferData_[name]->matrix->data->WorldInverseTranspose = matrix.WorldInverseTranspose;
	} else {

		// GLTFModel以外のMatrix計算 基本こっち側
		cBufferData_[name]->matrix->data->World = matrix.World;
		cBufferData_[name]->matrix->data->WVP = matrix.WVP;
		cBufferData_[name]->matrix->data->WorldInverseTranspose = matrix.WorldInverseTranspose;

		if (drawType == GS) {

			// GSを通したときのMatrix計算
			cBufferData_[name]->gsMatrix_->data->gsMatrix_ = gsMat.gsMatrix_;
		}
	}

	/*--------------------------------------------------------------------------------------------------------*/
	// CameraWorldPosの初期化
	cBufferData_[name]->camera->data->worldPosition = cameraWorldPos;

}



/*////////////////////////////////////////////////////////////////////////////////

*								ConstBufferの更新処理

////////////////////////////////////////////////////////////////////////////////*/
void ConstBufferManager::UpdateConstBuffer(
	const std::string& name, const Material& material, const PhongRefMaterial& phongRef_,
	const TransformationMatrix& matrix, const GSMatrix4x4& gsMat, const PunctualLight& light,
	const Vector3& cameraWorldPos, const PipelineType& drawType, bool isUseGLTFModel) {

	/*--------------------------------------------------------------------------------------------------------*/
	// Materialの更新
	cBufferData_[name]->material->data->color = material.color;
	cBufferData_[name]->material->data->enableLighting = material.enableLighting;
	cBufferData_[name]->material->data->enableHalfLambert = material.enableHalfLambert;
	cBufferData_[name]->material->data->uvTransform = material.uvTransform;

	/*--------------------------------------------------------------------------------------------------------*/
	// PhongReflectionMaterialの更新
	cBufferData_[name]->phongRefMaterial->data->color = phongRef_.color;
	cBufferData_[name]->phongRefMaterial->data->enableLighting = phongRef_.enableLighting;
	cBufferData_[name]->phongRefMaterial->data->enablePhongReflection = phongRef_.enablePhongReflection;
	cBufferData_[name]->phongRefMaterial->data->enableBlinnPhongReflection = phongRef_.enableBlinnPhongReflection;
	cBufferData_[name]->phongRefMaterial->data->uvTransform = phongRef_.uvTransform;
	cBufferData_[name]->phongRefMaterial->data->specularColor = phongRef_.specularColor;
	cBufferData_[name]->phongRefMaterial->data->shininess = phongRef_.shininess;

	/*--------------------------------------------------------------------------------------------------------*/
	// DirectionalLightの更新

	if (material.enableLighting || phongRef_.enableLighting) {

		cBufferData_[name]->light->data->color = light.directional.color;
		cBufferData_[name]->light->data->direction = light.directional.direction;
		cBufferData_[name]->light->data->intensity = light.directional.intensity;
	}

	/*--------------------------------------------------------------------------------------------------------*/
	// PointLightの初期化

	if (material.enableLighting || phongRef_.enableLighting) {

		cBufferData_[name]->pointLight->data->color = light.point.color;
		cBufferData_[name]->pointLight->data->pos = light.point.pos;
		cBufferData_[name]->pointLight->data->intensity = light.point.intensity;
		cBufferData_[name]->pointLight->data->radius = light.point.radius;
		cBufferData_[name]->pointLight->data->decay = light.point.decay;
	}

	/*--------------------------------------------------------------------------------------------------------*/
	// SpotLightの初期化

	if (material.enableLighting || phongRef_.enableLighting) {

		cBufferData_[name]->spotLight->data->color = light.spot.color;
		cBufferData_[name]->spotLight->data->pos = light.spot.pos;
		cBufferData_[name]->spotLight->data->direction = light.spot.direction;
		cBufferData_[name]->spotLight->data->distance = light.spot.distance;
		cBufferData_[name]->spotLight->data->intensity = light.spot.intensity;
		cBufferData_[name]->spotLight->data->cosAngle = light.spot.cosAngle;
		cBufferData_[name]->spotLight->data->cosFalloffStart = light.spot.cosFalloffStart;
		cBufferData_[name]->spotLight->data->decay = light.spot.decay;
	}

	/*--------------------------------------------------------------------------------------------------------*/
	// Matrixの初期化
	if (isUseGLTFModel) {

		// GLTFModelのMatrix計算
		cBufferData_[name]->matrix->data->World = Matrix4x4::Multiply
		(ModelManager::GetInstance()->GetModelData(name).rootNode.localMatrix, matrix.World);
		cBufferData_[name]->matrix->data->WVP = Matrix4x4::Multiply
		(ModelManager::GetInstance()->GetModelData(name).rootNode.localMatrix, matrix.WVP);
		cBufferData_[name]->matrix->data->WorldInverseTranspose = matrix.WorldInverseTranspose;
	} else {

		// GLTFModel以外のMatrix計算 基本こっち側
		cBufferData_[name]->matrix->data->World = matrix.World;
		cBufferData_[name]->matrix->data->WVP = matrix.WVP;
		cBufferData_[name]->matrix->data->WorldInverseTranspose = matrix.WorldInverseTranspose;

		if (drawType == GS) {

			// GSを通したときのMatrix計算
			cBufferData_[name]->gsMatrix_->data->gsMatrix_ = gsMat.gsMatrix_;
		}
	}

	/*--------------------------------------------------------------------------------------------------------*/
	// CameraWorldPosの初期化
	cBufferData_[name]->camera->data->worldPosition = cameraWorldPos;
}



/*////////////////////////////////////////////////////////////////////////////////

*									DrawCommand

////////////////////////////////////////////////////////////////////////////////*/
void ConstBufferManager::SetConstBuffer(
	ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType,
	const DrawFuncType& funcName, const std::string& name) {

	if (funcName == DrawFuncType::DTriangle || funcName == DrawFuncType::DSphere || funcName == DrawFuncType::DModel) {

		// wvp
		commandList->SetGraphicsRootConstantBufferView(1, cBufferData_[name]->matrix->resource->GetGPUVirtualAddress());

		if (funcName != DrawFuncType::DTriangle) {

			// directionalLight
			commandList->SetGraphicsRootConstantBufferView(3, cBufferData_[name]->light->resource->GetGPUVirtualAddress());
		}

		if (pipelineType == Primitive || pipelineType == Normal || pipelineType == GS) {

			// material
			commandList->SetGraphicsRootConstantBufferView(0, cBufferData_[name]->material->resource->GetGPUVirtualAddress());

			if (pipelineType == Normal || pipelineType == GS) {

				// pointLight
				commandList->SetGraphicsRootConstantBufferView(4, cBufferData_[name]->pointLight->resource->GetGPUVirtualAddress());
				// spotLight
				commandList->SetGraphicsRootConstantBufferView(5, cBufferData_[name]->spotLight->resource->GetGPUVirtualAddress());
			}
		}

		if (pipelineType == PhongReflection) {

			// phongRefMaterial
			commandList->SetGraphicsRootConstantBufferView(0, cBufferData_[name]->phongRefMaterial->resource->GetGPUVirtualAddress());
			// camera
			commandList->SetGraphicsRootConstantBufferView(4, cBufferData_[name]->camera->resource->GetGPUVirtualAddress());
			// pointLight用のCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(5, cBufferData_[name]->pointLight->resource->GetGPUVirtualAddress());
			// spotLight用のCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(6, cBufferData_[name]->spotLight->resource->GetGPUVirtualAddress());
		}
	}

	if (funcName == DrawFuncType::DParticle) {

		// material
		commandList->SetGraphicsRootConstantBufferView(0, cBufferData_[name]->material->resource->GetGPUVirtualAddress());
		// wvp
		commandList->SetGraphicsRootConstantBufferView(1, cBufferData_[name]->particleMatrix->resource->GetGPUVirtualAddress());
	}

	if (funcName == DrawFuncType::DGSModel) {

		// gsMatrix
		commandList->SetGraphicsRootConstantBufferView(0, cBufferData_[name]->gsMatrix_->resource->GetGPUVirtualAddress());
	}
}