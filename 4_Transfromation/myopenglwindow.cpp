#include "myopenglwindow.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


MyOpenGLWindow::MyOpenGLWindow()
{
}
void MyOpenGLWindow::initializeGL()
{
    // 初始化 OpenGL函数，与QOpenGLFunctions_3_3_Core配合使用，之后便可以直接使用OpenGL函数
    initializeOpenGLFunctions();
    shader_ = new MyShader("F:/Projects/3DEngine/LearnOpenGLInQt/4_Transfromation/Resources/Shaders/vertex_mix.txt", "F:/Projects/3DEngine/LearnOpenGLInQt/4_Transfromation/Resources/Shaders/fragment_mix.txt");
    glEnable(GL_DEPTH_TEST);

    float vertices[] = {

        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 *sizeof (float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 *sizeof (float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered vbo as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the g_vao afterwards so other g_vao calls won't accidentally modify this g_vao, but this rarely happens. Modifying other
    // g_vaos requires a call to glBindVertexArray anyways so we generally don't unbind g_vaos (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载并生成纹理
    std::string image_path = "F:/Projects/3DEngine/LearnOpenGLInQt/4_Transfromation/Resources/Textures/container.jpg";
    int width, height, nrChannels;
    unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture_0_);
    glBindTexture(GL_TEXTURE_2D, texture_0_);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载并生成纹理
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    image_path = "F:/Projects/3DEngine/LearnOpenGLInQt/4_Transfromation/Resources/Textures/awesomeface.png";
    data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    shader_->use();
    shader_->setInt("texture1", 0);
    shader_->setInt("texture2", 1);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    time_.start();
}
void MyOpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MyOpenGLWindow::paintGL()
{
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture_);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_0_);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)time_.elapsed() / 1000, glm::vec3(0.0f, 0.0f, 1.0f));

    std::cout << time_.elapsed() << std::endl;

    // draw our first triangle
    shader_->use();

    GLuint transformLocation = glGetUniformLocation(shader_->program_, "transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(vao_); // seeing as we only have a single g_vao there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // no need to unbind it every time

    update();
}
