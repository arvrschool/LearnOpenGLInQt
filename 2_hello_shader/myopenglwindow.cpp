#include "myopenglwindow.h"

MyOpenGLWindow::MyOpenGLWindow()
{
}
void MyOpenGLWindow::initializeGL()
{
    // 初始化 OpenGL函数，与QOpenGLFunctions_3_3_Core配合使用，之后便可以直接使用OpenGL函数
    initializeOpenGLFunctions();
//    shader_ = new MyShader(":/shaders/Resources/Shaders/vertex.txt", ":/shaders/Resources/Shaders/fragment.txt");
    shader_ = new MyShader("F:/Projects/3DEngine/LearnOpenGLInQt/2_hello_shader/Resources/Shaders/vertex.txt", "F:/Projects/3DEngine/LearnOpenGLInQt/2_hello_shader/Resources/Shaders/fragment.txt");
    glEnable(GL_DEPTH_TEST);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
//    float vertices[] = {
//        -0.5f, -0.5f, 0.0f, // left
//        0.5f, -0.5f, 0.0f, // right
//        0.0f,  0.5f, 0.0f  // top
//    };
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    unsigned int vbo, ebo;
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered vbo as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the g_vao afterwards so other g_vao calls won't accidentally modify this g_vao, but this rarely happens. Modifying other
    // g_vaos requires a call to glBindVertexArray anyways so we generally don't unbind g_vaos (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void MyOpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MyOpenGLWindow::paintGL()
{
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw our first triangle
    shader_->use();
    glBindVertexArray(vao_); // seeing as we only have a single g_vao there's no need to bind it every time, but we'll do so to keep things a bit more organized
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // no need to unbind it every time
}
