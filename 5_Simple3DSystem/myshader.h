#ifndef MYSHADER_H
#define MYSHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyShader
{

public:
    MyShader(const char* vertext_path, const char* fragment_path);
    void use();

    // uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

public:
    GLuint program_;
    QOpenGLFunctions_3_3_Core *core_;
};

#endif // MYSHADER_H
