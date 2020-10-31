#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// constructor, expects a filepath to a 3D model.
Model::Model(string const &path, bool gamma) : gamma_correction_(gamma)
{
    core_ = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    stbi_set_flip_vertically_on_load(true);
    loadModel(path);
}

// draws the model, and thus all its meshes
void Model::draw(MyShader &shader)
{
    for (unsigned int i = 0; i < meshes_.size(); i++)
        meshes_[i].draw(shader);
}

unsigned int Model::textureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    core_->glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        core_->glBindTexture(GL_TEXTURE_2D, textureID);
        core_->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        core_->glGenerateMipmap(GL_TEXTURE_2D);

        core_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        core_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        core_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        core_->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
