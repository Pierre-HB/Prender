#pragma once

#include <glad/glad.h>
#include <string>
#include "src/algebra/Algebra.h"


enum ShaderType { NO_SHADER, DEFAULT_P_N_UV };

struct usedShader
{
    bool vertexShader;
    bool geometryShader;
    bool fragmentShader;
    bool computeShader;

    usedShader(bool useVertexShader, bool useGeometryShader, bool useFragmentShader, bool useComputeShader) : vertexShader(useVertexShader), geometryShader(useGeometryShader), fragmentShader(useFragmentShader), computeShader(useComputeShader) {}

};
usedShader get_used_shader(ShaderType shader);


const char* get_vertex_shader(ShaderType shader);

const char* get_geometry_shader(ShaderType shader);

const char* get_fragment_shader(ShaderType shader);

const char* get_compute_shader(ShaderType shader);


class Shader
{
private:

    GLuint program;

    static ShaderType loadedShader;

    ShaderType shaderType;

    std::string readFile(const char* file) const;

    GLuint compileShader(const char* file, GLenum shader);

public:

    //! constructor from a vertex and fragment shader
    Shader(ShaderType shaderType);

    //! destructor
    ~Shader();

    //! use the shader
    void use() const;

    //! get the shader type
    ShaderType getShaderType() const;

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