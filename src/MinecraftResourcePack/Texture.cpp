#include "MinecraftResourcePack/Texture.h"

using namespace nanogui;
using namespace std::filesystem;

_TextureManager TextureManager;

ref<Texture> _TextureManager::getTexture(path& path) {
	if (_textureStore.contains(path)) return _textureStore.at(path);

	ref<Texture> texture = new Texture(path.string());
	_textureStore[path] = texture;
	return texture;
};