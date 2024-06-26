#include "ModelManager.h"



/*////////////////////////////////////////////////////////////////////////////////

*									singleton

////////////////////////////////////////////////////////////////////////////////*/
ModelManager* ModelManager::GetInstance() {

	static ModelManager instance;

	return &instance;
}



/*////////////////////////////////////////////////////////////////////////////////

*								Mtlファイルを読む関数

////////////////////////////////////////////////////////////////////////////////*/
ModelMaterialData ModelManager::LoadMaterialTemplateFile(const std::string& directorypath, const std::string& filename) {

	ModelMaterialData materialData;
	std::string line;
	std::ifstream file(directorypath + "/" + filename);

	// 開けなかったら止める
	assert(file.is_open());

	while (std::getline(file, line)) {

		std::string identifier;
		std::istringstream s(line);
		s >> identifier;            // 先頭の識別子を読む

		// identifierに応じた処理
		// map_Kd = textureのファイル名
		if (identifier == "map_Kd") {

			std::string textureFilename;
			s >> textureFilename;

			// 連結してファイルパスにする
			materialData.textureFilePath = directorypath + "/" + textureFilename;
		}
	}

	return materialData;
}



/*////////////////////////////////////////////////////////////////////////////////

*								Objファイルを読む関数

////////////////////////////////////////////////////////////////////////////////*/
ModelData ModelManager::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	ModelData modelData;            // 構築するModelData
	std::vector<Vector4> positions;       // 位置
	std::vector<Vector3> normals;   // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line;               // ファイルから読んだ1行を格納するもの

	// ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);
	// 開けなかったら止める
	assert(file.is_open());

	while (std::getline(file, line)) {

		std::string identifier;
		std::istringstream s(line);
		s >> identifier;            // 先頭の識別子を読む

		// identifierに応じた処理
		// v = 頂点位置
		// vt = 頂点テクスチャ座標
		// vn = 頂点法線
		// f = 面
		if (identifier == "v") {

			Vector4 position;
			s >> position.x >> position.y >> position.z;
			// X軸を反転
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		} else if (identifier == "vt") {

			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			// Y軸を反転
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		} else if (identifier == "vn") {

			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			// X軸を反転
			normal.x *= -1.0f;
			normals.push_back(normal);
		} else if (identifier == "f") {

			VertexData triangle[3];

			// 面は三角形限定、その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {

				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3] = { 0,0,0 };
				for (int32_t element = 0; element < 3; ++element) {

					std::string index;
					// 区切りでIndexを読む
					std::getline(v, index, '/');

					if (!index.empty()) {

						elementIndices[element] = std::stoi(index);
					}
				}

				// 要素へのIndexから、実際の要素の値を取得して、頂点は構築する
				Vector4 position = positions[elementIndices[0] - 1];
				//Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector2 texcoord = elementIndices[1] > 0 ? texcoords[elementIndices[1] - 1] : Vector2(0.0f, 0.0f);
				Vector3 normal = normals[elementIndices[2] - 1];

				triangle[faceVertex] = { position,texcoord,normal };
			}

			// 頂点を逆順で登録して回り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		} else if (identifier == "mtllib") {

			// materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;

			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}

	return modelData;
}



/*////////////////////////////////////////////////////////////////////////////////

*								モデルを作成する関数

////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::LoadModel(const std::string& directoryPath, const std::string& filename) {

	// ファイルパスからモデル名を取得
	std::filesystem::path path(filename);
	// 拡張子を除いたモデル名を取得
	std::string identifier = path.stem().string();

	ModelData modelData = LoadObjFile(directoryPath, filename);
	models_[identifier] = modelData;

	modelMesh_->CreateModelMesh(identifier, static_cast<UINT>(models_[identifier].vertices.size()));
}



/*////////////////////////////////////////////////////////////////////////////////

*							自作したモデルを作成する関数

////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::MakeModel(ModelData modelData, const std::string& modelName) {

	modelMesh_->CreateModelMesh(modelName, static_cast<UINT>(modelData.vertices.size()));

	models_[modelName] = modelData;
}



/*////////////////////////////////////////////////////////////////////////////////

*							モデルメッシュインスタンスの生成

////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::Initialize() {

	modelMesh_ = std::make_unique<Model>();
}



/*////////////////////////////////////////////////////////////////////////////////

*								頂点バッファへデータ転送

////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::VertexBufferMemcpy(const std::string& modelName) {

	std::memcpy(modelMesh_->GetModel(modelName)->data,
		models_[modelName].vertices.data(), sizeof(VertexData) * models_[modelName].vertices.size());
}



/*////////////////////////////////////////////////////////////////////////////////

*								　頂点バッファの設定

////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::IASetVertexBuffers(ID3D12GraphicsCommandList* commandList, const std::string& modelName) {

	commandList->IASetVertexBuffers(0, 1, &modelMesh_->GetModel(modelName)->vertexBufferView);
}



/*////////////////////////////////////////////////////////////////////////////////

*									 モデル描画

////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::ModelDrawCall(ID3D12GraphicsCommandList* commandList, const std::string& modelName) {

	commandList->DrawInstanced(UINT(models_[modelName].vertices.size()), 1, 0, 0);
}