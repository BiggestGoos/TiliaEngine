// Vendor
#include "vendor/glad/KHR_Debug_openGL_3_3/include/glad/glad.h"

// Tilia
#include "Texture.hpp"
#include TILIA_TILIA_EXCEPTION_INCLUDE
#include TILIA_OPENGL_3_3_ERROR_HANDLING_INCLUDE

void tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Min_Filter>::Set(Texture& texture, Set_Parameters&& parameters)
{
    texture.Bind(0, true);
    glTexParameteri(*texture.Get<enums::Property::Type>(), GL_TEXTURE_MIN_FILTER, *parameters);
    texture.Rebind(0);
}

tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Min_Filter>::Get_Parameters tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Min_Filter>::Get(Texture& texture)
{
    texture.Bind(0, true);
    std::int32_t ret_val{};
    glGetTexParameteriv(*texture.Get<enums::Property::Type>(), GL_TEXTURE_MIN_FILTER, &ret_val);
    texture.Rebind(0);
    return static_cast<enums::Filter>(ret_val);
}

void tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Mag_Filter>::Set(Texture& texture, Set_Parameters&& parameters)
{
    texture.Bind(0, true);
    glTexParameteri(*texture.Get<enums::Property::Type>(), GL_TEXTURE_MAG_FILTER, *parameters);
    texture.Rebind(0);
}

tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Mag_Filter>::Get_Parameters tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Mag_Filter>::Get(Texture& texture)
{
    texture.Bind(0, true);
    std::int32_t ret_val{};
    glGetTexParameteriv(*texture.Get<enums::Property::Type>(), GL_TEXTURE_MAG_FILTER, &ret_val);
    texture.Rebind(0);
    return static_cast<enums::Filter>(ret_val);
}

void tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_S>::Set(Texture& texture, Set_Parameters&& parameters)
{
    texture.Bind(0, true);
    glTexParameteri(*texture.Get<enums::Property::Type>(), GL_TEXTURE_WRAP_S, *parameters);
    texture.Rebind(0);
}

tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_S>::Get_Parameters tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_S>::Get(Texture& texture)
{
    texture.Bind(0, true);
    std::int32_t ret_val{};
    glGetTexParameteriv(*texture.Get<enums::Property::Type>(), GL_TEXTURE_WRAP_S, &ret_val);
    texture.Rebind(0);
    return static_cast<enums::Wrapping>(ret_val);
}

void tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_T>::Set(Texture& texture, Set_Parameters&& parameters)
{
    texture.Bind(0, true);
    glTexParameteri(*texture.Get<enums::Property::Type>(), GL_TEXTURE_WRAP_T, *parameters);
    texture.Rebind(0);
}

tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_T>::Get_Parameters tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_T>::Get(Texture& texture)
{
    texture.Bind(0, true);
    std::int32_t ret_val{};
    glGetTexParameteriv(*texture.Get<enums::Property::Type>(), GL_TEXTURE_WRAP_T, &ret_val);
    texture.Rebind(0);
    return static_cast<enums::Wrapping>(ret_val);
}

void tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_R>::Set(Texture& texture, Set_Parameters&& parameters)
{
    texture.Bind(0, true);
    glTexParameteri(*texture.Get<enums::Property::Type>(), GL_TEXTURE_WRAP_R, *parameters);
    texture.Rebind(0);
}

tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_R>::Get_Parameters tilia::texturing::properties::Texture_Property<tilia::texturing::enums::Property::Wrap_R>::Get(Texture& texture)
{
    texture.Bind(0, true);
    std::int32_t ret_val{};
    glGetTexParameteriv(*texture.Get<enums::Property::Type>(), GL_TEXTURE_WRAP_R, &ret_val);
    texture.Rebind(0);
    return static_cast<enums::Wrapping>(ret_val);
}

/* Texture class starts here */

std::unordered_map<std::uint16_t, std::pair<tilia::texturing::enums::Type, std::uint32_t>> tilia::texturing::Texture::s_bound_textures{};
std::unordered_map<std::uint16_t, std::pair<tilia::texturing::enums::Type, std::uint32_t>> tilia::texturing::Texture::s_saved_textures{};

void tilia::texturing::Texture::Bind(std::uint16_t slot, enums::Type type, std::uint32_t id, bool save_id)
{
    if (s_bound_textures[slot] == std::pair<enums::Type, std::uint32_t>{ type, id })
        return;
	if (id == 0)
	{
		throw utils::Tilia_Exception{ { TILIA_LOCATION, "Can't bind texture as ID is 0" } };
	}
	GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	GL_CALL(glBindTexture(*type, id));
	if (save_id)
        s_saved_textures[slot] = s_bound_textures[slot];
	s_bound_textures[slot] = { type, id };
}

void tilia::texturing::Texture::Unbind(std::uint16_t slot, enums::Type type, bool save_id)
{
    if (s_bound_textures[slot] == std::pair<enums::Type, std::uint32_t>{ type, 0 })
        return;
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(*type, 0));
    if (save_id)
        s_saved_textures[slot] = s_bound_textures[slot];
    s_bound_textures[slot] = { type, 0 };
}

void tilia::texturing::Texture::Rebind(std::uint16_t slot)
{
    if (s_bound_textures[slot] == s_saved_textures[slot])
        return;
    const auto& [type, id] { s_saved_textures[slot] };
    GL_CALL(glBindBuffer(*type, id));
    s_bound_textures[slot] = s_saved_textures[slot];
    s_saved_textures[slot] = { type, 0 };
}
