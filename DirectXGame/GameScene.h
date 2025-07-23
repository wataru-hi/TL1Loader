#pragma once
#include <kamataEngine.h>
#include <json.hpp>

#include <algorithm>
#include <fstream>
#include <memory>
#include <list>
#include <map>

class GameScene {
public:
	void Initialize();
	void Update();
	void Draw();

private:
	struct ObjectData {
		std::string type; // "type"
		std::string name; // "name"

		struct Transform {
			KamataEngine::Vector3 translation; // translation
			KamataEngine::Vector3 rotation;    // rotation
			KamataEngine::Vector3 scaling;     // scaling
		};

		Transform transform; // メンバの準備

		//"file_name"
		std::string file_name;

		//// オブジェクトのコンテナ
		// std::vector<ObjectData> objects;
	};

	// レベルデータ
	struct LevelData {
		// "name"
		std::string name;
		// "objects"
		std::vector<ObjectData> objects;
	};

	// モデルデータコンテナ
	std::map<std::string, KamataEngine::Model*> models;

	// ワールドトランスフォーム
	std::vector<KamataEngine::WorldTransform*> worldTransforms;

	//-------------------------------------------------------------------
	// レベルデータを構造体に格納していく
	//-------------------------------------------------------------------
	LevelData* levelData = nullptr;

	KamataEngine::Camera camera_;
};
