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

		void translateCamera(const glm::vec3& diff);

		void rotateCamera(const glm::vec3& degrees);

	private:
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		// Update the Model-View-Projection (MVP) matrix.
		void updateMvpMatrix() const;

	private:
		// GLSL program.
		OglLib::ProgramGLSL m_program;

		// GLSL program ID.
		GLuint m_programId = {};

		GLuint m_vao = {};

		// Vertex buffer.
		GLuint m_vbo = {};

		// Index buffer.
		GLuint m_index = {};

		// Number of indices.
		GLsizei m_indexCount = {};

		// Colour buffer.
		GLuint m_color = {};

		std::unique_ptr<OglLib::CameraFps> m_spCamera;

		// MVP matrix uniform.
		GLint m_unMvp = { -1 };
	};
}
