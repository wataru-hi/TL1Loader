#pragma once
#include "kamataEngine.h"
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
	static inline const std::string kDefaultBaseDirectory = "Resources/level/";
	static inline const std::string fileName = "test.json";
	//const std::string kExtension = "";

	struct  modelDeta {
		//KamataEngine::Model* model;
		std::shared_ptr<KamataEngine::Model> model;
		KamataEngine::WorldTransform worldTransform;
	};

	std::list<modelDeta> modelList;
	//std::vector<modelDeta> modelList;

	//std::vector<KamataEngine::Model*> modelDate;
	//std::vector<KamataEngine::WorldTransform> worldTransformDate;

	std::map<std::string, std::shared_ptr<KamataEngine::Model>> models;

	std::unique_ptr<KamataEngine::Model> testModel;
	KamataEngine::WorldTransform testWT;

	KamataEngine::Camera camera;
};
