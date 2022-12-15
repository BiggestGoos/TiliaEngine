// Vendor
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <iostream>

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader_files/Uniform_Buffer.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Values/OpenGL/3.3/Utils.hpp"

// Defines static variables
std::uint32_t tilia::gfx::Uniform_Buffer::s_bound_ID{};
std::uint32_t tilia::gfx::Uniform_Buffer::s_previous_ID{};

/**
 * @brief Aligns a number to the closest higher multiple of another number.
 * 
 * @tparam T - The type of the number to align.
 * @tparam U - The type of the number which to align to.
 * @param to_round - The number to align.
 * @param multiple_of - The number which to align to.
 * 
 * @return The aligned number.
 */
template<typename T, typename U>
static T align_to(const T& to_round, const U& multiple_of)
{
    // If either of the given numbers are zero
    if (!to_round || !multiple_of)
        return to_round;
    // Uses math to align number to other number
    return static_cast<T>(((to_round + multiple_of - 1) / multiple_of) * multiple_of);
}

void tilia::gfx::Uniform_Buffer::Init(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables, const bool& indexing, const std::int32_t& bind_point)
{
    GL_CALL(glGenBuffers(1, &m_ID));
    // If any variables is given then adds them 
    if (block_variables.begin() != block_variables.end())
        Reset(std::move(block_variables), indexing);
    // Make sure the buffer is a uniform buffer
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    // If given then sets binding point to the given
    if (bind_point >= 0)
        Set_Bind_Point(static_cast<std::uint32_t>(bind_point));
}

void tilia::gfx::Uniform_Buffer::Init(const Uniform_Buffer& other, const std::int32_t& bind_point)
{
    // Copies variables from other uniform buffer
    m_variables = other.m_variables;
    // Calls default init fuction with bind point as argument which will generate a ubo which the id will be set to and bind it to bind point
    Init({}, false, ((bind_point)? bind_point : other.m_bind_point));
}

void tilia::gfx::Uniform_Buffer::Init(Uniform_Buffer&& other, const std::int32_t& bind_point)
{
    // Copies id and then sets other id to 0 as default
    m_ID = other.m_ID;
    other.m_ID = 0;
    // Copies bind point from other and then sets othe bind point to 0
    m_bind_point = other.m_bind_point;
    other.m_bind_point = 0;
    // Moves other variables data to variables
    m_variables = std::move(other.m_variables);
    // If given then sets binding point to the given
    if (bind_point >= 0)
        Set_Bind_Point(static_cast<std::uint32_t>(bind_point));
}

// The size of a vector4 is used to align things to its size
static const std::size_t VEC4_SIZE{ (*tilia::enums::GLSL_Container_Type::Vector4 * tilia::utils::Get_GLSL_Scalar_Size(tilia::enums::GLSL_Scalar_Type::Float)) };

void tilia::gfx::Uniform_Buffer::Reset(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables, const bool& indexing)
{

    // Clears the previous variables
    Clear();

    // The type of elements in the given ones
    using T = decltype(block_variables)::value_type;

    // Moves given variables into vector in order to index to them
    std::vector<T> variables{ std::move(block_variables) };
    
    std::size_t block_size{};

    const std::size_t var_count{ variables.size() };
    for (std::size_t i{ 0 }; i < var_count; ++i)
    {
        // Pushes (adds) variable to storage which then allows for the setting of uniform data using the name
        block_size = Push_Variable(block_size, std::move(variables[i].first), std::move(variables[i].second), indexing);
    }

    // Block size has to end in a multiple of a vector4
    block_size = align_to(block_size, VEC4_SIZE);

    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));

    // Allocates memory for the total size of the uniform block
    glBufferData(GL_UNIFORM_BUFFER, block_size, NULL, GL_DYNAMIC_DRAW);

    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

}

void tilia::gfx::Uniform_Buffer::Reset(const Uniform_Buffer& other)
{
}

void tilia::gfx::Uniform_Buffer::Reset(Uniform_Buffer&& other)
{
}

void tilia::gfx::Uniform_Buffer::Clear()
{

    // Clears the stored variables
    m_variables.clear();

}

void tilia::gfx::Uniform_Buffer::debug_print()
{

    for (auto& variable : m_variables)
    {
        std::cout << "Name: " << variable.first << " : Offset " << variable.second.first << '\n';
    }

}

void tilia::gfx::Uniform_Buffer::Set_Bind_Point(const std::uint32_t& bind_point)
{
    // If id is zero then throws exception
    if (!m_ID)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to set binding point for uniform buffer { ID: %v }"
		)(m_ID);

		throw e;

	}
    // Stores given binding point
    m_bind_point = bind_point;
    // Binds the ubo to the given binding point
    GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, m_ID));
}

void tilia::gfx::Uniform_Buffer::Bind() const {
    // If id is zero then throws exception
    if (!m_ID)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to bind uniform buffer { ID: %v }"
		)(m_ID);

		throw e;

	}
    // Binds ubo with id
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
    // Stores the id as the new bound id
    s_bound_ID = m_ID;
}

void tilia::gfx::Uniform_Buffer::Bind(const std::uint32_t& id) {
    // If id is zero then throws exception
    if (!id)
	{
		utils::Tilia_Exception e{ LOCATION };

		e.Add_Message("Failed to bind uniform buffer { ID: %v }"
		)(id);

		throw e;

	}
    // Binds ubo with id
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, id));
    // Stores the id as the new bound id
    s_bound_ID = id;
}

void tilia::gfx::Uniform_Buffer::Unbind(const bool& save_id) {
    // Binds the openGL ubo to zero
    GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    // If true then saves the old bound id
    if (save_id)
    {
        s_previous_ID = s_bound_ID;
    }

	s_bound_ID = 0;
}

void tilia::gfx::Uniform_Buffer::Rebind() {
    // Binds the previously bound id
	GL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, s_previous_ID));
    // Sets the current bound id to the previous bound id
	s_bound_ID = s_previous_ID;
	s_bound_ID = 0;
}

void tilia::gfx::Uniform_Buffer::Uniform(const std::size_t& offset, const std::size_t& size, const void* vs)
{
    // If ubo is not already bound then binds it
    if (m_ID != s_bound_ID)
    {
        Unbind(true);
        Bind();
    }
    // Sets the data of the offset and size with the given data
    GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, vs));
    // If just bound the ubo then rebinds the old one
    if (m_ID == s_bound_ID && m_ID != s_previous_ID)
        Rebind();

}

void tilia::gfx::Uniform_Buffer::Uniform(const std::string& loc, const std::size_t& var_size, const void* vs)
{
    // If ubo is not already bound then binds it
    if (m_ID != s_bound_ID)
    {
        Unbind(true);
        Bind();
    }

    // The offset to the start of the variable in the uniform block
    std::size_t& start_offset{ m_variables[loc].first };
    // The variable of which to set the uniform data for
    GLSL_Variable& variable{ m_variables[loc].second };

    const std::size_t array_count{ variable.Get_Array_Count() };
    // If variable is not an array
    if (!array_count)
    {
        // If variable is not matrix
        if (*variable.Get_Container_Type() < *enums::GLSL_Container_Type::Matrix2)
        {
            // Sets the uniform data for the given variable
            GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, start_offset, var_size, static_cast<const void*>(static_cast<const char*>(vs))));
            // If just bound, the ubo then rebinds the old one
            if (m_ID == s_bound_ID && m_ID != s_previous_ID)
                Rebind();
            // Early return
            return;
        }

        // If variable is matrix

        // The size in bytes between each element (vector4)
        const std::size_t stride{ VEC4_SIZE };
        // However many elements (vector4) there are in the matrix
        const std::size_t element_count{ static_cast<std::size_t>(*variable.Get_Container_Type() / *enums::GLSL_Container_Type::Vector4) };
        for (std::size_t u{ 0 }; u < element_count; ++u)
        {
            // Sets the uniform data for the element of the given matrix
            GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, start_offset + stride * u, var_size, static_cast<const void*>(static_cast<const char*>(vs) + var_size * u)));
        }
        // If just bound, the ubo then rebinds the old one
        if (m_ID == s_bound_ID && m_ID != s_previous_ID)
            Rebind();
        // Early return
        return;
    }

    // If variable is array

    // The size in bytes between each element in the array
    const std::size_t stride{ align_to(var_size, VEC4_SIZE) };
    for (std::size_t i{ 0 }; i < array_count; ++i)
    {
        // If element of array is not matrix
        if (*variable.Get_Container_Type() < *enums::GLSL_Container_Type::Matrix2)
        {
            // Sets the uniform data for the element of the given array
            GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, start_offset + stride * i, var_size, static_cast<const void*>(static_cast<const char*>(vs) + var_size * i)));
            // If just bound, the ubo then rebinds the old one
            if (i == array_count - 1 && m_ID == s_bound_ID && m_ID != s_previous_ID)
            {
                Rebind();
                // Early return
                return;
            }
            else if (i == array_count - 1)
            {
                // Early return
                return;
            }
            // Early continue
            continue;
        }
        // The amount of elements in the matrix
        const std::size_t element_count{ static_cast<std::size_t>(*variable.Get_Container_Type() / *enums::GLSL_Container_Type::Vector4) };
        for (std::size_t u{ 0 }; u < element_count; ++u)
        {
            // Sets the uniform data of the matrix element of the array
            GL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, start_offset + (stride * element_count * i) + (stride * u), var_size, static_cast<const void*>(static_cast<const char*>(vs) + (var_size * element_count * i) + (var_size * u))));
        }
    }

    // If just bound, the ubo then rebinds the old one
    if (m_ID == s_bound_ID && m_ID != s_previous_ID)
        Rebind();

}

std::size_t tilia::gfx::Uniform_Buffer::Push_Variable(std::size_t block_size, const std::string& name, const GLSL_Variable& variable, const bool& indexing)
{

    // Aligns the block size to the alignment of the given variable
    block_size = align_to(block_size, utils::Get_GLSL_Container_Alignment(variable.Get_Container_Type()) * utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type()));

    const std::size_t array_count{ variable.Get_Array_Count() };
    // If variable is an array then aligns the block size to the alignment of a vector4
    if (array_count)
        block_size = align_to(block_size, VEC4_SIZE);
    // Stores block size and variable with the name
    m_variables[name] = std::make_pair(block_size, variable);
    // The size of the variable or if array then size of each element
    const std::size_t variable_size{ utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type()) * *variable.Get_Container_Type() };

    // If the given variable is an array
    if (array_count)
    {
        // If indexing is false then just adds total size of array
        if (!indexing)
        {
            // Adds total size of array to block size
            block_size += array_count * align_to(variable_size, VEC4_SIZE);
            // Early return
            return block_size;
        }
        // If indexing is true then adds extra variables with indexing attached
        for (std::size_t i{ 0 }; i < array_count; ++i)
        {
            // Indexing name
            std::string new_name{ (std::stringstream() << name << '[' << i << ']').str() };
            // Recursively pushes each element of the array with the indexing added at the end of the name
            // Block size is also increased with each call.
            block_size = Push_Variable(block_size, new_name, GLSL_Variable(variable.Get_Scalar_Type(), variable.Get_Container_Type()), indexing);
            block_size = align_to(block_size, VEC4_SIZE);
        }
    }
    // If container type is vector
    else if (*variable.Get_Container_Type() >= *enums::GLSL_Container_Type::Vector2 && *variable.Get_Container_Type() <= *enums::GLSL_Container_Type::Vector4)
    {
        // If indexing is false then just adds total size of vector
        if (!indexing)
        {
            // Adds total size of vector to block size
            block_size += variable_size;
            // Early return
            return block_size;
        }
        // If indexing is true then adds extra variables with indexing attached
        // The number of elements in the vector
        const std::size_t element_count{ static_cast<std::size_t>(*variable.Get_Container_Type()) };
        for (std::size_t i{ 0 }; i < element_count; ++i)
        {
            // Two types of indexing names
            std::string new_name_1{ (std::stringstream() << name << '.' << ((i != 3)? static_cast<char>('x' + i) : 'w')).str() },
                        new_name_2{ (std::stringstream() << name << '[' << i << ']').str() };
            std::size_t temp{ block_size };
            // Recursively pushes each element of the vector with the indexing added at the end of the name
            // Block size is also increased with each call.
            block_size = Push_Variable(block_size, new_name_1, GLSL_Variable(variable.Get_Scalar_Type(), enums::GLSL_Container_Type::Scalar), indexing);
            Push_Variable(temp, new_name_2, GLSL_Variable(variable.Get_Scalar_Type(), enums::GLSL_Container_Type::Scalar), indexing);
        }
    }
    // If container type is matrix
    else if (*variable.Get_Container_Type() >= *enums::GLSL_Container_Type::Matrix2)
    {
        // If indexing is false then just adds total size of matrix
        if (!indexing)
        {
            // Adds total size of matrix to block size
            block_size += variable_size;
            // Early return
            return block_size;
        }
        // If indexing is true then adds extra variables with indexing attached
        // The number of elements (columns) in the matrix
        const std::size_t element_count{ static_cast<std::size_t>(*variable.Get_Container_Type() / *enums::GLSL_Container_Type::Vector4) };
        for (std::size_t i{ 0 }; i < element_count; ++i)
        {
            // Indexing name
            std::string new_name{ (std::stringstream() << name << '[' << i << ']').str() };
            // Recursively pushes each element as a vector4 of the matrix with the indexing added at the end of the name
            // Block size is also increased with each call.
            block_size = Push_Variable(block_size, new_name, GLSL_Variable(variable.Get_Scalar_Type(), enums::GLSL_Container_Type::Vector4), indexing);
            block_size = align_to(block_size, VEC4_SIZE);
        }
    }
    // If container type is scalar
    else if (variable.Get_Container_Type() == enums::GLSL_Container_Type::Scalar)
    {
        // Adds size of variable to block size
        block_size += utils::Get_GLSL_Scalar_Size(variable.Get_Scalar_Type()) * *variable.Get_Container_Type();
    }

    // Returns the new block size
    return block_size;

}