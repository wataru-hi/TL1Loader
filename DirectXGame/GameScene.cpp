#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	const std::string fullpath = kDefaultBaseDirectory + fileName;

	std::ifstream file;

	file.open(fullpath);
	if (file.fail())
		assert(0);

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	levelData->name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(levelData->name.compare("scene") == 0);

	// "objects"の全オブジェクトを走査
	for (nlohmann::json object : deserialized["objects"]) {
		assert(object.contains("type"));

		//// 種別を取得
		// std::string type = object["type"].get<std::string>();

		//// 要素追加
		// levelData->objects.emplace_back(LevelData::ObjectData{});
		//  今追加した要素の参照を得る
		//  LevelData::ObjectData& objectData = levelData->objects.back();

		if (object["type"].get<std::string>() == "MESH") {
			levelData->objects.emplace_back(ObjectData{});
			ObjectData& objectData = levelData->objects.back();

			objectData.type = object["type"].get<std::string>();
			objectData.name = object["name"].get<std::string>();

			// トランスフォームのパラメータ読み込み
			// トランスフォームのパラメータ読み込み
			nlohmann::json transform = object["transform"];
			// 平行移動
			objectData.transform.translation.x = (float)transform["translation"][0];
			objectData.transform.translation.y = (float)transform["translation"][2];
			objectData.transform.translation.z = (float)transform["translation"][1];
			// 回転角
			objectData.transform.rotation.x = -(float)transform["rotation"][0];
			objectData.transform.rotation.y = -(float)transform["rotation"][2];
			objectData.transform.rotation.z = -(float)transform["rotation"][1];
			// スケーリング
			objectData.transform.scaling.x = (float)transform["scaling"][0];
			objectData.transform.scaling.y = (float)transform["scaling"][2];
			objectData.transform.scaling.z = (float)transform["scaling"][1];

			if (object.contains("file_name")) {
				objectData.fileName = object["file_name"].get<std::string>();
			}

			for (auto& objectData_ : levelData->objects) {
				decltype(models)::iterator it = models.find(objectData_.fileName);
				
				if (it != models.end()) {
					//currentModel = std::make_shared<Model>(Model::CreateFromOBJ(objectData.fileName));
					Model* currentModel = Model::CreateFromOBJ(objectData.fileName);
					models[objectData.fileName] = currentModel;
				} else {
					assert(0);
				}
				WorldTransform newWT;
				newWT.Initialize();
				newWT.rotation_ = objectData.transform.rotation;
				newWT.translation_ = objectData.transform.translation;
				newWT.scale_= objectData.transform.scaling;

				worldtransforms.push_back(&newWT);
				
				
				/*newWT.Initialize();
				newWT.translation_ = objectData.transform.translation;
				newWT.rotation_ = objectData.transform.rotation;
				newWT.scale_ = objectData.transform.scaling;*/

				/*ModelDeta newModelData;
				newModelData.model = currentModel;
				newModelData.worldTransform.Initialize();
				newModelData.worldTransform.translation_ = objectData.transform.translation;
				newModelData.worldTransform.rotation_ = objectData.transform.rotation;
				newModelData.worldTransform.scale_ = objectData.transform.scaling;


				modelList.push_back(std::move(newModelData));*/
				//	newModelData.worldTransform.translation_ = objectData.translation;
				//	newModelData.worldTransform.rotation_ = objectData.rotation;
				//	newModelData.worldTransform.scale_ = objectData.scaling;

				//	modelList.push_back(std::move(newModelData));
			}
		}
	}

	// if (object.contains("file_name")) {
	//	// ファイル名
	//	objectData.fileName = object["file_name"];
	// }

	// ... 再帰処理またはその他の処理 ...
	// 種類ごとの処理
	// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼び出しで枝を走査する
	// if (object.contains("children")) {
	//	// ... recursive call or further processing for children ...
	//	break;
	//}

	//// レベルデータからオブジェクトを生成、配置
	// for (auto& objectData : levelData->objects) {
	//	ModelDeta newModelData{}; // スタック上で生成
	//	// ファイル名から登録済みモデルを検索
	//	auto it = models.find(objectData.fileName);
	//	if (it != models.end()) {
	//		newModelData.model = it->second;
	//	} else {
	//		newModelData.model = std::shared_ptr<Model>(Model::CreateFromOBJ(objectData.fileName));
	//	}
	//	// モデルを指定して3Dオブジェクトを生成
	//	newModelData.worldTransform.Initialize();
	//	newModelData.worldTransform.translation_ = objectData.translation;
	//	newModelData.worldTransform.rotation_ = objectData.rotation;
	//	newModelData.worldTransform.scale_ = objectData.scaling;

	//	modelList.push_back(std::move(newModelData));

	camera.Initialize();
	testModel = std::unique_ptr<Model>(Model::CreateFromOBJ("Cube"));
	testWorldTransform.Initialize();
}
void GameScene::Update() {
	ImGui::Begin("Camera");
	ImGui::DragFloat3("pos", &camera.translation_.x, 0.1f);
	ImGui::DragFloat3("rot", &camera.rotation_.x, 0.1f);
	ImGui::End();

	//ImGui::Begin("modelList");
	//// modelListの各モデルについて情報を表示
	//for (auto& modelData : modelList) {
	//	// モデルの名前（識別子として使用）
	//	// オブジェクトデータからファイル名を取得できる場合、それを使用すると良いでしょう
	//	// 例: ImGui::Text("Model: %s", objectData.fileName.c_str());
	//	// ここでは仮にインデックスを使用します
	//	ImGui::PushID(&modelData); // 各モデルを一意に識別するためのIDスタック

	//	// ワールド変換のTranslation情報を表示・編集
	//	// Vector3をfloat[3]に変換
	//	float translation[3] = {modelData.worldTransform.translation_.x, modelData.worldTransform.translation_.y, modelData.worldTransform.translation_.z};

	//	// ImGui::DragFloat3: スライダーのようにドラッグして値を変更
	//	if (ImGui::DragFloat3("Translation (Drag)", translation, 0.1f, -100.0f, 100.0f, "%.3f")) {
	//		modelData.worldTransform.translation_.x = translation[0];
	//		modelData.worldTransform.translation_.y = translation[1];
	//		modelData.worldTransform.translation_.z = translation[2];
	//	}

	//	// 必要であれば、RotationやScalingも同様に追加できます
	//	// float rotation[3] = { modelData.worldTransform.rotation_.x, modelData.worldTransform.rotation_.y, modelData.worldTransform.rotation_.z };
	//	// if (ImGui::InputFloat3("Rotation", rotation, "%.3f")) { /* ... */ }

	//	ImGui::PopID();     // IDスタックを戻す
	//	ImGui::Separator(); // 各モデルの区切り
	//}

	//ImGui::End(); // デバッグウィンドウの終了

	camera.UpdateMatrix();
}

void GameScene::Draw() {
	Model::PreDraw();

	testModel->Draw(testWorldTransform, camera);

	int i = 0;
	for (auto& objectData : levelData->objects) {
		// 登録モデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}
		model->Draw(*worldtransforms[i], camera);

		i++;
	}

	Model::PostDraw();
}