// Standard
#include <stdexcept>
#include <cmath>
#include <iostream>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture_files/Cube_Map_Data.hpp"
#include "Core/Modules/File_System/Windows/File_System.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"
#include "Core/Modules/Console/Logging.hpp"
#include "Core/Values/OpenGL/3_3/Utils.hpp"

// The file system defined in another file
extern tilia::utils::File_System file_system;

tilia::gfx::Cube_Map_Data& tilia::gfx::Cube_Map_Data::operator=(const Cube_Map_Data& other) noexcept
{
    // Self check
    if (&other == this)
        return *this;

    // Copies the data for each side.
    constexpr std::size_t side_count{ *enums::Misc::Cube_Sides };
    for (std::size_t i = 0; i < side_count; i++)
    {
        this->sides[i].file_path = other.sides[i].file_path;
        // If there is any, copies the data of that side
        if (other.sides[i].texture_data) {
            uint32_t byte_count{ static_cast<uint32_t>((powf(static_cast<float>(this->size), 2.0f) *
            utils::Get_Color_Format_Count(static_cast<uint32_t>(*other.sides[i].data_color_format)))) };
            this->Copy_Data(i, other.sides[i].texture_data.get(), byte_count);
        }
        this->sides[i].color_format = other.sides[i].color_format;
        this->sides[i].data_color_format = other.sides[i].data_color_format;
    }

    this->size = other.size;
    this->filter_min = other.filter_min;
    this->filter_mag = other.filter_mag;
    this->wrap_s = other.wrap_s;
    this->wrap_t = other.wrap_t;
    this->wrap_r = other.wrap_r;

    return *this;
}

tilia::gfx::Cube_Map_Data& tilia::gfx::Cube_Map_Data::operator=(Cube_Map_Data&& other) noexcept
{
    // Self check
    if (&other == this)
        return *this;

    // Moves the data for each side.
    constexpr std::size_t side_count{ *enums::Misc::Cube_Sides };
    for (std::size_t i = 0; i < side_count; i++)
    {
        this->sides[i].file_path = std::move(other.sides[i].file_path);
        // If there is any, moves the data of that side
        if (other.sides[i].texture_data) {
            this->sides[i].texture_data.reset(other.sides[i].texture_data.get());
            other.sides[i].texture_data.reset();
        }
        this->sides[i].color_format = other.sides[i].color_format;
        this->sides[i].data_color_format = other.sides[i].data_color_format;
    }

    this->size = other.size;
    this->filter_min = other.filter_min;
    this->filter_mag = other.filter_mag;
    this->wrap_s = other.wrap_s;
    this->wrap_t = other.wrap_t;
    this->wrap_r = other.wrap_r;

    return *this;
}

tilia::gfx::Cube_Map_Data::Cube_Map_Data(const Cube_Map_Data& other) noexcept
{
    *this = other;
}

tilia::gfx::Cube_Map_Data::Cube_Map_Data(Cube_Map_Data&& other) noexcept
{
    *this = std::move(other);
}

void tilia::gfx::Cube_Map_Data::Reload(const std::size_t& index)
{

    // Loads in the data from the stored path with the given index
    if (this->sides[index].file_path != "") {
        std::int32_t temp{}, format{};
        this->sides[index].texture_data.reset(file_system.Load_Image(this->sides[index].file_path.c_str(),
            temp, temp, format, 0, true));
        this->sides[index].data_color_format = utils::Get_Data_Color_Format(format);
    }
    else
    {

        utils::Tilia_Exception e{ LOCATION };

        e.Add_Message(
            "Failed to reload data"
            "\n>>> Side: %v"
        )(utils::Get_Cube_Map_Side_String(*enums::Cube_Map_Sides::Positive_X + static_cast<uint32_t>(index)));

        throw e;

    }

}

void tilia::gfx::Cube_Map_Data::Reload()
{

    // Loads in the data from the stored paths
    for (std::size_t i = 0; i < *enums::Misc::Cube_Sides; i++)
    {

        if (this->sides[i].file_path != "") {
            int32_t format{};
            this->sides[i].texture_data.reset(file_system.Load_Image(this->sides[i].file_path.c_str(),
                this->size, this->size, format, 0, true));
            this->sides[i].data_color_format = utils::Get_Data_Color_Format(format);
        }
        else
        {

            utils::Tilia_Exception e{ LOCATION };

            e.Add_Message(
                "Failed to reload data"
                "\n>>> Side: %v"
            )(utils::Get_Cube_Map_Side_String(*enums::Cube_Map_Sides::Positive_X + static_cast<uint32_t>(i)));

            throw e;

        }

    }

}

void tilia::gfx::Cube_Map_Data::Copy_Data(const std::size_t& index, uint8_t* texture_data, uint32_t byte_count)
{
    if (!byte_count) {
        // Calculates the byte count by taking the square of the size and multiplying by the number of color channels.
        byte_count = static_cast<uint32_t>((powf(static_cast<float>(this->size), 2.0f) *
            utils::Get_Color_Format_Count(static_cast<uint32_t>(*this->sides[index].color_format))));
    }

    // Allocates memory of size byte count and then copies the given data to that memory location.
    this->sides[index].texture_data = std::make_unique<uint8_t[]>(static_cast<size_t>(byte_count));
    std::copy(texture_data, texture_data + byte_count, this->sides[index].texture_data.get());
}
