#include "Shader.h"
#include <fstream>
#include <sstream>
#include "main.h"


//! initialise the used shader
ShaderType Shader::loadedShader = ShaderType::NO_SHADER;

usedShader get_used_shader(ShaderType shader) {
	switch (shader)
	{
	case NO_SHADER:
		return usedShader(false, false, false, false);
	case DEFAULT_P_N_UV:
		return usedShader(true, false, true, false);
	case DEBUG_TBN:
		return usedShader(true, true, true, false);
	default:
#ifdef CONSOLE
		std::cout << "[WARNING] used shader for shader type " << shader << " is undefind" << std::endl;
#endif
		return usedShader(true, true, true, true);
	}
}

const char* get_vertex_shader(ShaderType shader) {
	switch (shader)
	{
	case NO_SHADER:
		return "";
	case DEFAULT_P_N_UV:
		return "src/shaders/vertexUVShader.glsl";
	case DEBUG_TBN:
		return "src/shaders/vertexTBNShader.glsl";
	default:
#ifdef CONSOLE
		std::cout << "[WARNING] vertex shader for shader type " << shader << " is undefind" << std::endl;
#endif
		return "";
	}
}

const char* get_geometry_shader(ShaderType shader) {
	switch (shader)
	{
	case NO_SHADER:
		return "";
	case DEFAULT_P_N_UV:
		return "";
	case DEBUG_TBN:
		return "src/shaders/geometryTBNShader.glsl";
	default:
#ifdef CONSOLE
			std::cout << "[WARNING] geometry shader for shader type " << shader << " is undefind" << std::endl;
#endif
			return "";
	}
}

const char* get_fragment_shader(ShaderType shader) {
	switch (shader)
	{
	case NO_SHADER:
		return "";
	case DEFAULT_P_N_UV:
		return "src/shaders/fragmentUVShader.glsl";
	case DEBUG_TBN:
		return "src/shaders/fragmentTBNShader.glsl";
	default:
#ifdef CONSOLE
		std::cout << "[WARNING] fragment shader for shader type " << shader << " is undefind" << std::endl;
#endif
		return "";
	}
}

const char* get_compute_shader(ShaderType shader) {
	switch (shader)
	{
	case NO_SHADER:
		return "";
	case DEFAULT_P_N_UV:
		return "";
	case DEBUG_TBN:
		return "";
	default:
#ifdef CONSOLE
		std::cout << "[WARNING] compute shader for shader type " << shader << " is undefind" << std::endl;
#endif
		return "";
	}
}



std::string Shader::readFile(const char* file) const {
	std::ifstream inputStream;
	std::stringstream stream;
	inputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		inputStream.open(file);
		stream << inputStream.rdbuf();
		inputStream.close();
	}
	catch (std::ifstream::failure e) {
#ifdef CONSOLE
		std::cout << "[ERROR] shader could not read file '" << file << "'" << std::endl;
#endif
	}
	return stream.str();
}

GLuint Shader::compileShader(const char* file, GLenum shader) {
	GLuint shaderID;
	std::string shaderString = readFile(file);
	const GLchar* shaderCode = shaderString.c_str();

	shaderID = glCreateShader(shader);
#ifdef DEBUG
	debug::NB_OPENGL_PTR++;
#endif
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	int success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
#ifdef CONSOLE
	char infoLog[512];
	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "[ERROR] at compilation of shader : " << file << std::endl << infoLog << std::endl;
	}
#endif
	return shaderID;
}
Shader::Shader(ShaderType shaderType) : shaderType(shaderType)
{
	GLuint vertexShaderID;
	GLuint geometryShaderID;
	GLuint fragmentShaderID;
	GLuint computeShaderID;

	usedShader used_shader = get_used_shader(shaderType);
	program = glCreateProgram();

	if (used_shader.vertexShader) {
		vertexShaderID = compileShader(get_vertex_shader(shaderType), GL_VERTEX_SHADER);
		glAttachShader(program, vertexShaderID);
	}

	if (used_shader.geometryShader) {
		geometryShaderID = compileShader(get_geometry_shader(shaderType), GL_GEOMETRY_SHADER);
		glAttachShader(program, geometryShaderID);
	}

	if (used_shader.fragmentShader) {
		fragmentShaderID = compileShader(get_fragment_shader(shaderType), GL_FRAGMENT_SHADER);
		glAttachShader(program, fragmentShaderID);
	}

	if (used_shader.computeShader) {
		computeShaderID = compileShader(get_compute_shader(shaderType), GL_COMPUTE_SHADER);
		glAttachShader(program, computeShaderID);
	}
		
	glLinkProgram(program);
	int success;
	glGetShaderiv(program, GL_LINK_STATUS, &success);
#ifdef CONSOLE
	char infoLog[512];
	if (!success) {
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "[ERROR] Linking of shader" << std::endl << infoLog << std::endl;
	}
#endif

	if (used_shader.vertexShader) {
#ifdef DEBUG
		debug::NB_OPENGL_PTR--;
#endif
		glDeleteShader(vertexShaderID);
	}
		
	if (used_shader.geometryShader) {
#ifdef DEBUG
		debug::NB_OPENGL_PTR--;
#endif
		glDeleteShader(geometryShaderID);
	}
		if (used_shader.fragmentShader) {
#ifdef DEBUG
			debug::NB_OPENGL_PTR--;
#endif
			glDeleteShader(fragmentShaderID);
	}
		if (used_shader.computeShader) {
#ifdef DEBUG
			debug::NB_OPENGL_PTR--;
#endif
			glDeleteShader(computeShaderID);
	}
		
#ifdef DEBUG
	debug::NB_INSTANCES++;
	debug::NB_OPENGL_PTR++;
#endif
}

Shader::~Shader() {
	glDeleteProgram(program);
#ifdef DEBUG
	debug::NB_INSTANCES--;
	debug::NB_OPENGL_PTR--;
#endif
}

void Shader::use() const {
	//don't reload the shader if it is already loaded to save time
	if (Shader::loadedShader != shaderType) {
		Shader::loadedShader = shaderType;
		glUseProgram(program);
	}
	
}

ShaderType Shader::getShaderType() const {
	return shaderType;
}

GLuint Shader::getID() const {
	return program;
}

void Shader::setUniform(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setUniform(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, int* value, int count) const {
	glUniform1iv(glGetUniformLocation(program, name.c_str()), count, value);
}

void Shader::setUniform(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float* value, int count) const {
	glUniform1fv(glGetUniformLocation(program, name.c_str()), count, value);
}

void Shader::setUniform(const std::string& name, mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, true, value.c);
}

void Shader::setUniform(const std::string& name, mat3 value) const {
	glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, false, value.c);
}

void Shader::setUniform(const std::string& name, mat2 value) const {
	glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, false, value.c);
}

void Shader::setUniform(const std::string& name, vec2 value) const {
	glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &(value.x));
}

void Shader::setUniform(const std::string& name, vec3 value) const {
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &(value.x));
}

void Shader::setUniform(const std::string& name, vec4 value) const {
	glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &(value.x));
}

void Shader::setUniform(const std::string& name, ivec2 value) const {
	glUniform2iv(glGetUniformLocation(program, name.c_str()), 1, &(value.x));
}

void Shader::setUniform(const std::string& name, ivec3 value) const {
	glUniform3iv(glGetUniformLocation(program, name.c_str()), 1, &(value.x));
}

void Shader::setUniform(const std::string& name, ivec4 value) const {
	glUniform4iv(glGetUniformLocation(program, name.c_str()), 1, &(value.x));
}
