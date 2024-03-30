#pragma once

#include <glad/glad.h>
#include <string>
#include "src/algebra/Algebra.h"

class Shader
{
private:
    GLuint program;

    std::string readFile(const char* file) const;
public:

    //! constructor from a vertex and fragment shader
    Shader(const char* vertexShader, const char* fragmentShader);

    //! destructor
    ~Shader();

    //! use the shader
    void use();

    //! uniform loading
    //! Load a bool
    void setUniform(const std::string& name, bool value) const;
    //! Load an int
    void setUniform(const std::string& name, int value) const;
    //! Load an array of int
    void setUniform(const std::string& name, int* value, int count) const;
    //! Load a float
    void setUniform(const std::string& name, float value) const;
    //! Load an array of float
    void setUniform(const std::string& name, float* value, int count) const;
    //! Load a mat2
    void setUniform(const std::string& name, mat2 value) const;
    //! Load a mat3
    void setUniform(const std::string& name, mat3 value) const;
    //! Load a mat4
    void setUniform(const std::string& name, mat4 value) const;
    //! Load a vec2
    void setUniform(const std::string& name, vec2 value) const;
    //! Load a vec3
    void setUniform(const std::string& name, vec3 value) const;
    //! Load a vec4
    void setUniform(const std::string& name, vec4 value) const;
    //! Load an ivec2
    void setUniform(const std::string& name, ivec2 value) const;
    //! Load an ivec3
    void setUniform(const std::string& name, ivec3 value) const;
    //! Load an ivec4
    void setUniform(const std::string& name, ivec4 value) const;
};