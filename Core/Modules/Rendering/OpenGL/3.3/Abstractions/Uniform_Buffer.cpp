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
static T align_to(const T& to_round, const U& multiple_of)
{
    if (!to_round || !multiple_of)
        return to_round;
    return ((to_round + multiple_of - 1) / multiple_of) * multiple_of;
}

void tilia::gfx::Uniform_Buffer::Init(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables)
{

    GL_CALL(glGenBuffers(1, &m_ID));

    Reset(std::move(block_variables));

    // Make sure the buffer is a uniform buffer
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

}

void tilia::gfx::Uniform_Buffer::Reset(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables)
{

    const std::size_t vec4_size{ (*enums::GLSL_Container_Type::Vector4 * utils::Get_GLSL_Scalar_Size(enums::GLSL_Scalar_Type::Float)) };

    std::vector<std::pair<std::string, GLSL_Variable>> variables{ std::move(block_variables) };

    std::size_t block_size{};

    const std::size_t var_count{ variables.size() };
    for (std::size_t i{ 0 }; i < var_count; ++i)
    {

        //std::size_t variable_size{ utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type) * *variables[i].second.container_type };

        //if (*variables[i].second.container_type > *enums::GLSL_Container_Type::Vector2 && block_size % vec4_size != 0 || variables[i].second.array_count)
        //{
        //    block_size = round_up(block_size, vec4_size);
        //}

        //if (variables[i].second.array_count || *variables[i].second.container_type >= *enums::GLSL_Container_Type::Vector2)
        //{

        //    variable_size = round_up(variable_size, vec4_size);

        //    std::size_t array_count{};
        //    if (!variables[i].second.array_count)
        //    {
        //        array_count = *variables[i].second.container_type;
        //    }
        //    else
        //    {
        //        array_count = variables[i].second.array_count * *variables[i].second.container_type;
        //    }

        //    for (std::size_t u{ 0 }; u < array_count; ++u)
        //    {

        //        if (!variables[i].second.array_count)
        //        {

        //            if (*variables[i].second.container_type < *enums::GLSL_Container_Type::Matrix2)
        //            {

        //                m_variables[(std::stringstream() << variables[i].first << '.' << ((u != 3)? static_cast<char>('x' + u) : 'w')).str()] = { block_size, variable_size };
        //                m_variables[(std::stringstream() << variables[i].first << '[' << u << ']').str()] = { block_size, variable_size };

        //            }
        //            else if (*variables[i].second.container_type >= *enums::GLSL_Container_Type::Matrix2)
        //            {

        //                m_variables[(std::stringstream() << variables[i].first << '[' << static_cast<std::size_t>(u / utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type)) << ']' << '[' << (u % utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type)) << ']').str()] = { block_size, variable_size / utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type) };

        //                m_variables[(std::stringstream() << variables[i].first << '[' << static_cast<std::size_t>(u / utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type)) << ']').str()] = { block_size, variable_size };

        //                m_variables[(std::stringstream() << variables[i].first << '[' << static_cast<std::size_t>(u / utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type)) << ']' << '.' << (((u % utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type) != 3) ? static_cast<char>('x' + (u % utils::Get_GLSL_Scalar_Size(variables[i].second.scalar_type))) : 'w'))).str()] = { block_size, variable_size };

        //            }

        //        }

        //    }

        //}

        //const std::size_t& array_count{ variables[i].second.array_count };
        //for (std::size_t u{ 0 }; u < array_count; ++u)
        //{

        //    if (u == 0)
        //        m_arrays[variables[i].first] = { block_size, 0, variable_size };

        //    variable_size = round_up(variable_size, vec4_size);

        //    std::stringstream var_name{};
        //    var_name << variables[i].first << '[' << u << ']';

        //    m_variables[var_name.str()] = { block_size, variable_size };

        //    block_size += variable_size;

        //    if (u == array_count - 1)
        //        m_arrays[variables[i].first][1] = block_size;

        //}

        //if (!array_count)
        //{

        //    m_variables[variables[i].first] = { block_size, variable_size };

        //    block_size += variable_size;
        //}

        Push_Variable(block_size, variables[i].first, variables[i].second);

    }

    block_size = align_to(block_size, vec4_size);

    std::cout << block_size << '\n';

    // Make sure the buffer is a uniform buffer
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));

    glBufferData(GL_UNIFORM_BUFFER, block_size, NULL, GL_DYNAMIC_DRAW);

    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

}

void tilia::gfx::Uniform_Buffer::debug_print()
{

    // for (auto& var : m_variables)
    // {

    //     std::cout << "name: " << var.first << " : offset: " << var.second.Get_Scalar_Type << " : size: " << var.second[1] << '\n';

    // }
    // for (auto& arr : m_arrays)
    // {

    //     std::cout << "name: " << arr.first << " : offset: " << arr.second[0] << " : size: " << arr.second[1] << " : stride: " << arr.second[2] << '\n';

    // }

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

void tilia::gfx::Uniform_Buffer::Uniform(const std::size_t& offset, const std::size_t& array_size, const std::size_t& var_size, const void* vs)
{

    std::size_t variable_size{ 0 };

    const std::size_t vec4_size{ (*enums::GLSL_Container_Type::Vector4 * utils::Get_GLSL_Scalar_Size(enums::GLSL_Scalar_Type::Float)) };

    const std::size_t stride{ align_to(var_size, vec4_size) };

    if (m_ID != s_bound_ID)
    {
        Unbind(true);
        Bind();
    }

    for (std::size_t i{ offset }; i < (array_size + offset); i += stride)
    {
        GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, i, var_size, static_cast<const void*>(static_cast<const char*>(vs) + variable_size)));
        variable_size += var_size;
    }

    if (m_ID == s_bound_ID && m_ID != s_previous_ID)
        Rebind();

}

void tilia::gfx::Uniform_Buffer::Push_Variable(std::size_t& block_size, const std::string& name, const GLSL_Variable& variable)
{

    const std::size_t vec4_size{ (*enums::GLSL_Container_Type::Vector4 * utils::Get_GLSL_Scalar_Size(enums::GLSL_Scalar_Type::Float)) };

    m_variables.at(name) = { align_to(block_size, utils::Get_GLSL_Container_Alignment(variable.Get_Container_Type()) * utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type())), variable };

}

/*
void tilia::gfx::Uniform_Buffer::Push_Variable(std::size_t& block_size, const std::string& name, const GLSL_Variable& variable)
{

    const std::size_t vec4_size{ (*enums::GLSL_Container_Type::Vector4 * utils::Get_GLSL_Scalar_Size(enums::GLSL_Scalar_Type::Float)) };

    if (*variable.Get_Container_Type() > *enums::GLSL_Container_Type::Vector2 && block_size % vec4_size != 0 || variable.Get_Array_Count())
    {
        block_size = align_to(block_size, vec4_size);
    }

    std::size_t variable_size{ utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type()) * *variable.Get_Container_Type() };

    std::size_t array_count{};

    if (variable.Get_Array_Count() || *variable.Get_Container_Type() >= *enums::GLSL_Container_Type::Matrix2)
    {

        if (!variable.Get_Array_Count())
        {
            array_count = *variable.Get_Container_Type() / utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type());
            m_arrays[name] = { block_size, 0, variable_size / array_count };
        }
        else
        {
            array_count = variable.Get_Array_Count();
            if (*variable.Get_Container_Type() >= *enums::GLSL_Container_Type::Matrix2)
            {
                m_arrays[name] = { block_size, 0, variable_size / (*variable.Get_Container_Type() / utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type())) };
            }
            else
            {
                m_arrays[name] = { block_size, 0, variable_size };
            }
        }

        for (std::size_t u{ 0 }; u < array_count; ++u)
        {

            std::stringstream var_name{};
            var_name << name << '[' << u << ']';

            if (!variable.Get_Array_Count())
            {
                m_variables[var_name.str()] = { block_size, variable_size / array_count };
                block_size += align_to(variable_size, vec4_size) / array_count;
            }
            else
            {
                m_variables[var_name.str()] = { block_size, variable_size };
                block_size += align_to(variable_size, vec4_size);
            }

        }

        if (!variable.Get_Array_Count())
        {
            m_arrays[name][1] = align_to(variable_size, vec4_size);
        }
        else
        {
            m_arrays[name][1] = align_to(variable_size, vec4_size) * array_count;
        }

    }
    else
    {

        m_variables[name] = { block_size, variable_size };

        block_size += variable_size;
    }

}
*/