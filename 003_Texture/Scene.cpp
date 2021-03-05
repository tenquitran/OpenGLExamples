#include "framework.h"
#include "Scene.h"
// stb library.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//////////////////////////////////////////////////////////////////////////

using namespace OglLib;
using namespace TextureApp;

//////////////////////////////////////////////////////////////////////////


Scene::~Scene()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (0 != m_vbo)
    {
        glDeleteBuffers(1, &m_vbo);
    }

    if (0 != m_index)
    {
        glDeleteBuffers(1, &m_index);
    }

    if (0 != m_color)
    {
        glDeleteBuffers(1, &m_color);
    }

    if (0 != m_texCoord)
    {
        glDeleteBuffers(1, &m_texCoord);
    }

    if (0 != m_texture)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &m_texture);
    }

    if (0 != m_vao)
    {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &m_vao);
    }
}

bool Scene::initialize()
{
    // Create GLSL program object.

    ShaderCollection shaders = {
        { GL_VERTEX_SHADER,   "shaders\\scene.vert" },
        { GL_FRAGMENT_SHADER, "shaders\\scene.frag" } };

    if (!m_program.initialize(shaders))
    {
        std::wcerr << L"GLSL program initialization failed\n";
        return false;
    }
    else if (!m_program.validate())
    {
        std::wcerr << L"GLSL program validation failed\n";
        return false;
    }

    m_programId = m_program.getProgram();

    glm::vec3 cameraPosition = { 0.0f, 0.0f, -2.0f };

    m_spCamera = std::make_unique<CameraFps>(cameraPosition);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Set up the vertex buffer.

    glm::vec3 origin = {};

    GLfloat side = 0.7f;

    const GLfloat HalfSide = side / 2;

    std::vector<GLfloat> vertices = {
        // front
        origin.x - HalfSide, origin.y - HalfSide, origin.z + HalfSide,
        origin.x + HalfSide, origin.y - HalfSide, origin.z + HalfSide,
        origin.x - HalfSide, origin.y + HalfSide, origin.z + HalfSide,
        origin.x + HalfSide, origin.y + HalfSide, origin.z + HalfSide,
        // back
        origin.x - HalfSide, origin.y - HalfSide, origin.z - HalfSide,
        origin.x + HalfSide, origin.y - HalfSide, origin.z - HalfSide,
        origin.x - HalfSide, origin.y + HalfSide, origin.z - HalfSide,
        origin.x + HalfSide, origin.y + HalfSide, origin.z - HalfSide };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    // Set up the index buffer.

    std::vector<GLuint> indices = {
        // front
        0, 1, 3,
        0, 3, 2,
        // back
        4, 7, 5,
        4, 6, 7,
        // right
        0, 6, 4,
        0, 2, 6,
        // left
        1, 5, 7,
        1, 7, 3,
        // top
        2, 3, 7,
        2, 7, 6,
        // bottom
        0, 5, 1,
        0, 4, 5 };

    m_indexCount = indices.size();

    glGenBuffers(1, &m_index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Set up the color buffer.

    std::vector<GLfloat> colors = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f };

    glGenBuffers(1, &m_color);
    glBindBuffer(GL_ARRAY_BUFFER, m_color);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), &colors[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_unMvp = glGetUniformLocation(m_programId, "mvp");
    if (-1 == m_unMvp)
    {
        std::cerr << "Failed to get uniform location: mvp\n";
        ATLASSERT(FALSE); return false;
    }

    if (!setupTexture())
    {
        std::cerr << "Failed to set up texture\n";
        ATLASSERT(FALSE); return false;
    }

    return true;
}

bool Scene::setupTexture()
{
    // Generate texture ID and bind it.

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Set the texture wrapping/filtering options.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load data from the texture file.

    int width, height;
    int channels;    // number of color channels

    std::string fileName = "media/brick.jpg";

    unsigned char *pData = stbi_load(fileName.c_str(), &width, &height, &channels, 0);

    if (!pData)
    {
        std::cerr << "Failed to load texture \"" << fileName << "\"\n";
        ATLASSERT(FALSE); return false;
    }

    // Specify a 2D texture image. This will allow the shaders to read the texture data.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pData);

    // Create and fill the texture coordinates buffer and set up the corresponding vertex attribute.

    glGenBuffers(1, &m_texCoord);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoord);

    std::vector<float> texCoords = {
        // front
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // back
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // right
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // left
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // top
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // bottom
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f };

    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);

    // The second argument is the number of components (2 for texture coordinates).
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(2);

    // For each texture sampler uniform, tell OpenGL to which texture unit it belongs to using glUniform1i().

    m_texSampler = glGetUniformLocation(m_programId, "ourTexture");

    if (-1 == m_texSampler)
    {
        std::cerr << "Failed to get texture sampler: ourTex\n";
        ATLASSERT(FALSE); return false;
    }

    glUseProgram(m_programId);

    glUniform1i(m_texSampler, 0);

    glUseProgram(0);

    return true;
}

void Scene::translateCamera(const glm::vec3& diff)
{
    m_spCamera->translate(diff);
}

void Scene::rotateCamera(const glm::vec3& degrees)
{
    m_spCamera->rotate(degrees);
}

void Scene::updateMvpMatrix() const
{
    ATLASSERT(-1 != m_unMvp);

    glUseProgram(m_programId);

    glm::mat4 mvp = m_spCamera->getModelViewProjectionMatrix();

    glUniformMatrix4fv(m_unMvp, 1, GL_FALSE, glm::value_ptr(mvp));

    glUseProgram(0);
}

void Scene::render() const
{
    ATLASSERT(m_programId);

    updateMvpMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_programId);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);

    // Activate texture unit zero and bind our texture to it (see Note 1 below).
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
