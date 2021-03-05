#pragma once


namespace TriangleApp
{
	class Scene
	{
	public:
		Scene() = default;

		virtual ~Scene();

		bool initialize();

		void render() const;

	private:
		// GLSL program.
		OglLib::ProgramGLSL m_program;

		// GLSL program ID.
		GLuint m_programId = {};

		GLuint m_vao = {};

		// Vertext buffer.
		GLuint m_vbo = {};

		// Index buffer.
		GLuint m_index = {};

		// Number of indices.
		GLsizei m_indexCount = {};

		// Colour buffer.
		GLuint m_color = {};

		GLsizei m_vertexCount = {};
	};
}
