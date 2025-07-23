#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	levelData = new LevelData();

	//-------------------------------------------------------------------
	// jsonファイルのデシリアライズ化
	//-------------------------------------------------------------------

	// jsonファイルのパス名
	const std::string fullpath = std::string("Resources/level/") + std::string("test.json");

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	nlohmann::json deserialized; // deserialized : 逆シリアライズ化
	                             // ⇒1つの文字列をメモリ中のデータ構造化すること
	                             // serialize:一列に並べる操作の事
	                             // ⇒1つの文字列で表現する「直列化」のこと

	// ファイルから読み込み、メモリへ格納
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());         // object か ※json形式には様々な型がある
	                                          // object型はその中でも「キーと値のペアを持つ構造」つまり連想配列が扱えるか聞いている
	assert(deserialized.contains("name"));    //"name"が含まれているか
	assert(deserialized["name"].is_string()); //["name"]は文字列か？

	// "name"を文字列として取得
	levelData->name = deserialized["name"].get<std::string>();
	assert(levelData->name == "scene"); // それは"scene"か？

	//// "name"を文字列として取得
	// std::string name = deserialized["name"].get<std::string>();
	//// 正しいレベルデータかチェック
	// assert(name.compare("scene") == 0);

	//// レベルデータ格納用インスタンスを生成
	// LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		// オブジェクト 一つ分の妥当性のチェック
		assert(object.contains("type")); //"type""が含まれているか
		// std::string type = object["type"].get<std::string>();

		if (object["type"].get<std::string>() == "MESH") {
			// 1個分の要素の準備
			levelData->objects.emplace_back(ObjectData{});
			ObjectData& objectData = levelData->objects.back(); // 追加要素の参照を用意し可読性もよくなる

			objectData.type = object["type"].get<std::string>(); //"type"
			objectData.name = object["name"].get<std::string>(); //"name"

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平行移動 "translation"
			objectData.transform.translation.x = (float)transform["translation"][1];
			objectData.transform.translation.y = (float)transform["translation"][2];
			objectData.transform.translation.z = (float)transform["translation"][0];
			// 回転角 "rotation"
			objectData.transform.rotation.x = -(float)transform["rotation"][1];
			objectData.transform.rotation.y = -(float)transform["rotation"][2];
			objectData.transform.rotation.z = -(float)transform["rotation"][0];
			// 拡大縮小 "scaling"
			objectData.transform.scaling.x = (float)transform["scaling"][1];
			objectData.transform.scaling.y = (float)transform["scaling"][2];
			objectData.transform.scaling.z = (float)transform["scaling"][0];

			//"file_name"
			if (object.contains("file_name")) {
				objectData.file_name = object["file_name"].get<std::string>();
			}

			//-------------------------------------------------------------------
			// レベルデータからオブジェクトを生成、配置
			//-------------------------------------------------------------------
			for (auto& objectData_ : levelData->objects) {
				// 登録モデルを検索
				decltype(models)::iterator it = models.find(objectData_.file_name);
				if (it != models.end()) {
					Model* model = Model::CreateFromOBJ(objectData.file_name);
					models[objectData.file_name] = model;
				}
				// 3Dオブジェクトを生成
				WorldTransform* newObject = new WorldTransform();
				// 位置の設定 objectData.transform.translation	に入っている
				newObject->translation_ = objectData_.transform.translation;
				// 回転の設定 objectData.transform.rotation		に入っている
				newObject->rotation_ = objectData_.transform.rotation;
				// 拡大縮小　objectData.scaling					に入っている
				newObject->scale_ = objectData_.transform.scaling;

				newObject->Initialize();

				// 配列に登録
				worldTransforms.push_back(std::move(newObject));
			}
		}

		//// MESH
		// if (type.compare("MESH") == 0) {
		//	// 要素追加
		//	levelData->objects.emplace_back(LevelData::ObjectData{});
		//	// 今追加した要素の参照を得る
		//	LevelData::ObjectData& objectData = levelData->objects.back();

		//	if (object.contains("file_name")) {
		//		// ファイル名
		//		objectData.fileName = object["file_name"];
		//	}
		//	// トランスフォームのパラメータ読み込み
		//	nlohmann::json& transform = object["transform"];
		//	// 平行移動 "translation"
		//	objectData.translation.x = (float)transform["translation"][1];
		//	objectData.translation.y = (float)transform["translation"][2];
		//	objectData.translation.z = (float)transform["translation"][0];
		//	// 回転角 "rotation"
		//	objectData.rotation.x = -(float)transform["rotation"][1];
		//	objectData.rotation.y = -(float)transform["rotation"][2];
		//	objectData.rotation.z = -(float)transform["rotation"][0];
		//	// 拡大縮小 "scaling"
		//	objectData.scaling.x = (float)transform["scaling"][1];
		//	objectData.scaling.y = (float)transform["scaling"][2];
		//	objectData.scaling.z = (float)transform["scaling"][0];
		//	// TODO: コライダーのパラメータ読み込み
		//}
		//// 再帰処理
		//// TODO:　オブジェクト走査を再起関数にまとめ、再帰呼出で枝を走査する
		// if (object.contains("children")) {
		// }
	}

	//// レベルデータからオブジェクトを生成、配置
	// for (auto& objectData : levelData->objects) {
	//	// ファイル名から登録済みモデルを検索
	//	Model* model = nullptr;
	//	//decltype(models)
	// }
	camera_.Initialize();
}
void GameScene::Update() {
	for (WorldTransform* object : worldTransforms) {

		object->TransferMatrix();
	}

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

	//camera.UpdateMatrix();
}

void GameScene::Draw() {
	Model::PreDraw();

	int i = 0;
	//-------------------------------------------------------------------
	// レベルデータからオブジェクトを生成、配置
	//-------------------------------------------------------------------
	for (auto& objectData : levelData->objects) {
		// 登録モデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.file_name);
		if (it != models.end()) {
			model = it->second;
		} else {
			model->CreateFromOBJ("Cube");
		}
		model->Draw(*worldTransforms[i], camera_);

		i++;
	}

	Model::PostDraw();
}