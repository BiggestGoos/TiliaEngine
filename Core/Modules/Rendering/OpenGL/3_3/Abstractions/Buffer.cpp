// Vendor
#include "vendor/glad/include/glad/glad.h"

// Tilia
#include "Buffer.hpp"
#include "Core/Values/Directories.hpp"
#include TILIA_OPENGL_3_3_ERROR_HANDLING_INCLUDE
#include TILIA_TILIA_EXCEPTION_INCLUDE

std::unordered_map<std::uint32_t, std::uint32_t> tilia::gfx::Buffer::s_bound_IDs{ {
        *enums::Buffer_Type::Vertex_Buffer, 0 }, { *enums::Buffer_Type::Element_Buffer, 0 }, { 
            *enums::Buffer_Type::Uniform_Buffer, 0 } };

std::unordered_map<std::uint32_t, std::uint32_t> tilia::gfx::Buffer::s_saved_IDs{ {
        *enums::Buffer_Type::Vertex_Buffer, 0 }, { *enums::Buffer_Type::Element_Buffer, 0 }, { 
            *enums::Buffer_Type::Uniform_Buffer, 0 } };

tilia::gfx::Buffer::Buffer(const Buffer& other)
{
    // We initialize this buffer with other buffers resources
    Init(other.m_type, other.m_memory_size, other.m_access_type, other.m_access_frequency,
        other.m_local_data.get(), other.m_local_data.get() != nullptr);
}

tilia::gfx::Buffer::Buffer(Buffer&& other) noexcept
    : m_ID{ other.m_ID }, m_local_data{ std::move(other.m_local_data) }, 
    m_memory_size{ other.m_memory_size }, m_type{ other.m_type }, 
    m_access_type{ other.m_access_type }, m_access_frequency{ other.m_access_frequency },
    m_map_type{ other.m_map_type }
{
    // Show that we can't use other buffer after moving
    other.m_ID = 0;
    other.m_memory_size = 0;
    other.m_map_type = enums::Buffer_Map_Type::None;
}

tilia::gfx::Buffer& tilia::gfx::Buffer::operator=(const Buffer& other)
{
    // Self check
    if (this == &other)
        return *this;

    Terminate();
    // We initialize this buffer with other buffers resources
    Init(other.m_type, other.m_memory_size, other.m_access_type, other.m_access_frequency,
        other.m_local_data.get(), other.m_local_data.get() != nullptr);

    return *this;
}

tilia::gfx::Buffer& tilia::gfx::Buffer::operator=(Buffer&& other) noexcept
{
    // Self check
    if (this == &other)
        return *this;

    // We terminate old buffer
    Terminate();

    m_ID = other.m_ID;
    m_local_data = std::move(other.m_local_data);
    m_memory_size = other.m_memory_size;
    m_map_type = other.m_map_type;
    m_type = other.m_type;
    m_access_type = other.m_access_type;
    m_access_frequency = other.m_access_frequency;
    m_map_type = other.m_map_type;
    // Show that we can't use other buffer after moving
    other.m_ID = 0;
    other.m_memory_size = 0;
    other.m_map_type = enums::Buffer_Map_Type::None;

    return *this;
}

void tilia::gfx::Buffer::Init(const enums::Buffer_Type& type, const std::size_t& size,
    const enums::Buffer_Access_Type& access_type, 
    const enums::Buffer_Access_Frequency& access_frequency, const void* data, 
    const bool& allocate_local)
{
    m_type = type;
    GL_CALL(glGenBuffers(1, &m_ID));
    if (size > 0)
        Allocate(size, access_type, access_frequency, data, allocate_local);
}

void tilia::gfx::Buffer::Terminate()
{
    // We deallocate the buffers
    GL_CALL(glDeleteBuffers(0, &m_ID));
    m_local_data.reset();
    // Show that we can't use this buffer after terminating
    m_ID = 0;
    m_memory_size = 0;
    m_map_type = enums::Buffer_Map_Type::None;
}

void tilia::gfx::Buffer::Allocate(const std::size_t& size, 
    const enums::Buffer_Access_Type& access_type, 
    const enums::Buffer_Access_Frequency& access_frequency, const void* data, 
    const bool& allocate_local)
{
    const bool was_bound{ m_ID == s_bound_IDs[*m_type] };
    if (!was_bound)
        Bind(true);

    // We calculate the openGL usage
    const std::uint32_t usage{ static_cast<std::uint32_t>(*access_type + *access_frequency) };

    // Allocate and set data
    GL_CALL(glBufferData(*m_type, size, data, usage));
    m_memory_size = size;
    m_access_type = access_type;
    m_access_frequency = access_frequency;

    if (allocate_local)
        Allocate_Local(m_memory_size, data);
    else
        m_local_data = nullptr;

    const bool is_bound{ m_ID == s_bound_IDs[*m_type] };
    if (is_bound && !was_bound)
        Rebind();
}

void tilia::gfx::Buffer::Allocate_Local(const std::size_t& size, const void* data)
{
    m_local_data = std::make_unique<Byte[]>(size);
    if (data != nullptr)
        std::memcpy(m_local_data.get(), data, size);
}

void tilia::gfx::Buffer::Set_Data(const std::size_t& offset, const std::size_t& size, 
    const void* data, const bool& set_local)
{
    if (set_local)
    {
        if (m_local_data == nullptr)
            Allocate_Local(m_memory_size);
        std::memcpy(m_local_data.get() + offset, data, size);
        return;
    }

    const bool was_bound{ m_ID == s_bound_IDs[*m_type] };
    if (!was_bound)
        Bind(true);

    // Sets data at the given offset and size
    GL_CALL(glBufferSubData(*m_type, offset, size, data));

    const bool is_bound{ m_ID == s_bound_IDs[*m_type] };
    if (is_bound && !was_bound)
        Rebind();
}

void tilia::gfx::Buffer::Get_Data(const std::size_t& offset, const std::size_t& size, void* data, 
    const bool& get_local)
{
    if (get_local)
    {
        if (m_local_data == nullptr) 
            Allocate_Local(m_memory_size);
        std::memcpy(static_cast<Byte*>(data), m_local_data.get() + offset, size);
        return;
    }
    const bool was_bound{ m_ID == s_bound_IDs[*m_type] };
    if (!was_bound)
        Bind(true);

    // Gets data at the given offset and size
    GL_CALL(glGetBufferSubData(*m_type, offset, size, data));

    const bool is_bound{ m_ID == s_bound_IDs[*m_type] };
    if (is_bound && !was_bound)
        Rebind();
}

void tilia::gfx::Buffer::Map_Data(const enums::Buffer_Map_Type& mapping_type, void*& data, 
    const bool& map_local)
{
    if (map_local)
    {
        if (m_local_data == nullptr)
            Allocate_Local(m_memory_size);
        data = m_local_data.get();
        m_map_type = enums::Buffer_Map_Type::Local;
        return;
    }
    const bool was_bound{ m_ID == s_bound_IDs[*m_type] };
    if (!was_bound)
        Bind(true);

    // Data is mapped to the openGL buffer. Ie. it points to the openGL buffer.
    GL_CALL(data = glMapBuffer(*m_type, *mapping_type));
    m_map_type = mapping_type;

    const bool is_bound{ m_ID == s_bound_IDs[*m_type] };
    if (is_bound && !was_bound)
        Rebind();
}

void tilia::gfx::Buffer::Unmap_Data()
{
    const bool was_bound{ m_ID == s_bound_IDs[*m_type] };
    if (!was_bound)
        Bind(true);

    GL_CALL(glUnmapBuffer(*m_type));
    m_map_type = enums::Buffer_Map_Type::None;

    const bool is_bound{ m_ID == s_bound_IDs[*m_type] };
    if (is_bound && !was_bound)
        Rebind();
}

void tilia::gfx::Buffer::Upload_Data() const
{
    if (m_local_data == nullptr)
    {
        utils::Tilia_Exception e{ LOCATION };
        e.Add_Message("Failed to upload local buffer to openGL due to it being nullptr "
            "{ Type: %v : ID: %v }")(*m_type)(m_ID);
        throw e;
    }
    const bool was_bound{ m_ID == s_bound_IDs[*m_type] };
    if (!was_bound)
        Bind(true);

    // We upload the locally stored buffer to our openGL buffer
    GL_CALL(glBufferSubData(*m_type, 0, m_memory_size, m_local_data.get()));

    const bool is_bound{ m_ID == s_bound_IDs[*m_type] };
    if (is_bound && !was_bound)
        Rebind();
}

void tilia::gfx::Buffer::Bind(const enums::Buffer_Type& type, const std::uint32_t& id, 
    const bool& save_id) 
{
    if (id == 0)
    {
        utils::Tilia_Exception e{ LOCATION };
        e.Add_Message("Failed to bind buffer { Type: %v : ID: %v }"
        )(*type)(id);
        throw e;
    }
    GL_CALL(glBindBuffer(*type, id));
    if (save_id)
    {
        // We save the previously bound id of type
        s_saved_IDs[*type] = s_bound_IDs[*type];
    }
    // We store the id as the new bound id of type
    s_bound_IDs[*type] = id;
}

void tilia::gfx::Buffer::Unbind(const enums::Buffer_Type& type, const bool& save_id)
{
    // Effectively unbinds buffer of type
    GL_CALL(glBindBuffer(*type, 0));
    if (save_id)
    {
        // We save the previously bound id of type
        s_saved_IDs[*type] = s_bound_IDs[*type];
    }
    // We store 0 as the new bound id of type
    s_bound_IDs[*type] = 0;
}

void tilia::gfx::Buffer::Rebind(const enums::Buffer_Type& type)
{
    // We bind the saved id of type
    GL_CALL(glBindBuffer(*type, s_saved_IDs[*type]));

    s_bound_IDs[*type] = s_saved_IDs[*type];
    s_saved_IDs[*type] = 0;
}

#if TILIA_UNIT_TESTS == 1

#include "vendor/Catch2/Catch2.hpp"

#include <iostream>

void tilia::gfx::OpenGL_3_3_Buffer_Test()
{

    Buffer buffer{};

    buffer.Init(enums::Buffer_Type::Element_Buffer, 8, enums::Buffer_Access_Type::Draw, 
        enums::Buffer_Access_Frequency::Dynamic, nullptr, true);

    std::int64_t int_64{ 10 };

    buffer.Set_Data(&int_64, true);
    buffer.Upload_Data();

    void* int_64_ret_ptr{ new std::int64_t{} };

    buffer.Get_Data(int_64_ret_ptr, false);
    REQUIRE(*static_cast<int64_t*>(int_64_ret_ptr) == int_64);

    buffer.Get_Data(int_64_ret_ptr, true);
    REQUIRE(*static_cast<int64_t*>(int_64_ret_ptr) == int_64);



    buffer.Terminate();

}

#endif // TILIA_UNIT_TESTS == 1