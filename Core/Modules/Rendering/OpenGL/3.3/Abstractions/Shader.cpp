// Vendor
#include "vendor/glad/include/glad/glad.h"

// Standard

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"

std::uint32_t tilia::gfx::Shader::s_bound_ID{};
std::uint32_t tilia::gfx::Shader::s_previous_ID{};

tilia::gfx::Shader::~Shader()
{
    
	try
	{
		GL_CALL(glDeleteProgram(m_ID));
	}
	catch (utils::Tilia_Exception& e)
	{

		e.Add_Message("Shader { ID: %v } failed to be destroyed")
			(m_ID);

		// Possibly forward e to someplace else and then throw

	}

}

void tilia::gfx::Shader::Init(std::initializer_list<std::weak_ptr<Shader_Part>> vertex_parts, std::initializer_list<std::weak_ptr<Shader_Part>> fragment_parts, std::initializer_list<std::weak_ptr<Shader_Part>> geometry_parts)
{

    GL_CALL(m_ID = glCreateProgram());

    if (!vertex_parts.size() || !fragment_parts.size()) {

        utils::Tilia_Exception e{ LOCATION };

        e.Add_Message("Shader { ID: %v } was not given enough vertex or fragment parts");

        throw e;

    }

    for (auto& v_part : vertex_parts) {
        Add_Part(v_part, false);
    }
    for (auto& f_part : fragment_parts) {
        Add_Part(f_part, false);
    }
    for (auto& g_part : geometry_parts) {
        Add_Part(g_part, false);
    }

    Reload();

}

void tilia::gfx::Shader::Add_Part(std::weak_ptr<Shader_Part> shader_part, const bool& reload)
{

	GL_CALL(glAttachShader(m_ID, shader_part.lock()->Get_ID()));

    if (reload)
        Reload();

}

void tilia::gfx::Shader::Remove_Part(std::weak_ptr<Shader_Part> shader_part, const bool& reload)
{

	GL_CALL(glDetachShader(m_ID, shader_part.lock()->Get_ID()));

    if (reload)
        Reload();

}

void tilia::gfx::Shader::Reload()
{

    GL_CALL(glLinkProgram(m_ID));
	GL_CALL(glValidateProgram(m_ID));

    std::int32_t result;

	GL_CALL(glGetProgramiv(m_ID, GL_LINK_STATUS, &result));

	if (result == GL_FALSE) {
		std::int32_t length;

		GL_CALL(glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &length));

		std::vector<char> message(static_cast<size_t>(length));

		GL_CALL(glGetProgramInfoLog(m_ID, length, &length, &message.front()));
		message[static_cast<size_t>(length) - 1] = '\0';

		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Shader { ID: %v } failed to reload"
			"\n>>> Message: %v"
		)(m_ID)
		(&message.front());

		throw e;

	}

}

void tilia::gfx::Shader::Bind() const {
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

void tilia::gfx::Shader::Bind(const std::uint32_t& id) {
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

void tilia::gfx::Shader::Unbind(const bool& save_id) {
    GL_CALL(glUseProgram(0));

    if (save_id)
    {
        s_previous_ID = s_bound_ID;
    }

	s_bound_ID = 0;
}

void tilia::gfx::Shader::Rebind() {
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

void tilia::gfx::Shader::Uniform(const std::string& loc, std::initializer_list<float> vs)
{
	Uniform(loc, vs.begin(), vs.size());
}

void tilia::gfx::Shader::Uniform(const std::string& loc, std::initializer_list<std::int32_t> vs)
{
	Uniform(loc, vs.begin(), vs.size());
}

void tilia::gfx::Shader::Uniform(const std::string& loc, std::initializer_list<std::uint32_t> vs)
{
	Uniform(loc, vs.begin(), vs.size());
}

void tilia::gfx::Shader::Uniform(const std::string& loc, const float* vs, const std::size_t& size)
{
	try {
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
	catch (utils::Tilia_Exception& e) {
		e.Add_Message("Failed to set uniform for shader { ID: %v }")(m_ID);

		throw e;
	}
}

void tilia::gfx::Shader::Uniform(const std::string& loc, const std::int32_t* vs, const std::size_t& size)
{
	try {
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
	catch (utils::Tilia_Exception& e) {
		e.Add_Message("Failed to set uniform for shader { ID: %v }")(m_ID);

		throw e;
	}
}

void tilia::gfx::Shader::Uniform(const std::string& loc, const std::uint32_t* vs, const std::size_t& size)
{
	try {
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
			SET_UNIFORM(GL_CALL(glUniform1uiv(Get_Uniform_Location(loc), static_cast<GLsizei>(size), vs)))
				return;
		}
	}
	catch (utils::Tilia_Exception& e) {
		e.Add_Message("Failed to set uniform for shader { ID: %v }")(m_ID);

		throw e;
	}
}

void tilia::gfx::Shader::Uniform(const std::string& loc, const float* vs, const std::size_t& size_x, const std::size_t& size_y)
{

	try {
		switch (size_x)
		{
		case 2:
			switch (size_y)
			{
			case 2:
				SET_UNIFORM(GL_CALL(glUniformMatrix2fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			case 3:
				SET_UNIFORM(GL_CALL(glUniformMatrix2x3fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			case 4:
				SET_UNIFORM(GL_CALL(glUniformMatrix2x4fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			}
			return;
		case 3:
			switch (size_y)
			{
			case 2:
				SET_UNIFORM(GL_CALL(glUniformMatrix3x2fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			case 3:
				SET_UNIFORM(GL_CALL(glUniformMatrix3fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			case 4:
				SET_UNIFORM(GL_CALL(glUniformMatrix3x4fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			}
			return;
		case 4:
			switch (size_y)
			{
			case 2:
				SET_UNIFORM(GL_CALL(glUniformMatrix4x2fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			case 3:
				SET_UNIFORM(GL_CALL(glUniformMatrix4x3fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			case 4:
				SET_UNIFORM(GL_CALL(glUniformMatrix4fv(Get_Uniform_Location(loc), 1, GL_FALSE, vs)))
				return;
			}
			return;
		}
	}
	catch (utils::Tilia_Exception& e) {
		e.Add_Message("Failed to set uniform for shader { ID: %v }")(m_ID);

		throw e;
	}

}

std::int32_t tilia::gfx::Shader::Get_Uniform_Location(const std::string& name)
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