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

// The size of a vector4 is used to align things to its size
static const std::size_t VEC4_SIZE{ (*tilia::enums::GLSL_Container_Type::Vector4 * tilia::utils::Get_GLSL_Scalar_Size(tilia::enums::GLSL_Scalar_Type::Float)) };

void tilia::gfx::Uniform_Buffer::Reset(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables)
{

    std::vector<std::pair<std::string, GLSL_Variable>> variables{ std::move(block_variables) };

    std::size_t block_size{};

    const std::size_t var_count{ variables.size() };
    for (std::size_t i{ 0 }; i < var_count; ++i)
    {

        block_size = Push_Variable(block_size, std::move(variables[i].first), std::move(variables[i].second));

    }

    block_size = align_to(block_size, VEC4_SIZE);

    // Make sure the buffer is a uniform buffer
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));

    glBufferData(GL_UNIFORM_BUFFER, block_size, NULL, GL_DYNAMIC_DRAW);

    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

}

void tilia::gfx::Uniform_Buffer::debug_print()
{

     for (auto& var : m_variables)
     {

         std::cout << "name: " << var.first << " : offset: " << var.second.first << '\n';

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

void tilia::gfx::Uniform_Buffer::Uniform(const std::string& loc, const std::size_t& var_size, const void* vs)
{

    if (m_ID != s_bound_ID)
    {
        Unbind(true);
        Bind();
    }

    std::size_t& start_offset{ m_variables[loc].first };
    GLSL_Variable& variable{ m_variables[loc].second };

    const std::size_t array_count{ variable.Get_Array_Count() };
    if (!array_count)
    {

        if (*variable.Get_Container_Type() < *enums::GLSL_Container_Type::Matrix2)
        {

            GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, start_offset, var_size, static_cast<const void*>(static_cast<const char*>(vs))));

        }
        else
        {

            const std::size_t stride{ align_to(var_size, VEC4_SIZE) };
            const std::size_t element_count{ static_cast<std::size_t>(*variable.Get_Container_Type() / *enums::GLSL_Container_Type::Vector4) };
            for (std::size_t u{ 0 }; u < element_count; ++u)
            {

                GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, start_offset + stride * u, var_size, static_cast<const void*>(static_cast<const char*>(vs) + var_size * u)));

            }

        }

    }
    else
    {

        const std::size_t stride{ align_to(var_size, VEC4_SIZE) };
        for (std::size_t i{ 0 }; i < array_count; ++i)
        {

            if (*variable.Get_Container_Type() < *enums::GLSL_Container_Type::Matrix2)
            {
                GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, start_offset + stride * i, var_size, static_cast<const void*>(static_cast<const char*>(vs) + var_size * i)));
            }
            else
            {

                const std::size_t element_count{ static_cast<std::size_t>(*variable.Get_Container_Type() / *enums::GLSL_Container_Type::Vector4) };
                for (std::size_t u{ 0 }; u < element_count; ++u)
                {

                    GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, start_offset + (stride * element_count * i) + (stride * u), var_size, static_cast<const void*>(static_cast<const char*>(vs) + (var_size * element_count * i) + (var_size * u))));

                }

            }

        }

    }

    if (m_ID == s_bound_ID && m_ID != s_previous_ID)
        Rebind();

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

    const std::size_t stride{ align_to(var_size, VEC4_SIZE) };

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

std::size_t tilia::gfx::Uniform_Buffer::Push_Variable(std::size_t block_size, const std::string& name, const GLSL_Variable& variable)
{

    // Aligns the block size to the size of a vector4
    block_size = align_to(block_size, utils::Get_GLSL_Container_Alignment(variable.Get_Container_Type()) * utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type()));

    const std::size_t array_count{ variable.Get_Array_Count() };
    if (array_count)
        block_size = align_to(block_size, VEC4_SIZE);

    m_variables[name] = std::make_pair(block_size, variable);

    if (!array_count)
    {
        
        // If container type is vector
        if (*variable.Get_Container_Type() >= *enums::GLSL_Container_Type::Vector2 && *variable.Get_Container_Type() <= *enums::GLSL_Container_Type::Vector4)
        {

            const std::size_t element_count{ static_cast<std::size_t>(*variable.Get_Container_Type()) };
            for (std::size_t i{ 0 }; i < element_count; ++i)
            {

                // Two types of accessable names
                std::string new_name_1{ (std::stringstream() << name << '.' << ((i != 3)? static_cast<char>('x' + i) : 'w')).str() },
                            new_name_2{ (std::stringstream() << name << '[' << i << ']').str() };

                std::size_t temp{ block_size };

                // Recursively pushes each element of the vector with the indexing added at the end of the name
                block_size = Push_Variable(block_size, new_name_1, GLSL_Variable(variable.Get_Scalar_Type(), enums::GLSL_Container_Type::Scalar));
                Push_Variable(temp, new_name_2, GLSL_Variable(variable.Get_Scalar_Type(), enums::GLSL_Container_Type::Scalar));

            }

        }
        // If container type is matrix
        else if (*variable.Get_Container_Type() >= *enums::GLSL_Container_Type::Matrix2)
        {

            const std::size_t element_count{ static_cast<std::size_t>(*variable.Get_Container_Type() / *enums::GLSL_Container_Type::Vector4) };
            for (std::size_t i{ 0 }; i < element_count; ++i)
            {

                std::string new_name{ (std::stringstream() << name << '[' << i << ']').str() };

                // Recursively pushes each element as a vector4 of the matrix with the indexing added at the end of the name
                block_size = Push_Variable(block_size, new_name, GLSL_Variable(variable.Get_Scalar_Type(), enums::GLSL_Container_Type::Vector4));

                block_size = align_to(block_size, VEC4_SIZE);

            }

        }
        // If container type is scalar
        else if (variable.Get_Container_Type() == enums::GLSL_Container_Type::Scalar)
        {
            block_size += utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type()) * *variable.Get_Container_Type();
        }

    }
    else
    {

        for (std::size_t i{ 0 }; i < array_count; ++i)
        {

            std::string new_name{ (std::stringstream() << name << '[' << i << ']').str() };

            // Recursively pushes each element of the array with the indexing added at the end of the name
            block_size = Push_Variable(block_size, new_name, GLSL_Variable(variable.Get_Scalar_Type(), variable.Get_Container_Type()));

            block_size = align_to(block_size, VEC4_SIZE);

        }

    }

    return block_size;

}