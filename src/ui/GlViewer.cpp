#include "ui/GlViewer.h"
#include "nanogui/opengl.h"

using namespace nanogui;

GlViewer::GlViewer(Widget* parent, Vector2i size) : Canvas(parent,1) {
	set_fixed_size(size);

	_defaultShader = new Shader(
		render_pass(),
		"default_shader",

		//vertex shader
		R"(
			#version 330

			uniform mat4 mvp;

			in vec3 position;

			void main(){
				gl_Position = mvp * vec4(position, 1.0);
			}
		)",

		//fragment shader
		R"(
			#version 330

			out vec4 color;
			
			void main(){
				color = vec4(2);
			}
		)"

	);

	float cube[] = {

			1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f, -1.f, -1.f,
			1.f, -1.f, -1.f, 1.f, 1.f, -1.f, 1.f, 1.f, 1.f,

			-1.f, 1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, 1.f,
			-1.f, -1.f, 1.f, -1.f, 1.f, 1.f, -1.f, 1.f, -1.f,

			-1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f, 1.f, -1.f, -1.f, 1.f, -1.f,

			-1.f, -1.f, 1.f, -1.f, -1.f, -1.f, 1.f, -1.f, -1.f,
			1.f, -1.f, -1.f, 1.f, -1.f, 1.f, -1.f, -1.f, 1.f,

			-1.f, 1.f, 1.f, -1.f, -1.f, 1.f, 1.f, -1.f, 1.f,
			1.f, -1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 1.f, 1.f,

			1.f, 1.f, -1.f, 1.f, -1.f, -1.f, -1.f, -1.f, -1.f,
			-1.f, -1.f, -1.f, -1.f, 1.f, -1.f, 1.f, 1.f, -1.f
	};

	_shader = _defaultShader;

	setVertices(cube, 12 * 3);

};

void GlViewer::setVertices(const float const* vertices, size_t count) {
	_vertexCount = count;
	_shader->set_buffer("position", VariableType::Float32, { _vertexCount, 3 }, vertices);
};

void GlViewer::draw_contents() {
	Matrix4f view = Matrix4f::look_at(
		Vector3f(0, -5, 20),
		Vector3f(0, 0, 0),
		Vector3f(0, 1, 0)
	);

	Matrix4f model = Matrix4f::rotate(
		Vector3f(0, 1, 0),
		(float)glfwGetTime()
	);

	Matrix4f proj = Matrix4f::perspective(
		float(25 * 3.14159 / 180),
		0.1f,
		200.f,
		m_size.x() / (float)m_size.y()
	);

	Matrix4f mvp = proj * view * model;

	_shader->set_uniform("mvp", mvp);

	_shader->begin();
	_shader->draw_array(Shader::PrimitiveType::Triangle, 0, _vertexCount);
	_shader->end();

};