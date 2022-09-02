/*****************************************************************//**
 * @file   Shader_Data.cpp
 * @brief  Holds the definitions of the functions declared in @include 
 *		   "headers/Shader_Data.h" for the @Shader_Data class.
 * 
 * Headers:
 * @include "headers/Shader_Data.h"
 * 
 * @author Gustav Fagerlind
 * @date   05/07/2022
 *********************************************************************/

// Standard
#include <cstring>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader_Data.hpp"

void tilia::render::Shader_Data::Uniform(const std::string& location, const float& v0)
{
	float* new_data{ new float{ v0 } };
	uniform_variables[location].type = Uniform_Type::Float;
	uniform_variables[location].amount = 1;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const float& v0, const float& v1)
{
	float* new_data{ new float[2]{ v0, v1 } };
	uniform_variables[location].type = Uniform_Type::Float;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const float& v0, const float& v1, const float& v2)
{
	float* new_data{ new float[3]{ v0, v1, v2 } };
	uniform_variables[location].type = Uniform_Type::Float;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const float& v0, const float& v1, const float& v2, const float& v3)
{
	float* new_data{ new float[4]{ v0, v1, v2, v3 } };
	uniform_variables[location].type = Uniform_Type::Float;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const int32_t& v0)
{
	int32_t* new_data{ new int32_t{ v0 } };
	uniform_variables[location].type = Uniform_Type::Int;
	uniform_variables[location].amount = 1;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1)
{
	int32_t* new_data{ new int32_t[2]{ v0, v1 } };
	uniform_variables[location].type = Uniform_Type::Int;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1, const int32_t& v2)
{
	int32_t* new_data{ new int32_t[3]{ v0, v1, v2 } };
	uniform_variables[location].type = Uniform_Type::Int;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1, const int32_t& v2, const int32_t& v3)
{
	int32_t* new_data{ new int32_t[4]{ v0, v1, v2, v3 } };
	uniform_variables[location].type = Uniform_Type::Int;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const uint32_t& v0)
{
	uint32_t* new_data{ new uint32_t{ v0 } };
	uniform_variables[location].type = Uniform_Type::Uint;
	uniform_variables[location].amount = 1;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1)
{
	uint32_t* new_data{ new uint32_t[2]{ v0, v1 } };
	uniform_variables[location].type = Uniform_Type::Uint;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1, const uint32_t& v2)
{
	uint32_t* new_data{ new uint32_t[3]{ v0, v1, v2 } };
	uniform_variables[location].type = Uniform_Type::Uint;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1, const uint32_t& v2, const uint32_t& v3)
{
	uint32_t* new_data{ new uint32_t[4]{ v0, v1, v2, v3 } };
	uniform_variables[location].type = Uniform_Type::Uint;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::vec2& v0)
{
	glm::vec2* new_data{ new glm::vec2{ v0 } };
	uniform_variables[location].type = Uniform_Type::Vec;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::vec3& v0)
{
	glm::vec3* new_data{ new glm::vec3{ v0 } };
	uniform_variables[location].type = Uniform_Type::Vec;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::vec4& v0)
{
	glm::vec4* new_data{ new glm::vec4{ v0 } };
	uniform_variables[location].type = Uniform_Type::Vec;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::ivec2& v0)
{
	glm::ivec2* new_data{ new glm::ivec2{ v0 } };
	uniform_variables[location].type = Uniform_Type::Ivec;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::ivec3& v0)
{
	glm::ivec3* new_data{ new glm::ivec3{ v0 } };
	uniform_variables[location].type = Uniform_Type::Ivec;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::ivec4& v0)
{
	glm::ivec4* new_data{ new glm::ivec4{ v0 } };
	uniform_variables[location].type = Uniform_Type::Ivec;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::uvec2& v0)
{
	glm::uvec2* new_data{ new glm::uvec2{ v0 } };
	uniform_variables[location].type = Uniform_Type::Uvec;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::uvec3& v0)
{
	glm::uvec3* new_data{ new glm::uvec3{ v0 } };
	uniform_variables[location].type = Uniform_Type::Uvec;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::uvec4& v0)
{
	glm::uvec4* new_data{ new glm::uvec4{ v0 } };
	uniform_variables[location].type = Uniform_Type::Uvec;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const size_t& count, const float* v0)
{
	float* new_data{ new float[count] {} };
	memcpy_s(new_data, count * sizeof(float), v0, count * sizeof(float));
	uniform_variables[location].type = Uniform_Type::Float_Arr;
	uniform_variables[location].amount = count;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const size_t& count, const int32_t* v0)
{
	int32_t* new_data{ new int32_t[count] {} };
	memcpy_s(new_data, count * sizeof(int32_t), v0, count * sizeof(int32_t));
	uniform_variables[location].type = Uniform_Type::Float_Arr;
	uniform_variables[location].amount = count;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const size_t& count, const uint32_t* v0)
{
	uint32_t* new_data{ new uint32_t[count] {} };
	memcpy_s(new_data, count * sizeof(uint32_t), v0, count * sizeof(uint32_t));
	uniform_variables[location].type = Uniform_Type::Float_Arr;
	uniform_variables[location].amount = count;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::mat2& v0)
{
	glm::mat2* new_data{ new glm::mat2{ v0 } };
	uniform_variables[location].type = Uniform_Type::Mat;
	uniform_variables[location].amount = 2;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::mat3& v0)
{
	glm::mat3* new_data{ new glm::mat3{ v0 } };
	uniform_variables[location].type = Uniform_Type::Mat;
	uniform_variables[location].amount = 3;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Uniform(const std::string& location, const glm::mat4& v0)
{
	glm::mat4* new_data{ new glm::mat4{ v0 } };
	uniform_variables[location].type = Uniform_Type::Mat;
	uniform_variables[location].amount = 4;
	uniform_variables[location].data = static_cast<void*>(new_data);
}

void tilia::render::Shader_Data::Remove_Uniform(const std::string& location)
{
	if (uniform_variables.find(location) != uniform_variables.end()) {
		Delete_Uniform(location);
	}

	uniform_variables.erase(location);
}

tilia::render::Shader_Data::~Shader_Data()
{

	for (auto it{ uniform_variables.begin() }; it != uniform_variables.end(); ++it)
	{
		Delete_Uniform(it->first);
	}

}

void tilia::render::Shader_Data::Delete_Uniform(const std::string& location)
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

bool tilia::render::Shader_Data::Uniform_Variable::operator==(const Uniform_Variable& other) const
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
