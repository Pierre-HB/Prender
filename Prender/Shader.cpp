#include "Shader.h"



#include <fstream>
#include <sstream>
#include <iostream>

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
		std::cout << "[ERROR] shader could not read file '" << file << "'" << std::endl;
	}
	return stream.str();
}

Shader::Shader(const char* vertexShader, const char* fragmentShader) 
{
	//Vertex shader
	std::string vertexString = readFile(vertexShader);
	const GLchar* vertexShaderCode = vertexString.c_str();

	GLuint vertexShaderID;
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShaderID);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "[ERROR] at compilation of vertexShader" << std::endl << infoLog << std::endl;
	}

	//Fragment shader
	std::string fragmentString = readFile(fragmentShader);
	const GLchar* fragmentShaderCode = fragmentString.c_str();

	GLuint fragmentShaderID;
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << "[ERROR] at compilation of fragmentShader" << std::endl << infoLog << std::endl;
	}

	//Linking
	program = glCreateProgram();

	glAttachShader(program, vertexShaderID);
	glAttachShader(program, fragmentShaderID);
	glLinkProgram(program);

	glGetShaderiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "[ERROR] Linking of shader" << std::endl << infoLog << std::endl;
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::use() {
	glUseProgram(program);
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
