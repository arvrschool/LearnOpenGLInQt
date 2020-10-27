#include "myshader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


MyShader::MyShader(const char* vertext_path, const char* fragment_path)
{
    core_ = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    // 1. 从文件路径中获取顶点/片段着色器
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;
    // 保证ifstream对象可以抛出异常：
    v_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        v_shader_file.open(vertext_path);
        f_shader_file.open(fragment_path);
        std::stringstream v_shader_stream, f_shader_stream;
        // 读取文件的缓冲内容到数据流中
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();
        // 关闭文件处理器
        v_shader_file.close();
        f_shader_file.close();
        // 转换数据流到string
        vertex_code   = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();

    // 2. 编译着色器
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // 顶点着色器
    vertex = core_->glCreateShader(GL_VERTEX_SHADER);
    core_->glShaderSource(vertex, 1, &v_shader_code, NULL);
    core_->glCompileShader(vertex);
    // 打印编译错误（如果有的话）
    core_->glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        core_->glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 片段着色器
    fragment = core_->glCreateShader(GL_FRAGMENT_SHADER);
    core_->glShaderSource(fragment, 1, &f_shader_code, NULL);
    core_->glCompileShader(fragment);
    // 打印编译错误（如果有的话）
    core_->glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        core_->glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // 3. link
    program_ = core_->glCreateProgram();
    core_->glAttachShader(program_, vertex);
    core_->glAttachShader(program_, fragment);
    core_->glLinkProgram(program_);
    // 打印连接错误（如果有的话）
    core_->glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if(!success)
    {
        core_->glGetProgramInfoLog(program_, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::program_::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    core_->glDeleteShader(vertex);
    core_->glDeleteShader(fragment);
}

void MyShader::use()
{
    core_->glUseProgram(program_);
}

// uniform工具函数
void MyShader::setBool(const std::string &name, bool value) const
{
    GLint location = core_->glGetUniformLocation(program_, name.c_str());
    core_->glUniform1i(location, value);
}

void MyShader::setInt(const std::string &name, int value) const
{
    GLint location = core_->glGetUniformLocation(program_, name.c_str());
    core_->glUniform1i(location, value);
}

void MyShader::setFloat(const std::string &name, float value) const
{
    GLint location = core_->glGetUniformLocation(program_, name.c_str());
    core_->glUniform1f(location, value);
}

void MyShader::setMat4(const std::string &name, glm::mat4 value) const
{
    GLint location = core_->glGetUniformLocation(program_, name.c_str());
    core_->glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void MyShader::setVec3(const std::string &name, glm::vec3 value) const
{
    GLint location = core_->glGetUniformLocation(program_, name.c_str());
    core_->glUniform3fv(location, 1, glm::value_ptr(value));
}

void MyShader::setVec3(const std::string &name, float x, float y, float z) const
{
    GLint location = core_->glGetUniformLocation(program_, name.c_str());
    glm::vec3 value = glm::vec3(x, y, z);
    core_->glUniform3fv(location, 1, glm::value_ptr(value));
}
