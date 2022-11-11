// Vendor
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <iostream>

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Uniform_Buffer.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"

std::uint32_t tilia::gfx::Uniform_Buffer::s_bound_ID{};
std::uint32_t tilia::gfx::Uniform_Buffer::s_previous_ID{};

void tilia::gfx::Uniform_Buffer::Init(std::initializer_list<std::pair<std::string, std::pair<enums::GLSL_Type, std::size_t>>> block_variables)
{

    GL_CALL(glGenBuffers(1, &m_ID));

    std::size_t size{};

    for (auto& variable : block_variables)
    {
        m_variables[variable.first] = { size, *variable.second.first * variable.second.second };
        if (variable == *(block_variables.end() - 1))
        {
            size += *variable.second.first * variable.second.second;
            break;
        }
        size += (*variable.second.first * variable.second.second >= 16)? *variable.second.first * variable.second.second : 16;
    }

    std::cout << size << '\n';

    // Make sure the buffer is a uniform buffer
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));

    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

}

void tilia::gfx::Uniform_Buffer::debug_print()
{

    for (auto& var : m_variables)
    {

        std::cout << "name: " << var.first << " : offset: " << var.second.first << " : size: " << var.second.second << '\n';

    }

}

void tilia::gfx::Uniform_Buffer::Set_Bind_Point(const std::uint32_t& bind_point)
{
    m_bind_point = bind_point;

    GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, m_ID));
}

void tilia::gfx::Uniform_Buffer::Bind() const {
    if (!m_ID)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to bind uniform buffer { ID: %v }"
		)(m_ID);

		throw e;

	}
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
    s_bound_ID = m_ID;
}

void tilia::gfx::Uniform_Buffer::Bind(const std::uint32_t& id) {
    if (!id)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to bind uniform buffer { ID: %v }"
		)(id);

		throw e;

	}
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, id));
    s_bound_ID = id;
}

void tilia::gfx::Uniform_Buffer::Unbind(const bool& save_id) {
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    if (save_id)
    {
        s_previous_ID = s_bound_ID;
    }

	s_bound_ID = 0;
}

void tilia::gfx::Uniform_Buffer::Rebind() {
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, s_previous_ID));
	s_bound_ID = s_previous_ID;
	s_bound_ID = 0;
}

void tilia::gfx::Uniform_Buffer::Uniform(const std::string& loc, const void* vs)
{

    if (m_ID != s_bound_ID)
	{
        Unbind(true);
        Bind(); 
    }

    GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, m_variables[loc].first, m_variables[loc].second, vs));

	if (m_ID == s_bound_ID && m_ID != s_previous_ID)
        Rebind(); 

}