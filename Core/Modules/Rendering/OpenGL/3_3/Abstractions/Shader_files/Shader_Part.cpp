// Vendor
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <vector>

// Tilia
#include "Shader_Part.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_OPENGL_3_3_ERROR_HANDLING_INCLUDE
#include TILIA_TILIA_EXCEPTION_INCLUDE
#include TILIA_OPENGL_3_3_UTILS_INCLUDE
#include TILIA_WINDOWS_FILE_SYSTEM_INCLUDE
#include TILIA_OPENGL_3_3_SHADER_INCLUDE

// The file system defined in another file
extern tilia::utils::File_System file_system;

tilia::gfx::Shader_Part::~Shader_Part()
{

	try
	{
		GL_CALL(glDeleteShader(m_ID));
	}
	catch (utils::Tilia_Exception& t_e)
	{

		t_e.Add_Message(TILIA_LOCATION) 
			<< "Shader part { ID: " << m_ID << " } failed to be destroyed";

		// Possibly forward e to someplace else and then throw

	}

}

void tilia::gfx::Shader_Part::Init(const bool& reload)
{

	try {
		
		GL_CALL(m_ID = glCreateShader(*m_type));

		if (reload)
			Compile(true);

	}
	catch(utils::Tilia_Exception& t_e) {
		t_e.Add_Message(TILIA_LOCATION) 
			<< "Failed to init Shader_Part { ID: " << m_ID << " }";
		throw t_e;
	}

}

void tilia::gfx::Shader_Part::Source()
{

	try {
		m_source = file_system.Load_File(m_path);
	}
	catch(utils::Tilia_Exception& t_e)
	{
		t_e.Add_Message(TILIA_LOCATION) 
			<< "Failed to load shader source for Shader_Part { ID: " << m_ID << " }";
		throw t_e;
	}

}

void tilia::gfx::Shader_Part::Compile(const bool& source)
{

	try {

		if (source)
			Source();

		const char* src{ m_source.c_str() };

		GL_CALL(glShaderSource(m_ID, 1, &src, nullptr));

		GL_CALL(glCompileShader(m_ID));

		std::int32_t result;

		GL_CALL(glGetShaderiv(m_ID, GL_COMPILE_STATUS, &result));

		if (result == GL_FALSE) {
			std::int32_t length;

			GL_CALL(glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &length));

			std::vector<char> message(static_cast<size_t>(length));

			GL_CALL(glGetShaderInfoLog(m_ID, length, &length, &message.front()));
			message[static_cast<size_t>(length) - 1] = '\0';

			GL_CALL(glDeleteShader(m_ID));

			throw utils::Tilia_Exception{ utils::Exception_Data{ TILIA_LOCATION } 
				<< "Shader part { ID: " << m_ID << " } failed to be created"
				<< "\n>>> Part type: " << utils::Get_Shader_Type_String(m_type)
				<< "\n>>> Message: " << &message.front() };

		}
	
		for (auto& shader : m_attached_to)
		{
			if (shader != nullptr)
				shader->Reload();
		}

	}
	catch(utils::Tilia_Exception& t_e) {
		t_e.Add_Message(TILIA_LOCATION) 
			<< "Failed to compile Shader_Part { ID: " << m_ID << " }";
		throw t_e;
	}

}
