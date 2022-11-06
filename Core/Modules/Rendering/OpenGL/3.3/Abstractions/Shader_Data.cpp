#if 0
// Vendor
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <cstring>
#include <memory>
#include <iostream>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader_Data.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"
#include "Core/Modules/File_System/Windows/File_System.hpp"

#if 0

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const float& v0)
{
	float* new_data{ new float{ v0 } };
	uniform_variables[location].type = Uniform_Type::Float;
	uniform_variables[location].amount = 1;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const float& v0, const float& v1)
{
	float* new_data{ new float[2]{ v0, v1 } };
	uniform_variables[location].type = Uniform_Type::Float;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const float& v0, const float& v1, const float& v2)
{
	float* new_data{ new float[3]{ v0, v1, v2 } };
	uniform_variables[location].type = Uniform_Type::Float;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const float& v0, const float& v1, const float& v2, const float& v3)
{
	float* new_data{ new float[4]{ v0, v1, v2, v3 } };
	uniform_variables[location].type = Uniform_Type::Float;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const int32_t& v0)
{
	int32_t* new_data{ new int32_t{ v0 } };
	uniform_variables[location].type = Uniform_Type::Int;
	uniform_variables[location].amount = 1;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1)
{
	int32_t* new_data{ new int32_t[2]{ v0, v1 } };
	uniform_variables[location].type = Uniform_Type::Int;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1, const int32_t& v2)
{
	int32_t* new_data{ new int32_t[3]{ v0, v1, v2 } };
	uniform_variables[location].type = Uniform_Type::Int;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1, const int32_t& v2, const int32_t& v3)
{
	int32_t* new_data{ new int32_t[4]{ v0, v1, v2, v3 } };
	uniform_variables[location].type = Uniform_Type::Int;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const uint32_t& v0)
{
	uint32_t* new_data{ new uint32_t{ v0 } };
	uniform_variables[location].type = Uniform_Type::Uint;
	uniform_variables[location].amount = 1;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1)
{
	uint32_t* new_data{ new uint32_t[2]{ v0, v1 } };
	uniform_variables[location].type = Uniform_Type::Uint;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1, const uint32_t& v2)
{
	uint32_t* new_data{ new uint32_t[3]{ v0, v1, v2 } };
	uniform_variables[location].type = Uniform_Type::Uint;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1, const uint32_t& v2, const uint32_t& v3)
{
	uint32_t* new_data{ new uint32_t[4]{ v0, v1, v2, v3 } };
	uniform_variables[location].type = Uniform_Type::Uint;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::vec2& v0)
{
	glm::vec2* new_data{ new glm::vec2{ v0 } };
	uniform_variables[location].type = Uniform_Type::Vec;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::vec3& v0)
{
	glm::vec3* new_data{ new glm::vec3{ v0 } };
	uniform_variables[location].type = Uniform_Type::Vec;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::vec4& v0)
{
	glm::vec4* new_data{ new glm::vec4{ v0 } };
	uniform_variables[location].type = Uniform_Type::Vec;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::ivec2& v0)
{
	glm::ivec2* new_data{ new glm::ivec2{ v0 } };
	uniform_variables[location].type = Uniform_Type::Ivec;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::ivec3& v0)
{
	glm::ivec3* new_data{ new glm::ivec3{ v0 } };
	uniform_variables[location].type = Uniform_Type::Ivec;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::ivec4& v0)
{
	glm::ivec4* new_data{ new glm::ivec4{ v0 } };
	uniform_variables[location].type = Uniform_Type::Ivec;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::uvec2& v0)
{
	glm::uvec2* new_data{ new glm::uvec2{ v0 } };
	uniform_variables[location].type = Uniform_Type::Uvec;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::uvec3& v0)
{
	glm::uvec3* new_data{ new glm::uvec3{ v0 } };
	uniform_variables[location].type = Uniform_Type::Uvec;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::uvec4& v0)
{
	glm::uvec4* new_data{ new glm::uvec4{ v0 } };
	uniform_variables[location].type = Uniform_Type::Uvec;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const size_t& count, const float* v0)
{
	float* new_data{ new float[count] {} };
	memcpy_s(new_data, count * sizeof(float), v0, count * sizeof(float));
	uniform_variables[location].type = Uniform_Type::Float_Arr;
	uniform_variables[location].amount = count;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const size_t& count, const int32_t* v0)
{
	int32_t* new_data{ new int32_t[count] {} };
	memcpy_s(new_data, count * sizeof(int32_t), v0, count * sizeof(int32_t));
	uniform_variables[location].type = Uniform_Type::Float_Arr;
	uniform_variables[location].amount = count;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const size_t& count, const uint32_t* v0)
{
	uint32_t* new_data{ new uint32_t[count] {} };
	memcpy_s(new_data, count * sizeof(uint32_t), v0, count * sizeof(uint32_t));
	uniform_variables[location].type = Uniform_Type::Float_Arr;
	uniform_variables[location].amount = count;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::mat2& v0)
{
	glm::mat2* new_data{ new glm::mat2{ v0 } };
	uniform_variables[location].type = Uniform_Type::Mat;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::mat3& v0)
{
	glm::mat3* new_data{ new glm::mat3{ v0 } };
	uniform_variables[location].type = Uniform_Type::Mat;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Uniform(const std::string& location, const glm::mat4& v0)
{
	glm::mat4* new_data{ new glm::mat4{ v0 } };
	uniform_variables[location].type = Uniform_Type::Mat;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::gfx::Shader_Data::Remove_Uniform(const std::string& location)
{
	if (uniform_variables.find(location) != uniform_variables.end()) {
		Delete_Uniform(location);
	}

	uniform_variables.erase(location);
}

tilia::gfx::Shader_Data::~Shader_Data()
{

	for (auto it{ uniform_variables.begin() }; it != uniform_variables.end(); ++it)
	{
		Delete_Uniform(it->first);
	}

}

void tilia::gfx::Shader_Data::Delete_Uniform(const std::string& location)
{

	switch (uniform_variables[location].type)
	{

	case Uniform_Type::Float:
		if (uniform_variables[location].amount > 1) {
			delete[] static_cast<float*>(uniform_variables[location].data);
		}
		else
		{
			delete static_cast<float*>(uniform_variables[location].data);
		}
		break;

	case Uniform_Type::Int:
		if (uniform_variables[location].amount > 1) {
			delete[] static_cast<int32_t*>(uniform_variables[location].data);
		}
		else
		{
			delete static_cast<int32_t*>(uniform_variables[location].data);
		}
		break;

	case Uniform_Type::Uint:
		if (uniform_variables[location].amount > 1) {
			delete[] static_cast<uint32_t*>(uniform_variables[location].data);
		}
		else
		{
			delete static_cast<uint32_t*>(uniform_variables[location].data);
		}
		break;

	case Uniform_Type::Vec:
		switch (uniform_variables[location].amount)
		{
		case 2:
			delete static_cast<glm::vec2*>(uniform_variables[location].data);
			break;
		case 3:
			delete static_cast<glm::vec3*>(uniform_variables[location].data);
			break;
		case 4:
			delete static_cast<glm::vec4*>(uniform_variables[location].data);
			break;
		}
		break;

	case Uniform_Type::Ivec:
		switch (uniform_variables[location].amount)
		{
		case 2:
			delete static_cast<glm::ivec2*>(uniform_variables[location].data);
			break;
		case 3:
			delete static_cast<glm::ivec3*>(uniform_variables[location].data);
			break;
		case 4:
			delete static_cast<glm::ivec4*>(uniform_variables[location].data);
			break;
		}
		break;

	case Uniform_Type::Uvec:
		switch (uniform_variables[location].amount)
		{
		case 2:
			delete static_cast<glm::uvec2*>(uniform_variables[location].data);
			break;
		case 3:
			delete static_cast<glm::uvec3*>(uniform_variables[location].data);
			break;
		case 4:
			delete static_cast<glm::uvec4*>(uniform_variables[location].data);
			break;
		}
		break;

	case Uniform_Type::Float_Arr:
		delete[] static_cast<float*>(uniform_variables[location].data);
		break;

	case Uniform_Type::Int_Arr:
		delete[] static_cast<int32_t*>(uniform_variables[location].data);
		break;

	case Uniform_Type::Uint_Arr:
		delete[] static_cast<uint32_t*>(uniform_variables[location].data);
		break;

	case Uniform_Type::Mat:
		switch (uniform_variables[location].amount)
		{
		case 2:
			delete static_cast<glm::mat2*>(uniform_variables[location].data);
			break;
		case 3:
			delete static_cast<glm::mat3*>(uniform_variables[location].data);
			break;
		case 4:
			delete static_cast<glm::mat4*>(uniform_variables[location].data);
			break;
		}
		break;

	}

}

bool tilia::gfx::Shader_Data::Uniform_Variable::operator==(const Uniform_Variable& other) const
{
	
	if (this->type != other.type)
		return false;
	if (this->amount != other.amount)
		return false;

	switch (other.type)
	{

	case Uniform_Type::Float:
		if (memcmp(static_cast<float*>(this->data), static_cast<float*>(other.data), other.amount * sizeof(float)))
			return false;
		break;

	case Uniform_Type::Int:
		if (memcmp(static_cast<int32_t*>(this->data), static_cast<int32_t*>(other.data), other.amount * sizeof(int32_t)))
			return false;
		break;

	case Uniform_Type::Uint:
		if (memcmp(static_cast<uint32_t*>(this->data), static_cast<uint32_t*>(other.data), other.amount * sizeof(uint32_t)))
			return false;
		break;

	case Uniform_Type::Vec:
		if (memcmp(static_cast<float*>(this->data), static_cast<float*>(other.data), other.amount * sizeof(float)))
			return false;
		break;

	case Uniform_Type::Ivec:
		if (memcmp(static_cast<int32_t*>(this->data), static_cast<int32_t*>(other.data), other.amount * sizeof(int32_t)))
			return false;
		break;

	case Uniform_Type::Uvec:
		if (memcmp(static_cast<uint32_t*>(this->data), static_cast<uint32_t*>(other.data), other.amount * sizeof(uint32_t)))
			return false;
		break;

	case Uniform_Type::Float_Arr:
		if (memcmp(static_cast<float*>(this->data), static_cast<float*>(other.data), other.amount * sizeof(float)))
			return false;
		break;

	case Uniform_Type::Int_Arr:
		if (memcmp(static_cast<int32_t*>(this->data), static_cast<int32_t*>(other.data), other.amount * sizeof(int32_t)))
			return false;
		break;

	case Uniform_Type::Uint_Arr:
		if (memcmp(static_cast<uint32_t*>(this->data), static_cast<uint32_t*>(other.data), other.amount * sizeof(uint32_t)))
			return false;
		break;

	case Uniform_Type::Mat:
		if (memcmp(static_cast<float*>(this->data), static_cast<float*>(other.data), other.amount * other.amount * sizeof(float)))
			return false;
		break;

	}

	return true;

}

#endif // 0

#if 1

std::uint32_t tilia::gfx::Shader_Data::s_bound_ID{};
std::uint32_t tilia::gfx::Shader_Data::s_previous_ID{};

// The file system defined in another file
extern tilia::utils::File_System file_system;

tilia::gfx::Shader_Data::~Shader_Data() {
	Delete_Shader();
}

void tilia::gfx::Shader_Data::Bind() const {
    if (!m_ID)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to bind shader { ID: %v }"
		)(m_ID);

		throw e;

	}
    GL_CALL(glUseProgram(m_ID));
    s_bound_ID = m_ID;
}

void tilia::gfx::Shader_Data::Bind(const std::uint32_t& id) {
    if (!id)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to bind shader { ID: %v }"
		)(id);

		throw e;

	}
    GL_CALL(glUseProgram(id));
    s_bound_ID = id;
}

void tilia::gfx::Shader_Data::Unbind(const bool& save_id) {
    GL_CALL(glUseProgram(0));

    if (save_id)
    {
        s_previous_ID = s_bound_ID;
    }

	s_bound_ID = 0;
}

void tilia::gfx::Shader_Data::Rebind() {
	GL_CALL(glUseProgram(s_previous_ID));
	s_bound_ID = s_previous_ID;
	s_bound_ID = 0;
}

#define SET_UNIFORM(x) if (m_ID != s_bound_ID)\
					   { Unbind(true);\
                       Bind(); }\
                       x;\
					   if (m_ID == s_bound_ID && m_ID != s_previous_ID)\
                       { Rebind(); }

void tilia::gfx::Shader_Data::Uniform(const std::string& loc, std::initializer_list<float> vs)
{
	Uniform(loc, vs.begin(), vs.size());
}

void tilia::gfx::Shader_Data::Uniform(const std::string& loc, std::initializer_list<std::int32_t> vs)
{
	Uniform(loc, vs.begin(), vs.size());
}

void tilia::gfx::Shader_Data::Uniform(const std::string& loc, std::initializer_list<std::uint32_t> vs)
{
	Uniform(loc, vs.begin(), vs.size());
}

void tilia::gfx::Shader_Data::Uniform(const std::string& loc, const float* vs, const std::size_t& size)
{
	switch (size)
	{
	case 1:
		SET_UNIFORM(GL_CALL(glUniform1f(Get_Uniform_Location(loc), vs[0])));
		return;
	case 2:
		SET_UNIFORM(GL_CALL(glUniform2f(Get_Uniform_Location(loc), vs[0], vs[1])));
		return;
	case 3:
		SET_UNIFORM(GL_CALL(glUniform3f(Get_Uniform_Location(loc), vs[0], vs[1], vs[2])));
		return;
	case 4:
		SET_UNIFORM(GL_CALL(glUniform4f(Get_Uniform_Location(loc), vs[0], vs[1], vs[2], vs[3])));
		return;
	default:
		SET_UNIFORM(GL_CALL(glUniform1fv(Get_Uniform_Location(loc), static_cast<GLsizei>(size), vs)))		
		return;
	}
}

void tilia::gfx::Shader_Data::Uniform(const std::string& loc, const std::int32_t* vs, const std::size_t& size)
{
	switch (size)
	{
	case 1:
		SET_UNIFORM(GL_CALL(glUniform1i(Get_Uniform_Location(loc), vs[0])));
		return;
	case 2:
		SET_UNIFORM(GL_CALL(glUniform2i(Get_Uniform_Location(loc), vs[0], vs[1])));
		return;
	case 3:
		SET_UNIFORM(GL_CALL(glUniform3i(Get_Uniform_Location(loc), vs[0], vs[1], vs[2])));
		return;
	case 4:
		SET_UNIFORM(GL_CALL(glUniform4i(Get_Uniform_Location(loc), vs[0], vs[1], vs[2], vs[3])));
		return;
	default:
		SET_UNIFORM(GL_CALL(glUniform1iv(Get_Uniform_Location(loc), static_cast<GLsizei>(size), vs)))
			return;
	}
}

void tilia::gfx::Shader_Data::Uniform(const std::string& loc, const std::uint32_t* vs, const std::size_t& size)
{
	switch (size)
	{
	case 1:
		SET_UNIFORM(GL_CALL(glUniform1ui(Get_Uniform_Location(loc), vs[0])));
		return;
	case 2:
		SET_UNIFORM(GL_CALL(glUniform2ui(Get_Uniform_Location(loc), vs[0], vs[1])));
		return;
	case 3:
		SET_UNIFORM(GL_CALL(glUniform3ui(Get_Uniform_Location(loc), vs[0], vs[1], vs[2])));
		return;
	case 4:
		SET_UNIFORM(GL_CALL(glUniform4ui(Get_Uniform_Location(loc), vs[0], vs[1], vs[2], vs[3])));
		return;
	default:
		SET_UNIFORM(GL_CALL(glUniform1uiv(Get_Uniform_Location(loc), static_cast<GLsizei>(size), vs)))			return;
	}
}

std::int32_t tilia::gfx::Shader_Data::Get_Uniform_Location(const std::string& name)
{
	if (m_location_cache.find(name) != m_location_cache.end())
		return m_location_cache[name];

	GL_CALL(std::int32_t location = glGetUniformLocation(m_ID, name.c_str()));

	// Check is -1
	//if (location == -1)
	//	log::Log(log::Type::ERROR, "SHADER", "Uniform { Name: %s } does not exist / is not being used", name.c_str());

	m_location_cache[name] = location;

	return location;
}

std::uint32_t tilia::gfx::Shader_Data::Make_Shader(const tilia::enums::Shader_Type& type)
{
	
	std::uint32_t id{};

	GL_CALL(id = glCreateShader(*type));
	
	std::string src{};

	switch (type)
	{
	case enums::Shader_Type::Vertex:
		src = m_parts[0].source;
		break;
	case enums::Shader_Type::Fragment:
		src = m_parts[1].source;
		break;
	case enums::Shader_Type::Geomentry:
		src = m_parts[2].source;
		break;
	}

	const char* c_src{ src.c_str() };

	GL_CALL(glShaderSource(id, 1, &c_src, nullptr));

	GL_CALL(glCompileShader(id));

	std::int32_t result;

	GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE) {
		std::int32_t length;

		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		std::vector<char> message(static_cast<size_t>(length));
		
		GL_CALL(glGetShaderInfoLog(id, length, &length, &message.front()));
		message[static_cast<size_t>(length) - 1] = '\0';

		GL_CALL(glDeleteShader(id));

		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Shader part { ID: %v } failed to be created"
			"\n>>> Part type: %v"
			"\n>>> Message: %v"
		)(id)
		(utils::Get_Shader_Type_String(type))
		(&message.front());

		throw e;

	}

	return id;

}

void tilia::gfx::Shader_Data::Generate_Shader() noexcept
{
	try
	{

		GL_CALL(m_ID = glCreateProgram());

	}
	catch (utils::Tilia_Exception& e)
	{

		e.Add_Message("Shader { ID: %v } failed to be generated")
			(m_ID);

		// Possibly forward e to someplace else and then throw

	}
}

void tilia::gfx::Shader_Data::Delete_Shader() noexcept
{



}

void tilia::gfx::Shader_Data::Reload(const std::size_t& index)
{

	std::uint32_t shader_id{};

	switch (index)
	{
	case 0: // Vertex

		m_parts[0].source = file_system.Load_File(m_parts[0].path);

		shader_id = Make_Shader(enums::Shader_Type::Vertex);

		break;
	case 1: // Fragment

		m_parts[1].source = file_system.Load_File(m_parts[1].path);

		shader_id = Make_Shader(enums::Shader_Type::Fragment);

		break;
	case 2: // Geometry

		m_parts[2].source = file_system.Load_File(m_parts[2].path);

		shader_id = Make_Shader(enums::Shader_Type::Geomentry);

		break;
	default:

		Reload(0);
				
		Reload(1);

		if (m_use_geometry)
			Reload(2);	

		return;
	}

	try
	{

		GL_CALL(glAttachShader(m_ID, shader_id));
		
		GL_CALL(glLinkProgram(m_ID));
		GL_CALL(glValidateProgram(m_ID));
	
	}
	catch (utils::Tilia_Exception& e)
	{

		GL_CALL(glDeleteShader(shader_id));

		e.Add_Message("Shader { %v } failed to attach part"
			"\n>>> Part ID: %v"
		)(m_ID)(shader_id);

		throw e;

	}

	GL_CALL(glDeleteShader(shader_id));

	std::int32_t result;

	GL_CALL(glGetProgramiv(m_ID, GL_LINK_STATUS, &result));

	if (result == GL_FALSE) {
		std::int32_t length;

		GL_CALL(glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &length));

		std::vector<char> message(static_cast<size_t>(length));

		GL_CALL(glGetProgramInfoLog(m_ID, length, &length, &message.front()));
		message[static_cast<size_t>(length) - 1] = '\0';

		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Shader { ID: %v } failed to reload shader part { ID: %v }"
			"\n>>> Part type: %v"
			"\n>>> Message: %v"
		)(m_ID)(shader_id)
		(utils::Get_Shader_Type_String(utils::Get_Index_Shader_Type(index)))
		(&message.front());

		throw e;

	}

}

#endif
#endif