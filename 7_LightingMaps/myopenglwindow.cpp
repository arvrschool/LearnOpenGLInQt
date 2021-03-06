#include "myopenglwindow.h"
#include <iostream>
#include <QKeyEvent>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SPECULAR 1

MyOpenGLWindow::MyOpenGLWindow()
{
    camera_ = new MyCamera(glm::vec3(0.0f, 0.0f, 3.0f));
}

// world space positions of our cubes
glm::vec3 g_cube_positions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3 (2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

void MyOpenGLWindow::initializeGL()
{
    // 初始化 OpenGL函数，与QOpenGLFunctions_3_3_Core配合使用，之后便可以直接使用OpenGL函数
    initializeOpenGLFunctions();
#if SPECULAR
    basic_light_shader_ = new MyShader("F:/Projects/3DEngine/LearnOpenGLInQt/7_LightingMaps/Resources/Shaders/basic_light_map_specular_vert.txt", "F:/Projects/3DEngine/LearnOpenGLInQt/7_LightingMaps/Resources/Shaders/basic_light_map_specular_fragment.txt");
#else
    basic_light_shader_ = new MyShader("F:/Projects/3DEngine/LearnOpenGLInQt/7_LightingMaps/Resources/Shaders/basic_light_map_diffuse_vert.txt", "F:/Projects/3DEngine/LearnOpenGLInQt/7_LightingMaps/Resources/Shaders/basic_light_map_diffuse_fragment.txt");
#endif
    light_cube_shader_ = new MyShader("F:/Projects/3DEngine/LearnOpenGLInQt/7_LightingMaps/Resources/Shaders/light_cube_vert.txt", "F:/Projects/3DEngine/LearnOpenGLInQt/7_LightingMaps/Resources/Shaders/light_cube_fragment.txt");

    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    unsigned int vbo;
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texcoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &light_vao_);
    glBindVertexArray(light_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered vbo as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    std::string image_path = "F:/Projects/3DEngine/LearnOpenGLInQt/7_LightingMaps/Resources/Textures/container2.png";
    int width, height, nrChannels;
    unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
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

#if SPECULAR
    glGenTextures(1, &texture_0_);
    glBindTexture(GL_TEXTURE_2D, texture_0_);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载并生成纹理
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    image_path = "F:/Projects/3DEngine/LearnOpenGLInQt/7_LightingMaps/Resources/Textures/container2_specular.png";
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
#endif

    basic_light_shader_->use();
    basic_light_shader_->setInt("material.diffuse", 0);
#if SPECULAR
    basic_light_shader_->setInt("material.specular", 1);
#endif
    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    // -----------------------------------------------------------------------------------------------------------
    //    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.f / 960.f, 0.1f, 100.0f);
    //    shader_->setMat4("projection", projection);
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    time_.start();
}

void MyOpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

float counter = 0;
void MyOpenGLWindow::paintGL()
{
    GLfloat current_frame = (GLfloat)time_.elapsed()/100;
    delta_time_ = current_frame - last_frame_;
    last_frame_ = current_frame;

    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture_);
#if SPECULAR
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_0_);
#endif
    counter += 0.1f;
    float radius = 3.0f;
    //    float cam_x = sin(time_.elapsed()/1000) * radius;
    //    float cam_z = cos(time_.elapsed()/1000) * radius;
    float cam_x = sin(counter) * radius;
    float cam_z = cos(counter) * radius;

    glm::vec3 light_pos(cam_x, 0, cam_z);

    // draw our first triangle
    basic_light_shader_->use();

    basic_light_shader_->setVec3("light.position", light_pos);
    glm::vec3 light_color;
    light_color.x = sin(counter * 2.0f);
    light_color.y = sin(counter * 0.7f);
    light_color.z = sin(counter * 1.3f);
    glm::vec3 diffuse_color = light_color   * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambient_color = diffuse_color * glm::vec3(0.2f); // low influence
    basic_light_shader_->setVec3("light.ambient", ambient_color);
    basic_light_shader_->setVec3("light.diffuse", diffuse_color);
    basic_light_shader_->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    // material properties
    basic_light_shader_->setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    basic_light_shader_->setFloat("material.shininess", 32.0f);
    basic_light_shader_->setVec3("viewPos", camera_->position_);

    // camera/view transformation
    glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //    float radius = 10.0f;
    //    float cam_x = sin((float)time_.elapsed() / 1000) * radius;
    //    float cam_z = cos((float)time_.elapsed() / 1000) * radius;
    //    view = glm::lookAt(glm::vec3(cam_x, 0.0f, cam_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = camera_->getViewMatrix();
    basic_light_shader_->setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(camera_->zoom_), 1280.f / 960.f, 0.1f, 100.0f);
    basic_light_shader_->setMat4("projection", projection);
    glm::mat4 model = glm::mat4(1.0f);
    basic_light_shader_->setMat4("model", model);

    glBindVertexArray(vao_); // seeing as we only have a single g_vao there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 36);

    light_cube_shader_->use();
    light_cube_shader_->setMat4("view", view);
    light_cube_shader_->setMat4("projection", projection);
    model = glm::mat4(1.0f);
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    light_cube_shader_->setMat4("model", model);
    glBindVertexArray(light_vao_);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //    glBindVertexArray(0); // no need to unbind it every time

    update();
}

void MyOpenGLWindow::keyPressEvent(QKeyEvent *event)
{
    float move_speed = 2.5 * delta_time_;
    if(event->key() == Qt::Key_W){
        camera_->processKeyboard(Camera_Movement::FORWARD, move_speed);
    }
    if(event->key() == Qt::Key_S){
        camera_->processKeyboard(Camera_Movement::BACKWARD, move_speed);
    }
    if(event->key() == Qt::Key_A){
        camera_->processKeyboard(Camera_Movement::LEFT, move_speed);
    }
    if(event->key() == Qt::Key_D){
        camera_->processKeyboard(Camera_Movement::RIGHT, move_speed);
    }
    if(event->key() == Qt::Key_E){ //上升
        camera_->processKeyboard(Camera_Movement::UP, move_speed);
    }
    if(event->key() == Qt::Key_Q){ //下降
        camera_->processKeyboard(Camera_Movement::DOWN, move_speed);
    }
}

void MyOpenGLWindow::mouseMoveEvent(QMouseEvent *event)
{
    float x_pos = event->pos().x();
    float y_pos = event->pos().y();
    if (first_mouse_)
    {
        last_x_ = x_pos;
        last_y_ = y_pos;
        first_mouse_ = false;
    }

    float xoffset = x_pos - last_x_;
    float yoffset = last_y_ - y_pos; // reversed since y-coordinates go from bottom to top

    last_x_ = x_pos;
    last_y_ = y_pos;

    camera_->processMouseMovement(xoffset, yoffset);
}

void MyOpenGLWindow::wheelEvent(QWheelEvent *event)
{
    std::cout << "wheel event" << std::endl;
    QPoint offset = event->angleDelta();
    if(fov_ >= 1.0f && fov_ <=45.0f)
        fov_ -= ((GLfloat)offset.y())/20;
    if(fov_ < 1.0f)
        fov_ = 1.0f;
    if(fov_ > 45.0f)
        fov_ = 45.0f;
    camera_->processMouseScroll(fov_);
}
