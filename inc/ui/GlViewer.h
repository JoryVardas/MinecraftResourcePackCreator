#pragma once

#include <nanogui/nanogui.h>
#include <vector>


class GlViewer : public nanogui::Canvas {
public:
	GlViewer(nanogui::Widget* parent, nanogui::Vector2i size);
	virtual ~GlViewer() {
	};
	virtual void setVertices(const float const* vertices, size_t count);

	virtual void draw_contents() override;

private:
	nanogui::ref<nanogui::Shader> _shader;
	nanogui::ref<nanogui::Shader> _defaultShader;
	size_t _vertexCount = 0;
};