#include <filesystem>
#include <nanogui\nanogui.h>
#include <map>

class _TextureManager {
public:
	nanogui::ref<nanogui::Texture> getTexture(std::filesystem::path& path);

private:
	std::map<std::filesystem::path, nanogui::ref<nanogui::Texture>> _textureStore;
};

extern _TextureManager TextureManager;