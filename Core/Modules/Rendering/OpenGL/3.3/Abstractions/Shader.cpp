/*****************************************************************//**
 * @file   Shader.cpp
 * @brief  Defines all non-inline member functions and constructors/destructors of the @see @Shader class.
 * 
 * Dependencies:
 * @include "dependencies/glad/include/glad/glad.h"
 * 
 * Standard:
 * @include <fstream>
 * @include <sstream>
 * @include <memory>
 * 
 * Headers:
 * @include "headers/Error_Handling.h"
 * @include "headers/Shader.h"
 * @include "headers/Logging.h"
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

// Dependencies
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <fstream>
#include <sstream>
#include <memory>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"
#include "Core/Modules/Console/Logging.hpp"
#include "Core/Values/OpenGL/3.3/Enums.hpp"

#if 0

// Initialize static member which holds the bound shader id
uint32_t tilia::gfx::Shader::s_bound_ID{};
// Initialize static member which holds the previously bound shader id
uint32_t tilia::gfx::Shader::s_previous_ID{};

/**
 * If there is no source code for the respective shaders then the Parse_Shader 
 * is called with that shader. After that it calls the Create_Shader function. 
 * If any function returns false, throws exception and prints error. If everything 
 * went right, prints success message.
 */
bool tilia::gfx::Shader::Generate_Shader()
{

    try
    {
        // Parse vertex-shader
        if (m_vertex_shader.source == "")
            if (!Parse_Shader(m_vertex_shader))
                throw 1;
        // Parse fragment-shader
        if (m_fragment_shader.source == "")
            if (!Parse_Shader(m_fragment_shader))
                throw 1;

        if (!Create_Shader())
            throw 1;

        log::Log(log::Type::SUCCESS, "SHADER", "Shader { ID: %u } was successfuly created", m_ID);

    }
    catch (...)
    {
        log::Log(log::Type::ERROR, "SHADER", "Could not create shader");
        return false;
    }

    return true;

}

/**
 * Copies the memeber Shader_Types to the same as the given 
 * Shader_Types and then calls the Generate_Shader function.
 */
bool tilia::gfx::Shader::Init(const Shader_Type& vert_shader, const Shader_Type& frag_shader)
{

    m_vertex_shader = vert_shader;
    m_fragment_shader = frag_shader;

    return Generate_Shader();

}

/**
 * Sets the memeber Shader_Types to have the given paths and 
 * no sorce code and then calls the Generate_Shader function.
 */
bool tilia::gfx::Shader::Init(const std::string& vert_path, const std::string& frag_path)
{

    m_vertex_shader = { vert_path, "" };
    m_fragment_shader = { frag_path, "" };

    return Generate_Shader();

}

/**
 * Copies the memeber Shader_Types to the same as the
 * Shader_Types of the given Shader and then calls the 
 * Generate_Shader function.
 */
bool tilia::gfx::Shader::Init(const Shader& shader)
{

    m_vertex_shader = shader.Get_Shader(0);
    m_fragment_shader = shader.Get_Shader(1);

    return Generate_Shader();

}

/**
 * Deletes the openGL shader of the m_ID
 */
tilia::gfx::Shader::~Shader()
{
    GL_CALL(glDeleteProgram(m_ID));
    log::Log(log::Type::SUCCESS, "SHADER", "Shader { ID: %u } was succesfully destroyed", m_ID);
}

/**
 * Sets exceptions of ifstream. Opens, gets data of, and closes ifstream with the path of the shader. 
 * Then sets the shader source to the data. If exception was thrown, prints errors, and returns false.
 * Otherwise it returns true at the end of the function.
 */
bool tilia::gfx::Shader::Parse_Shader(Shader_Type& shader) const {

    std::ifstream stream{};

    // Sets exceptions
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {

        // Does file stuff
        stream.open(shader.path);
        std::stringstream ss;

        ss << stream.rdbuf();

        stream.close();

        // Sets data
        shader.source = ss.str();

    }
    catch (std::ifstream::failure& e)
    {
        // Print errors
        log::Log(log::Type::ERROR, "SHADER", "Could not read file");
        log::Log_Indent("Path", "%s", shader.path.c_str());
        log::Log_Indent("Reason", e.what());
        return false;
    }

    return true;

}

/**
 * Creates shader and sets the given id, adds the source of the shader to the openGL shader 
 * with the id and then compiles the shader. Then does some error checking and if something 
 * went bad it prints errors. If error is found deletes shader with id and also returns false.
 * If it went good, returns true.
 */
bool tilia::gfx::Shader::Compile_Shader(const uint32_t& type, Shader_Type& shader, int32_t& id) {

    // Creates shader
    GL_CALL(id = glCreateShader(type));
    const char* src = shader.source.c_str();
    // Adds source
    GL_CALL(glShaderSource(id, 1, &src, nullptr));
    // Compiles shader
    GL_CALL(glCompileShader(id));
    
    // Error checking
    int32_t result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int32_t length;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::unique_ptr<char[]> message = std::make_unique<char[]>(static_cast<size_t>(length * sizeof(char)));
        GL_CALL(glGetShaderInfoLog(id, length, &length, message.get()));
        message.get()[length - 1] = '\0';
        // Prints errors
        log::Log(log::Type::ERROR, "SHADER", "Failed to compile %s shader", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        log::Log_Indent("Message", "%s", message.get());
        GL_CALL(glDeleteShader(id));
        return false;
    }

    return true;

}

/**
 * First it creates a shader program and sets m_ID to the returned id. Then it compiles 
 * the two shaders and sets the shader id's. After that it attaches the two shaders, links, 
 * and validates the shader. At last it deletes the two shaders since they already are 
 * attached to the shader program. If something went wron when compiling shaders, returns false,
 * otherwise it returns true if everything went alright.
 */
bool tilia::gfx::Shader::Create_Shader() {

    // Creates program and sets m_ID
    GL_CALL(m_ID = glCreateProgram());
    int32_t v_id{}, f_id{};
    // Compiles shaders
    if (!Compile_Shader(GL_VERTEX_SHADER, m_vertex_shader, v_id))
        return false;
    if (!Compile_Shader(GL_FRAGMENT_SHADER, m_fragment_shader, f_id))
        return false;
    
    // Attaches to, links, and validates shader program
    GL_CALL(glAttachShader(m_ID, v_id));
    GL_CALL(glAttachShader(m_ID, f_id));
    GL_CALL(glLinkProgram(m_ID));
    GL_CALL(glValidateProgram(m_ID));

    // Deletes shaders
    GL_CALL(glDeleteShader(v_id));
    GL_CALL(glDeleteShader(f_id));

    return true;

}

/**
 * Checks if the given uniform name is already found and stored in the m_uniform_locs. 
 * If it is then the location is promptly returned. If not then the location is found 
 * using the glGetUniformLocation function. If the returned location is equal to -1, 
 * prints error. If not then it adds the location to m_uniform_locs and then returns location.
 */
int32_t tilia::gfx::Shader::Get_Uniform_Location(const std::string& name)
{
    // Check m_uniform_locs
    if (m_uniform_locs.find(name) != m_uniform_locs.end())
        return m_uniform_locs[name];

    // Get location with glGetUniformLocation
    GL_CALL(int32_t location = glGetUniformLocation(m_ID, name.c_str()));
    // Check is -1
    if (location == -1)
        log::Log(log::Type::ERROR, "SHADER", "Uniform { Name: %s } does not exist / is not being used", name.c_str());

    // Adds location to m_uniform_locs
    m_uniform_locs[name] = location;

    return location;
}

/**
 * If m_ID is equal to 0 then it prints error. Otherwise it bind the shader using 
 * the glUseProgram function with m_ID. Also sets s_bound_ID to m_ID.
 */
void tilia::gfx::Shader::Bind() const
{
    if (!m_ID)
        return log::Log(log::Type::ERROR, "SHADER", "Failed to bind shader { ID: %u }", m_ID);
    GL_CALL(glUseProgram(m_ID));
    s_bound_ID = m_ID;
}

/**
 * Unbinds the currently bound shader. First checks if you want to save the previously
 * bound shader id. If so then gets the currently bound shader id and stores it in
 * s_previous_ID. Otherwise it sets s_previous_ID to 0. After that it binds the shader
 * to 0, effectively unbinding it. Also sets s_bound_ID to 0.
 */
void tilia::gfx::Shader::Unbind(const bool& save_id)
{
    // Checks save_id
    if (save_id)
    {
        // Stores previous id
        s_previous_ID = s_bound_ID;
    }
    else
    {
        s_previous_ID = 0;
    }

    // Binds texture of type m_texture_type to 0
    GL_CALL(glUseProgram(0));
}

/**
 * Binds the shader of the stored s_previous_ID id.
 */
void tilia::gfx::Shader::Rebind() const
{
    GL_CALL(glUseProgram(s_previous_ID));
}

/**
 * Macro to wrap around uniform functions.
 */
#define SET_UNIFORM(x) Unbind(true);\
                       Bind();\
                       x;\
                       Rebind();

void tilia::gfx::Shader::Uniform(const std::string& location, const float& v0)
{
    SET_UNIFORM(GL_CALL(glUniform1f(Get_Uniform_Location(location), v0)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const float& v0, const float& v1)
{
    SET_UNIFORM(GL_CALL(glUniform2f(Get_Uniform_Location(location), v0, v1)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const float& v0, const float& v1, const float& v2)
{
    SET_UNIFORM(GL_CALL(glUniform3f(Get_Uniform_Location(location), v0, v1, v2)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const float& v0, const float& v1, const float& v2, const float& v3)
{
    SET_UNIFORM(GL_CALL(glUniform4f(Get_Uniform_Location(location), v0, v1, v2, v3)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const int32_t& v0)
{
    SET_UNIFORM(GL_CALL(glUniform1i(Get_Uniform_Location(location), v0)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1)
{
    SET_UNIFORM(GL_CALL(glUniform2i(Get_Uniform_Location(location), v0, v1)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1, const int32_t& v2)
{
    SET_UNIFORM(GL_CALL(glUniform3i(Get_Uniform_Location(location), v0, v1, v2)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const int32_t& v0, const int32_t& v1, const int32_t& v2, const int32_t& v3)
{
    SET_UNIFORM(GL_CALL(glUniform4i(Get_Uniform_Location(location), v0, v1, v2, v3)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const uint32_t& v0)
{
    SET_UNIFORM(GL_CALL(glUniform1ui(Get_Uniform_Location(location), v0)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1)
{
    SET_UNIFORM(GL_CALL(glUniform2ui(Get_Uniform_Location(location), v0, v1)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1, const uint32_t& v2)
{
    SET_UNIFORM(GL_CALL(glUniform3ui(Get_Uniform_Location(location), v0, v1, v2)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const uint32_t& v0, const uint32_t& v1, const uint32_t& v2, const uint32_t& v3)
{
    SET_UNIFORM(GL_CALL(glUniform4ui(Get_Uniform_Location(location), v0, v1, v2, v3)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::vec2& v0)
{
    SET_UNIFORM(GL_CALL(glUniform2f(Get_Uniform_Location(location), v0.x, v0.y)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::vec3& v0)
{
    SET_UNIFORM(GL_CALL(glUniform3f(Get_Uniform_Location(location), v0.x, v0.y, v0.z)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::vec4& v0)
{
    SET_UNIFORM(GL_CALL(glUniform4f(Get_Uniform_Location(location), v0.x, v0.y, v0.z, v0.w)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::ivec2& v0)
{
    SET_UNIFORM(GL_CALL(glUniform2i(Get_Uniform_Location(location), v0.x, v0.y)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::ivec3& v0)
{
    SET_UNIFORM(GL_CALL(glUniform3i(Get_Uniform_Location(location), v0.x, v0.y, v0.z)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::ivec4& v0)
{
    SET_UNIFORM(GL_CALL(glUniform4i(Get_Uniform_Location(location), v0.x, v0.y, v0.z, v0.w)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::uvec2& v0)
{
    SET_UNIFORM(GL_CALL(glUniform2ui(Get_Uniform_Location(location), v0.x, v0.y)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::uvec3& v0)
{
    SET_UNIFORM(GL_CALL(glUniform3ui(Get_Uniform_Location(location), v0.x, v0.y, v0.z)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::uvec4& v0)
{
    SET_UNIFORM(GL_CALL(glUniform4ui(Get_Uniform_Location(location), v0.x, v0.y, v0.z, v0.w)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const size_t& count, const float* v0)
{
    SET_UNIFORM(GL_CALL(glUniform1fv(Get_Uniform_Location(location), static_cast<GLint>(count), v0)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const size_t& count, const int32_t* v0)
{
    SET_UNIFORM(GL_CALL(glUniform1iv(Get_Uniform_Location(location), static_cast<GLint>(count), v0)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const size_t& count, const uint32_t* v0)
{
    SET_UNIFORM(GL_CALL(glUniform1uiv(Get_Uniform_Location(location), static_cast<GLint>(count), v0)));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::mat2& v0)
{
    SET_UNIFORM(GL_CALL(glUniformMatrix2fv(Get_Uniform_Location(location), 1, GL_FALSE, &v0[0][0])));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::mat3& v0)
{
    SET_UNIFORM(GL_CALL(glUniformMatrix3fv(Get_Uniform_Location(location), 1, GL_FALSE, &v0[0][0])));
}

void tilia::gfx::Shader::Uniform(const std::string& location, const glm::mat4& v0)
{
    SET_UNIFORM(GL_CALL(glUniformMatrix4fv(Get_Uniform_Location(location), 1, GL_FALSE, &v0[0][0])));
}

void tilia::gfx::Shader::Uniform(const Shader_Data& shader_data)
{

    for (auto it{ shader_data.uniform_variables.begin() }; it != shader_data.uniform_variables.end(); ++it)
    {
        switch (it->second.type)
        {

        case Uniform_Type::Float:
            switch (it->second.amount)
            {
            case 1:
                Uniform(it->first, reinterpret_cast<float*>(it->second.data)[0]);
                break;
            case 2:
                Uniform(it->first, reinterpret_cast<float*>(it->second.data)[0], reinterpret_cast<float*>(it->second.data)[1]);
                break;
            case 3:
                Uniform(it->first, reinterpret_cast<float*>(it->second.data)[0], reinterpret_cast<float*>(it->second.data)[1], reinterpret_cast<float*>(it->second.data)[2]);
                break;
            case 4:
                Uniform(it->first, reinterpret_cast<float*>(it->second.data)[0], reinterpret_cast<float*>(it->second.data)[1], reinterpret_cast<float*>(it->second.data)[2], reinterpret_cast<float*>(it->second.data)[3]);
                break;
            }
            break;

        case Uniform_Type::Int:
            switch (it->second.amount)
            {
            case 1:
                Uniform(it->first, reinterpret_cast<int32_t*>(it->second.data)[0]);
                break;
            case 2:
                Uniform(it->first, reinterpret_cast<int32_t*>(it->second.data)[0], reinterpret_cast<int32_t*>(it->second.data)[1]);
                break;
            case 3:
                Uniform(it->first, reinterpret_cast<int32_t*>(it->second.data)[0], reinterpret_cast<int32_t*>(it->second.data)[1], reinterpret_cast<int32_t*>(it->second.data)[2]);
                break;
            case 4:
                Uniform(it->first, reinterpret_cast<int32_t*>(it->second.data)[0], reinterpret_cast<int32_t*>(it->second.data)[1], reinterpret_cast<int32_t*>(it->second.data)[2], reinterpret_cast<int32_t*>(it->second.data)[3]);
                break;
            }
            break;

        case Uniform_Type::Uint:
            switch (it->second.amount)
            {
            case 1:
                Uniform(it->first, reinterpret_cast<uint32_t*>(it->second.data)[0]);
                break;
            case 2:
                Uniform(it->first, reinterpret_cast<uint32_t*>(it->second.data)[0], reinterpret_cast<uint32_t*>(it->second.data)[1]);
                break;
            case 3:
                Uniform(it->first, reinterpret_cast<uint32_t*>(it->second.data)[0], reinterpret_cast<uint32_t*>(it->second.data)[1], reinterpret_cast<uint32_t*>(it->second.data)[2]);
                break;
            case 4:
                Uniform(it->first, reinterpret_cast<uint32_t*>(it->second.data)[0], reinterpret_cast<uint32_t*>(it->second.data)[1], reinterpret_cast<uint32_t*>(it->second.data)[2], reinterpret_cast<uint32_t*>(it->second.data)[3]);
                break;
            }
            break;

        case Uniform_Type::Vec:
            switch (it->second.amount)
            {
            case 2:
                Uniform(it->first, *reinterpret_cast<glm::vec2*>(it->second.data));
                break;
            case 3:
                Uniform(it->first, *reinterpret_cast<glm::vec3*>(it->second.data));
                break;
            case 4:
                Uniform(it->first, *reinterpret_cast<glm::vec4*>(it->second.data));
                break;
            }
            break;

        case Uniform_Type::Ivec:
            switch (it->second.amount)
            {
            case 2:
                Uniform(it->first, *reinterpret_cast<glm::ivec2*>(it->second.data));
                break;
            case 3:
                Uniform(it->first, *reinterpret_cast<glm::ivec3*>(it->second.data));
                break;
            case 4:
                Uniform(it->first, *reinterpret_cast<glm::ivec4*>(it->second.data));
                break;
            }
            break;

        case Uniform_Type::Uvec:
            switch (it->second.amount)
            {
            case 2:
                Uniform(it->first, *reinterpret_cast<glm::uvec2*>(it->second.data));
                break;
            case 3:
                Uniform(it->first, *reinterpret_cast<glm::uvec3*>(it->second.data));
                break;
            case 4:
                Uniform(it->first, *reinterpret_cast<glm::uvec4*>(it->second.data));
                break;
            }
            break;

        case Uniform_Type::Float_Arr:
            Uniform(it->first, it->second.amount, reinterpret_cast<float*>(it->second.data));
            break;

        case Uniform_Type::Int_Arr:
            Uniform(it->first, it->second.amount, reinterpret_cast<int32_t*>(it->second.data));
            break;

        case Uniform_Type::Uint_Arr:
            Uniform(it->first, it->second.amount, reinterpret_cast<uint32_t*>(it->second.data));
            break;

        case Uniform_Type::Mat:
            switch (it->second.amount)
            {
            case 2:
                Uniform(it->first, *reinterpret_cast<glm::mat2*>(it->second.data));
                break;
            case 3:
                Uniform(it->first, *reinterpret_cast<glm::mat3*>(it->second.data));
                break;
            case 4:
                Uniform(it->first, *reinterpret_cast<glm::mat4*>(it->second.data));
                break;
            }
            break;

        }
    }

}

#endif // 0

static void Check_Shader(const uint32_t& type, const std::uint32_t& id) {

    std::int32_t result;
    GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        std::int32_t length;
        GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        std::vector<char> message(length * sizeof(char));

        GL_CALL(glGetShaderInfoLog(id, length, &length, &message.front()));
        message[length - 1] = '\0';

        tilia::utils::Tilia_Exception e{ LOCATION };

        e.Add_Message("Failed to create shader { type: %v }"
            "\n>>> Message: %v"
            )(type)
            (static_cast<const char*>(&message.front()));

        throw e;

        GL_CALL(glDeleteShader(id));
    }

}

static std::uint32_t Compile_Shader(const uint32_t& type, const tilia::gfx::Shader_Type& shader) {
    
    std::uint32_t id{};
    GL_CALL(id = glCreateShader(type));
    const char* src = shader.source.c_str();
    
    GL_CALL(glShaderSource(id, 1, &src, nullptr));
    
    GL_CALL(glCompileShader(id));

    Check_Shader(type, id);

    return id;

}



tilia::gfx::Shader::Shader(const Shader_Type& vertex, const Shader_Type& fragment, const Shader_Type& geometry = {})
    : Shader()
{

    std::uint32_t 
        v_id{ Compile_Shader(*enums::Shader_Type::Vertex, vertex) }, 
        f_id{ Compile_Shader(*enums::Shader_Type::Fragment, fragment) },
        g_id{};

    if (geometry.path != "")
        g_id = Compile_Shader(*enums::Shader_Type::Geomentry, geometry);



}

tilia::gfx::Shader::Shader(){

    try
    {
        GL_CALL_MESSAGE("Main program", m_ID = glCreateProgram());
    }
    catch (utils::Tilia_Exception& e)
    {

        e.Add_Message("Shader failed to be generated");

        // Possibly forward e to someplace else and then throw

    }

}
