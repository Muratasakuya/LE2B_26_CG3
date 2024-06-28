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

	Assimp::Importer impoter;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = impoter.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	// メッシュがないのには対応しない
	assert(scene->HasMeshes());

	// メッシュ解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

		aiMesh* mesh = scene->mMeshes[meshIndex];

		// 法線がないMeshは今回は非対応
		assert(mesh->HasNormals());

		// TexcoordがないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0));

		// face解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {

			aiFace& face = mesh->mFaces[faceIndex];

			// 三角形のみサポート
			assert(face.mNumIndices == 3);

			// vertex解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {

				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex;

				vertex.pos = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };

				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.pos.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				modelData.vertices.push_back(vertex);
			}
		}
	}

	// material解析
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {

		aiMaterial* material = scene->mMaterials[materialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {

			aiString textureFilePath;

			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	// 今のクラス設計上textureはTextureManagerで読んで使っているどっちがいいかはこれから要相談

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