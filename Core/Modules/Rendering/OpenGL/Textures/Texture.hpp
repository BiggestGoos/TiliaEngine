/**************************************************************************************************
 * @file   Texture.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   08/04/2023
 *************************************************************************************************/

#ifndef TILIA_TEXTURE_HPP
#define TILIA_TEXTURE_HPP

// Standard
#include <cstdint>
#include <unordered_map>

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_CONSTANTS_INCLUDE

namespace tilia
{

	namespace texturing
	{

		class Texture;

		namespace enums
		{
			enum class Type
			{
				Two_D    = 0x0DE1,
				Cube_Map = 0x8513
			}; // Type

			enum class Filter
			{
				Nearest = 0x2600,
				Linear = 0x2601
			}; // Filter

			enum class Wrapping
			{
				Repeat			= 0x2901,
				Mirror_Repeat   = 0x8370,
				Clamp_To_Edge   = 0x812F,
				Clamp_To_Border = 0x812D
			};

			enum class Property
			{
				ID		   = 0x0000,
				Type	   = 0x0001,
				Min_Filter = 0x0002,
				Mag_Filter = 0x0003,
				Wrap_S     = 0x0004,
				Wrap_T     = 0x0005,
				Wrap_R     = 0x0006
			}; // Property

		} // enums

		namespace properties
		{

			template<enums::Property Type>
			struct Texture_Property {};

			template<>
			struct Texture_Property<enums::Property::Min_Filter>
			{
				using Set_Parameters = enums::Filter;
				using Get_Parameters = enums::Filter;
				static void Set(Texture& texture, Set_Parameters&& parameters);
				static Get_Parameters Get(Texture& texture);
			};

			template<>
			struct Texture_Property<enums::Property::Mag_Filter>
			{
				using Set_Parameters = enums::Filter;
				using Get_Parameters = enums::Filter;
				static void Set(Texture& texture, Set_Parameters&& parameters);
				static Get_Parameters Get(Texture& texture);
			};

			template<>
			struct Texture_Property<enums::Property::Wrap_S>
			{
				using Set_Parameters = enums::Wrapping;
				using Get_Parameters = enums::Wrapping;
				static void Set(Texture& texture, Set_Parameters&& parameters);
				static Get_Parameters Get(Texture& texture);
			};

			template<>
			struct Texture_Property<enums::Property::Wrap_T>
			{
				using Set_Parameters = enums::Wrapping;
				using Get_Parameters = enums::Wrapping;
				static void Set(Texture& texture, Set_Parameters&& parameters);
				static Get_Parameters Get(Texture& texture);
			};

			template<>
			struct Texture_Property<enums::Property::Wrap_R>
			{
				using Set_Parameters = enums::Wrapping;
				using Get_Parameters = enums::Wrapping;
				static void Set(Texture& texture, Set_Parameters&& parameters);
				static Get_Parameters Get(Texture& texture);
			};

		} // properties

		class Texture
		{
		public:

			template<enums::Property Type, typename Property = properties::Texture_Property<Type>>
			void Set(typename Property::Set_Parameters parameters)
			{
				Property::Set(*this, std::move(parameters));
			}

			template<enums::Property Type, typename Property = properties::Texture_Property<Type>>
			auto Get()
			{
				return Property::Get(*this);
			}

			template<>
			auto Get<enums::Property::ID>()
			{
				return m_ID;
			}

			template<>
			auto Get<enums::Property::Type>()
			{
				return m_type;
			}

			static void Bind(std::uint16_t slot, enums::Type type, std::uint32_t id, bool save_id = false);

			inline void Bind(std::uint16_t slot, bool save_id = false) const {
				Bind(slot, m_type, m_ID, save_id);
			}

			static void Unbind(std::uint16_t slot, enums::Type type, bool save_id = false);

			inline void Unbind(std::uint16_t slot, bool save_id = false) const {
				Unbind(slot, m_type, save_id);
			}

			static void Rebind(std::uint16_t slot);

		protected:

			enums::Type m_type{};

			std::uint32_t m_ID{};

			Texture(enums::Type type) : m_type{ type } { }

		private:

			static std::unordered_map<std::uint16_t, std::pair<enums::Type, std::uint32_t>> s_bound_textures;
			static std::unordered_map<std::uint16_t, std::pair<enums::Type, std::uint32_t>> s_saved_textures;

		public:

			Texture() = default;

		}; // Texture

	} // texturing

} // tilia

#endif // TILIA_TEXTURE_HPP