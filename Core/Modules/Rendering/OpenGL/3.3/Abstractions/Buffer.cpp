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
    // If the given size is more than zero then we allocate memory of that size
    if (size)
        Allocate(size, access_type, access_frequency, data, allocate_local);
}

void tilia::gfx::Buffer::Terminate()
{
    // We delete the openGL buffer of type with id
    GL_CALL(glDeleteBuffers(*m_type, &m_ID));
    // We set the id to 0 in order to show that we deleted the openGL buffer
    m_ID = 0;
    // We reset the local buffer to show that we have terminated the buffer
    m_local_data.reset();
    // In order to show that we have deleted the buffers we also set the memory size to zero to showcase this
    m_memory_size = 0;
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
        std::memcpy(m_local_data.get() + offset, data, (size == 0 && offset == 0)? m_memory_size : size);
        // Early return
        return;
    }
    // If this is not already bound then we bind it
    if (m_ID != s_bound_IDs[m_type])
    {
        Unbind(true);
        Bind();
    }
    // We set the data in the already allocated memory for the buffer of type with the given data between the offset and offset + size.
    GL_CALL(glBufferSubData(*m_type, offset, (size == 0 && offset == 0)? m_memory_size : size, data));
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
            std::memset(static_cast<Byte*>(data) + offset, 0, (size == 0 && offset == 0)? m_memory_size : size);
            // Early return
            return;
        }
        // We copy the data from the local buffer to our return data
        std::memcpy(static_cast<Byte*>(data) + offset, m_local_data.get(), (size == 0 && offset == 0)? m_memory_size : size);
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
    GL_CALL(glGetBufferSubData(*m_type, offset, (size == 0 && offset == 0)? m_memory_size : size, data));
    // If we just bound this then we rebind the old one
    if (m_ID == s_bound_IDs[m_type] && m_ID != s_saved_IDs[m_type])
        Rebind();
}

void tilia::gfx::Buffer::Map_Data(const enums::Buffer_Map_Type& map_type, void*& data, const bool& map_local)
{
    // If we choose to then we map to our local memory instead of mapping to the openGL memory
    if (map_local)
    {
        // If our local buffer isn't already allocated then we do so before copying data to it
        if (!m_local_data)
            Allocate_Local(m_memory_size);
        // We set the data pointer to our local buffer
        data = m_local_data.get();
        // Since we don't map to an openGL memory block then we choose to say that we don't have a map type since that value is supposed to show in what way the openGL buffer has been mapped to
        m_map_type = enums::Buffer_Map_Type::None;
        // Early return
        return;
    }
    // If this is not already bound then we bind it
    if (m_ID != s_bound_IDs[m_type])
    {
        Unbind(true);
        Bind();
    }
    // If the given mapping type is not none then we map set the given data pointer to the mapped one which points to the openGL buffer memory
    if (map_type != enums::Buffer_Map_Type::None)
    {
        GL_CALL(data = glMapBuffer(*m_type, *map_type));
    }
    // If the mapping type is none then we unmap the buffer of type
    else
    {
        GL_CALL(glUnmapBuffer(*m_type));
    }
    // We store the map type which will indicate how the buffer is mapped
    m_map_type = map_type;
    // If we just bound this then we rebind the old one
    if (m_ID == s_bound_IDs[m_type] && m_ID != s_saved_IDs[m_type])
        Rebind();
}

void tilia::gfx::Buffer::Unmap_Data()
{
    // Since the mapping function takes a referene to a pointer we create a temporary object to satisfy it. We choose to call the mapping function here since it will unmap the buffer if the given mapping type is none
    void* temp{ nullptr };
    // As we already went through we call the mapping function in order to unmap the buffer
    Map_Data(enums::Buffer_Map_Type::None, temp);
}

void tilia::gfx::Buffer::Upload_Data() const
{
    // If data is nullptr then we throw exception
    if (!m_local_data)
    {
        utils::Tilia_Exception e{ LOCATION };
        e.Add_Message("Failed to upload local buffer to openGL due to the data being nullptr { Type: %v : ID: %v }")(*m_type)(m_ID);
        throw e;
    }
    // If this is not already bound then we bind it
    if (m_ID != s_bound_IDs[m_type])
    {
        Unbind(true);
        Bind();
    }
    // We upload the locally stored data to our openGL buffer
    GL_CALL(glBufferSubData(*m_type, 0, m_memory_size, m_local_data.get()));
    // If we just bound this then we rebind the old one
    if (m_ID == s_bound_IDs[m_type] && m_ID != s_saved_IDs[m_type])
        Rebind();
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

void tilia::gfx::Buffer::Rebind(const enums::Buffer_Type& type)
{
    // We bind the previously bound id
    GL_CALL(glBindBuffer(*type, s_saved_IDs[type]));
    // We set the current bound id to the saved id
    s_bound_IDs[type] = s_saved_IDs[type];
    s_saved_IDs[type] = 0;
}
