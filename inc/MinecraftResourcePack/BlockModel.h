#include "Common.h"
#include "Texture.h"
#include <filesystem>
using vec3 = std::tuple<float, float, float>;
using vec4 = std::tuple<float, float, float, float>;
enum class Axis {
	X, Y, Z
};

enum class BlockFace {
	DOWN,
	UP,
	NORTH,
	SOUTH,
	WEST,
	EAST,
	INVALID
};
std::string blockFaceToString(const BlockFace& blockFace) {
	switch (blockFace) {
		using enum BlockFace;
		case DOWN:
			return "down";
		case UP:
			return "up";
		case NORTH:
			return "north";
		case SOUTH:
			return "south";
		case WEST:
			return "west";
		case EAST:
			return "east";
		default:
			return "";
	}
};
BlockFace blockFaceFromString(const std::string& str) {
	if (str == "down") return BlockFace::DOWN;
	else if (str == "up") return BlockFace::UP;
	else if (str == "north") return BlockFace::NORTH;
	else if (str == "south") return BlockFace::SOUTH;
	else if (str == "west") return BlockFace::WEST;
	else if (str == "east") return BlockFace::EAST;
	else return BlockFace::INVALID;
};

class BlockModel {
public:
	struct Vertex {
		float _x, y, z;
		float _u, v;
		int _textureIndex;
	};
	struct DisplayPosition {
		vec3 _rotation;
		vec3 _translation;
		vec3 _scale;
	};
	struct Element {
		vec3 _from;
		vec3 _to;
		struct Rotation {
			vec3 _origin;
			Axis _axis;
			float _angle;
			bool _rescale = false;
		} _rotation;
		bool _shade = true;
		struct Face {
			vec4 _uv;
			std::string _texture;
			BlockFace _cullface;
			int _rotation = 0;
			int _tintIndex;
		};
		std::map<BlockFace, Face> _faces;
	};

	BlockModel(const std::filesystem::path& modelFile);

	std::pair<Vertex*, size_t> getModelData();
	std::vector<nanogui::ref<nanogui::Texture>>& getTextures();


private:
	std::unique_ptr<Vertex[]> _modelData = nullptr;
	size_t _modelDataLength = 0;
	bool _isValid = true;

	std::shared_ptr<BlockModel> _parent = nullptr;
	bool _ambientOcclusion = true;
	std::vector<DisplayPosition> _display;
	std::map<std::string, nanogui::ref<nanogui::Texture>> _textures;
	std::vector<Element> _elements;
};

class _BlockModelManager {
public:
	std::shared_ptr<BlockModel> getBlockModel(std::filesystem::path& path);

private:
	std::map<std::filesystem::path, std::shared_ptr<BlockModel>> _blockModelStore;
};

extern _BlockModelManager BlockModelManager;