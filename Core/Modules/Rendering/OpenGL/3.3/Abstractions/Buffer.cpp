// Vendor
#include "vendor/glad/include/glad/glad.h"

// Tilia
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Buffer.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"

// Defines static variables
std::map<tilia::enums::Buffer_Type, std::uint32_t> tilia::gfx::Buffer::s_bound_IDs{ { enums::Buffer_Type::Vertex_Buffer, 0 }, { enums::Buffer_Type::Element_Buffer, 0 }, { enums::Buffer_Type::Uniform_Buffer, 0 } };
std::map<tilia::enums::Buffer_Type, std::uint32_t> tilia::gfx::Buffer::s_saved_IDs{ { enums::Buffer_Type::Vertex_Buffer, 0 }, { enums::Buffer_Type::Element_Buffer, 0 }, { enums::Buffer_Type::Uniform_Buffer, 0 } };

void tilia::gfx::Buffer::Init(const enums::Buffer_Type& type, const std::size_t& size, const enums::Buffer_Access_Type& access_type, const enums::Buffer_Access_Frequency& access_frequency, const void* data, const bool& allocate_local)
{
    // We store the given type, generate an openGL buffer which gives us an id and then if a size is given then we allocate memory with that size with given parameters for what type of access we want etc
    m_type = type;
    GL_CALL(glGenBuffers(*m_type, &m_ID));
    if (size)
        Allocate(size, access_type, access_frequency, data, allocate_local);
}

void tilia::gfx::Buffer::Terminate()
{
    // We delete the openGL buffer of type with id
    GL_CALL(glDeleteBuffers(*m_type, &m_ID));
}

void tilia::gfx::Buffer::Allocate(const std::size_t& size, const enums::Buffer_Access_Type& access_type, const enums::Buffer_Access_Frequency& access_frequency, const void* data, const bool& allocate_local)
{
    // If this is not already bound then we bind it
    if (m_ID != s_bound_IDs[m_type])
    {
        Unbind(true);
        Bind();
    } 
    // We calculate the openGL usage
    std::uint32_t usage{ static_cast<std::uint32_t>(*access_type + *access_frequency) };
    // We allocate memory of size for this buffer of type with the potential data and the usage
    // If data is nullptr then we will just allocate an undefined block of memory of size
    GL_CALL(glBufferData(*m_type, size, data, usage));
    m_memory_size = size;
    m_access_type = access_type;
    m_access_frequency = access_frequency;
    // If we want to then we allocate memory for the local buffer, otherwise we set it to nullptr in order to show that we  don't have a local buffer
    if (allocate_local)
    {
        Allocate_Local(m_memory_size, data);
    }
    else
    {
        m_local_data = nullptr;
    }
    // If we just bound this then we rebind the old one
    if (m_ID == s_bound_IDs[m_type] && m_ID != s_saved_IDs[m_type])
        Rebind();
}

void tilia::gfx::Buffer::Allocate_Local(const std::size_t& size, const void* data)
{
    // Allocates local memory for temporarily storing data before uploading it to openGL
    m_local_data = std::make_unique<Byte[]>(size);
    // If given data then we copy it into our local buffer
    if (data)
        std::memcpy(m_local_data.get(), data, size);
}

void tilia::gfx::Buffer::Set_Data(const std::size_t& offset, const std::size_t& size, const void* data, const bool& set_local)
{
    // If we choose to then we set the data in our local buffer for later uploading
    if (set_local)
    {
        // If our local buffer isn't already allocated then we do so before copying data to it
        if (!m_local_data)
            Allocate_Local(m_memory_size);
        // We copy the given data into our local buffer
        std::memcpy(m_local_data.get() + offset, data, (size)? size : m_memory_size);
    }
    // If auto upload isn't true then we just return. This can allow for this function to be called without effect if we don't set the local buffer and auto upload is false
    if (!m_auto_upload)
        return;
    // If this is not already bound then we bind it
    if (m_ID != s_bound_IDs[m_type])
    {
        Unbind(true);
        Bind();
    }
    // We set the data in the already allocated memory for the buffer of type with the given data between the offset and offset + size.
    GL_CALL(glBufferSubData(*m_type, offset, (size)? size : m_memory_size, data));
    // If we just bound this then we rebind the old one
    if (m_ID == s_bound_IDs[m_type] && m_ID != s_saved_IDs[m_type])
        Rebind();
}

void tilia::gfx::Buffer::Get_Data(const std::size_t& offset, const std::size_t& size, void* data, const bool& get_local)
{
    // If we choose to then we get the local data instead of getting the openGL data
    if (get_local)
    {
        // If our local buffer is nullptr then we instead just set the data to 0
        if (!m_local_data) 
        {
            std::memset(static_cast<Byte*>(data) + offset, 0, (size)? size : m_memory_size);
            // Early return
            return;
        }
        // We copy the data from the local buffer to our return data
        std::memcpy(static_cast<Byte*>(data) + offset, m_local_data.get(), (size)? size : m_memory_size);
        // Early return
        return;
    }
    // If this is not already bound then we bind it
    if (m_ID != s_bound_IDs[m_type])
    {
        Unbind(true);
        Bind();
    }
    // We get the data contained in the currently bound buffer of type within the offset and size of the memory.
    GL_CALL(glGetBufferSubData(*m_type, offset, (size)? size : m_memory_size, data));
    // If we just bound this then we rebind the old one
    if (m_ID == s_bound_IDs[m_type] && m_ID != s_saved_IDs[m_type])
        Rebind();
}

void tilia::gfx::Buffer::Map_Data(const enums::Buffer_Map_Type& map_type, void*& data, const bool& map_local)
{
    if (map_local)
    {
        // If our local buffer isn't already allocated then we do so before copying data to it
        if (!m_local_data)
            Allocate_Local(m_memory_size);
        data = m_local_data.get();
        // Early return
        return;
    }
    // If this is not already bound then we bind it
    if (m_ID != s_bound_IDs[m_type])
    {
        Unbind(true);
        Bind();
    }
    if (map_type != enums::Buffer_Map_Type::None && data != nullptr)
    {
        GL_CALL(data = glMapBuffer(*m_type, *map_type));
        m_map_type = map_type;
    }
    else
    {

    }
    // If we just bound this then we rebind the old one
    if (m_ID == s_bound_IDs[m_type] && m_ID != s_saved_IDs[m_type])
        Rebind();
}

void tilia::gfx::Buffer::Map_Data(const enums::Buffer_Map_Type& map_type, const std::size_t& offset, const std::size_t& size, void*& data, const bool& map_local)
{
}

void tilia::gfx::Buffer::Unmap_Data()
{
    void* temp{ nullptr };
    Map_Data(enums::Buffer_Map_Type::None, temp);
}

void tilia::gfx::Buffer::Bind() const {
    // If id is zero then we throw exception
    if (!m_ID)
    {
        utils::Tilia_Exception e{ LOCATION };
        e.Add_Message("Failed to bind buffer { Type: %v : ID: %v }"
        )(*m_type)(m_ID);
        throw e;
    }
    // We bind buffer of type with id
    GL_CALL(glBindBuffer(*m_type, m_ID));
    // We store the id of type as the new bound id
    s_bound_IDs[m_type] = m_ID;
}

void tilia::gfx::Buffer::Bind(const enums::Buffer_Type& type, const std::uint32_t& id) {
    // If id is zero then we throw exception
    if (!id)
    {
        utils::Tilia_Exception e{ LOCATION };
        e.Add_Message("Failed to bind buffer { Type: %v : ID: %v }"
        )(*type)(id);
        throw e;
    }
    // We bind buffer of type with id
    GL_CALL(glBindBuffer(*type, id));
    // We store the id of type as the new bound id
    s_bound_IDs[type] = id;
}

void tilia::gfx::Buffer::Unbind(const bool& save_id) {
    // We bind the openGL buffer of type to zero which effectively unbinds the previously bound buffer of type.
    GL_CALL(glBindBuffer(*m_type, 0));
    // If true then we save the old bound id
    if (save_id)
    {
        s_saved_IDs[m_type] = s_bound_IDs[m_type];
    }
    s_bound_IDs[m_type] = 0;
}

void tilia::gfx::Buffer::Unbind(const enums::Buffer_Type& type, const bool& save_id)
{
    // We bind the openGL buffer of type to zero which effectively unbinds the previously bound buffer of type.
    GL_CALL(glBindBuffer(*type, 0));
    // If true then we save the old bound id
    if (save_id)
    {
        s_saved_IDs[type] = s_bound_IDs[type];
    }
    s_bound_IDs[type] = 0;
}

void tilia::gfx::Buffer::Rebind() {
    // We bind the previously bound id
    GL_CALL(glBindBuffer(*m_type, s_saved_IDs[m_type]));
    // We set the current bound id to the saved id
    s_bound_IDs[m_type] = s_saved_IDs[m_type];
    s_saved_IDs[m_type] = 0;
}

void tilia::gfx::Buffer::Rebind(const enums::Buffer_Type& type)
{
    // We bind the previously bound id
    GL_CALL(glBindBuffer(*type, s_saved_IDs[type]));
    // We set the current bound id to the saved id
    s_bound_IDs[type] = s_saved_IDs[type];
    s_saved_IDs[type] = 0;
}
