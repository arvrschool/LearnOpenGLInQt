#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions_3_3_Core>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "myshader.h"

using namespace std;

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 tex_coords;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
public:
    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        if(core_ == NULL)
            core_ = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

        this->vertices_ = vertices;
        this->indices_ = indices;
        this->textures_ = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void draw(MyShader &shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for(unsigned int i = 0; i < textures_.size(); i++)
        {
            core_->glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures_[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if(name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if(name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit
            core_->glUniform1i(core_->glGetUniformLocation(shader.program_, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures_[i].id);
        }

        // draw mesh
        core_->glBindVertexArray(vao_);
        core_->glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (const GLvoid*)0);
        core_->glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        core_->glActiveTexture(GL_TEXTURE0);
    }

private:
    void setupMesh()
    {
        core_->glGenVertexArrays(1, &vao_);
        core_->glGenBuffers(1, &vbo_);
        core_->glGenBuffers(1, &ebo_);

        core_->glBindVertexArray(vao_);
        // load data into vertex buffers
        core_->glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        core_->glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        core_->glEnableVertexAttribArray(0);
        core_->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        core_->glEnableVertexAttribArray(1);
        core_->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        core_->glEnableVertexAttribArray(2);
        core_->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
        // vertex tangent
        core_->glEnableVertexAttribArray(3);
        core_->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
        // vertex bitangent
        core_->glEnableVertexAttribArray(4);
        core_->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        core_->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        core_->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

        core_->glBindVertexArray(0);
    }

public:
    // mesh Data
    vector<Vertex>       vertices_;
    vector<unsigned int> indices_;
    vector<Texture>      textures_;
    unsigned int vao_;
private:
    unsigned int vbo_, ebo_;
    QOpenGLFunctions_3_3_Core *core_ = NULL;

};

#endif // MESH_H
