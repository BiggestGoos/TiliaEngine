// Vendor
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <iostream>

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Uniform_Buffer.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Values/OpenGL/3.3/Utils.hpp"

std::uint32_t tilia::gfx::Uniform_Buffer::s_bound_ID{};
std::uint32_t tilia::gfx::Uniform_Buffer::s_previous_ID{};

template<typename T, typename U>
static T round_up(const T& to_round, const U& multiple_of)
{
    if (!multiple_of)
        return to_round;
    return ((to_round + multiple_of - 1) / multiple_of) * multiple_of;
}

void tilia::gfx::Uniform_Buffer::Init(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables)
{

    GL_CALL(glGenBuffers(1, &m_ID));

    const std::size_t vec4_size{ (*enums::GLSL_Container_Type::Vector4 * utils::Get_GLSL_Scalar_Size(enums::GLSL_Scalar_Type::Float)) };

    std::vector<std::pair<std::string, GLSL_Variable>> variables{ block_variables };

    std::size_t block_size{};

    const std::size_t var_count{ variables.size() };
    for (std::size_t i{ 0 }; i < var_count; ++i)
    {

        std::size_t variable_size{};

        if (variables[i].second.container_type != enums::GLSL_Container_Type::Scalar && variables[i].second.container_type != enums::GLSL_Container_Type::Vector2 && block_size % vec4_size || variables[i].second.array_count)
        {
            block_size = round_up(block_size, vec4_size);
        }

        const std::size_t& array_count{ variables[i].second.array_count };
        for (std::size_t u{ 0 }; u < array_count; ++u)
        {

            variable_size = utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type) * *variables[i].second.container_type;

            if (u == 0)
                m_arrays[variables[i].first] = { block_size, 0, variable_size };

            variable_size = round_up(variable_size, vec4_size);

            std::stringstream var_name{};
            var_name << variables[i].first << '[' << u << ']';

            m_variables[var_name.str()] = { block_size, variable_size };

            block_size += variable_size;

            if (u == array_count - 1)
                m_arrays[variables[i].first][1] = block_size;
            
        }

        if (!array_count)
        {
            variable_size = utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type) * *variables[i].second.container_type;

            m_variables[variables[i].first] = { block_size, variable_size };

            block_size += variable_size;
        }

    }

    block_size = round_up(block_size, vec4_size);

    std::cout << block_size << '\n';

    // Make sure the buffer is a uniform buffer
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));

    glBufferData(GL_UNIFORM_BUFFER, block_size, NULL, GL_DYNAMIC_DRAW);

    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

}

void tilia::gfx::Uniform_Buffer::debug_print()
{

    for (auto& var : m_variables)
    {

        std::cout << "name: " << var.first << " : offset: " << var.second[0] << " : size: " << var.second[1] << '\n';

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

void tilia::gfx::Uniform_Buffer::Uniform(const std::size_t& offset, const std::size_t& size, const void* vs)
{

    if (m_ID != s_bound_ID)
    {
        Unbind(true);
        Bind();
    }

    GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, vs));

    if (m_ID == s_bound_ID && m_ID != s_previous_ID)
        Rebind();

}

void tilia::gfx::Uniform_Buffer::Uniform(const std::size_t& offset, const std::size_t& size, const std::size_t& stride, const void* vs)
{

    if (m_ID != s_bound_ID)
    {
        Unbind(true);
        Bind();
    }

    for (std::size_t i{ offset }; i < size; i += stride)
    {
        GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, i, stride, static_cast<const void*>(reinterpret_cast<const char*>(vs) + stride)));
    }

    if (m_ID == s_bound_ID && m_ID != s_previous_ID)
        Rebind();

}
