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
	// レベルデータ
	struct ObjectData {

		std::string type;
		std::string name;
		// オブジェクト1個分のデータ
		struct Transform {
			// 各トランスフォーム
			KamataEngine::Vector3 translation;
			KamataEngine::Vector3 rotation;
			KamataEngine::Vector3 scaling;
		};

		Transform transform;

		std::string fileName;

		// オブジェクトのコンテナ
		//std::vector<ObjectData> objects;
	};

	struct LevelData {
		std::string name;

		std::list<ObjectData> objects;
	};

	static inline const std::string kDefaultBaseDirectory = "Resources/level/";
	static inline const std::string fileName = "test.json";
	//const std::string kExtension = "";

	std::unique_ptr<LevelData> levelData;
	std::map<std::string, KamataEngine::Model*> models;

	std::vector<KamataEngine::WorldTransform*> worldtransforms;
	
	std::unique_ptr<KamataEngine::Model> testModel;
	KamataEngine::WorldTransform testWorldTransform;

	KamataEngine::Camera camera;
};
