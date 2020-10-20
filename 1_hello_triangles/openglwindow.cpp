#include "openglwindow.h"

#include <QDebug>

OpenGLWindow::OpenGLWindow()
{

}

const char *k_vertex_shader_source = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";

const char *k_fragment_shader_source = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";


int g_shader_program;
unsigned int g_vao;

int OpenGLWindow::createShader(GLenum type, const char* source)
{
    int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log;
        return -1;
    }
    return shader;
}

GLuint OpenGLWindow::createProgram(int vertex_shader, int fragment_shader)
{

    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // check for linking errors
    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log;
        return -1;
    }
    return shader_program;
}


void OpenGLWindow::initializeGL()
{
    // 初始化 OpenGL函数，与QOpenGLFunctions_3_3_Core配合使用，之后便可以直接使用OpenGL函数
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertex_shader = createShader(GL_VERTEX_SHADER, k_vertex_shader_source);
    // fragment shader
    int fragment_shader = createShader(GL_FRAGMENT_SHADER, k_fragment_shader_source);
    // link shaders
    g_shader_program = createProgram(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
        0.0f,  0.5f, 0.0f  // top
    };

    unsigned int vbo;
    glGenVertexArrays(1, &g_vao);
    glGenBuffers(1, &vbo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered vbo as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the g_vao afterwards so other g_vao calls won't accidentally modify this g_vao, but this rarely happens. Modifying other
    // g_vaos requires a call to glBindVertexArray anyways so we generally don't unbind g_vaos (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLWindow::paintGL()
{
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(g_shader_program);
    glBindVertexArray(g_vao); // seeing as we only have a single g_vao there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0); // no need to unbind it every time
}
