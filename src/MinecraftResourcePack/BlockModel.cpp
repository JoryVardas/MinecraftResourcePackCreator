#include <nlohmann/json.hpp>

#include "BlockModel.h"

using json = nlohmann::json;
using namespace std;

_BlockModelManager BlockModelManager;

BlockModel::BlockModel(const filesystem::path& modelFile) {

	if(modelFile.)
	ifstream blockModelFile(modelFile);

	if (!blockModelFile.good()) {
		blockModelFile.close();
		_isValid = false;
		return;
	}


	json blockModel;
	blockModelFile >> blockModel;

	blockModelFile.close();

	if (blockModel.contains("parent")) {
		_parent = BlockModelManager.getBlockModel(blockModel.at("parent").get<std::string>());
	}
};

std::pair<Vertex*, size_t> BlockModel::getModelData() {
	if (!_modelData && _parent) return _parent->getModelData();
	return { _modelData.get(), _modelDataLength };
};
vector<ref<Texture>>& BlockModel::getTextures() {
	return _textures;
};




shared_ptr<BlockModel> _BlockModelManager::getBlockModel(path& path) {
	if (_textureStore.contains(path)) return _blockModelStore.at(path);

	shared_ptr<BlockModel> blockModel = make_shared<BlockModel>(path);
	_blockModelStore[path] = blockModel;
	return blockModel;
};