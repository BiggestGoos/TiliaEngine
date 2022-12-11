/**************************************************************************************************
 * @file   Uniform_Buffer.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   11/08/2022
 *************************************************************************************************/

// Vendor
#include "vendor/glm/include/glm/glm.hpp"
#include "vendor/glm/include/glm/gtc/type_ptr.hpp"

// Standard
#include <cstdint>
#include <initializer_list>
#include <utility>
#include <string>
#include <unordered_map>
#include <array>
#include <vector>

// Tilia
#include "Core/Values/OpenGL/3.3/Enums.hpp"

#ifndef TILIA_UNIFORM_BUFFER_HPP
#define TILIA_UNIFORM_BUFFER_HPP

namespace tilia
{
    
    namespace gfx
    {   
        
        class Uniform_Buffer
        {
        public:

			struct GLSL_Variable_
			{

				enums::GLSL_Scalar_Type scalar_type{};
				enums::GLSL_Container_Type container_type{};
				std::size_t array_count{};

				GLSL_Variable_(const enums::GLSL_Scalar_Type& type) : scalar_type{ type }, container_type{ enums::GLSL_Container_Type::Scalar }, array_count{ 0 } { }

				GLSL_Variable_(const enums::GLSL_Scalar_Type& type, const std::size_t& count) : scalar_type{ type }, container_type{ enums::GLSL_Container_Type::Scalar }, array_count{ count } { }

				GLSL_Variable_(const enums::GLSL_Scalar_Type& type, const enums::GLSL_Container_Type& c_type) : scalar_type{ type }, container_type{ c_type }, array_count{ 0 } { }

				GLSL_Variable_(const enums::GLSL_Scalar_Type& type, const enums::GLSL_Container_Type& c_type, const std::size_t& count) : scalar_type{ type }, container_type{ c_type }, array_count{ count } { }

			};

			class GLSL_Variable
			{
			public:

				GLSL_Variable() = default;

				GLSL_Variable(const enums::GLSL_Scalar_Type& scalar_type) : m_scalar_type{ scalar_type }, m_container_type{ enums::GLSL_Container_Type::Scalar } {}

				GLSL_Variable(const enums::GLSL_Scalar_Type& scalar_type, const enums::GLSL_Container_Type& container_type) : m_scalar_type{ scalar_type }, m_container_type{ container_type } {}

				GLSL_Variable(const enums::GLSL_Scalar_Type& scalar_type, const std::size_t& array_count) : m_scalar_type{ scalar_type }, m_container_type{ enums::GLSL_Container_Type::Scalar }, m_array_count{ array_count } {}

				GLSL_Variable(const enums::GLSL_Scalar_Type& scalar_type, const enums::GLSL_Container_Type& container_type, const std::size_t& array_count) : m_scalar_type{ scalar_type }, m_container_type{ container_type }, m_array_count{ array_count } {}

				inline enums::GLSL_Scalar_Type Get_Scalar_Type() const { return m_scalar_type; }
				inline enums::GLSL_Container_Type Get_Container_Type() const { return m_container_type; }
				inline std::size_t Get_Array_Count() const { return m_array_count; }
				
			private:

				enums::GLSL_Scalar_Type m_scalar_type{};
				enums::GLSL_Container_Type m_container_type{};

				std::size_t m_array_count{};

			};

            void Init(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables);

			void Reset(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables);

            void debug_print();
            
            inline auto Get_ID() const {
				return m_ID;
			}

            void Set_Bind_Point(const std::uint32_t& bind_point);

            inline auto Get_Bind_Point() const {
                return m_bind_point;
            }

			/**
			 * @brief Binds the ubo.
			 */
			void Bind() const;

			/**
			 * @brief Binds the given ubo.
			 *
			 * @param id - The id of the ubo which will be bound.
			 */
			static void Bind(const std::uint32_t& id);

			/**
			 * @brief Unbinds the bound ubo. Static version
			 * of Unbind.
			 */
			static void Unbind(const bool& save_id = false);

			/**
			 * @brief Binds the stored previously bound ubo. Static version
			 * of Rebind.
			 */
			static void Rebind();

			#define TILIA_ENABLE_IF_UNIFORM std::enable_if_t<std::is_same<float, T>::value || std::is_same<std::int32_t, T>::value || std::is_same<std::uint32_t, T>::value || std::is_same<bool, T>::value>* = nullptr

			template<typename T, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::vector<T> v)
			{
				Uniform(loc, sizeof(T), &v.front());
			}

			template<typename T, std::size_t S, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::array<T, S> v)
			{
				Uniform(loc, sizeof(T), &v.front());
			}

			template<typename T, TILIA_ENABLE_IF_UNIFORM>				
			void Uniform(const std::string& loc, std::initializer_list<T> v)
			{
				Uniform(loc, sizeof(T), v.begin());
			}

			template<typename T, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, T v)
			{
				Uniform(loc, sizeof(T), &v);
			}

			template<typename T, glm::length_t size, glm::qualifier Q, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::vector<glm::vec<size, T, Q>> v)
			{
				Uniform(loc, sizeof(T) * size, &v.front());
			}

			template<typename T, std::size_t S, glm::length_t size, glm::qualifier Q, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::array<glm::vec<size, T, Q>, S> v)
			{
				Uniform(loc, sizeof(T) * size, &v.front());
			}

			template<typename T, glm::length_t size, glm::qualifier Q, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::initializer_list<glm::vec<size, T, Q>> v)
			{
				Uniform(loc, sizeof(T) * size, v.begin());
			}

			template<typename T, glm::length_t size, glm::qualifier Q, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, glm::vec<size, T, Q> v)
			{
				Uniform(loc, sizeof(T) * size, glm::value_ptr(v));
			}

			template<glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, std::vector<glm::mat<size_x, size_y, float, Q>> v)
			{
				Uniform(loc, sizeof(float) * size_y, &v.front());
			}

			template<std::size_t S, glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, std::array<glm::mat<size_x, size_y, float, Q>, S> v)
			{
				Uniform(loc, sizeof(float) * size_y, &v.front());
			}

			template<glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, std::initializer_list<glm::mat<size_x, size_y, float, Q>> v)
			{
				Uniform(loc, sizeof(float) * size_y, v.begin());
			}

			template<glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, glm::mat<size_x, size_y, float, Q> v)
			{
				Uniform(loc, sizeof(float) * size_y, glm::value_ptr(v));
			}

			void Uniform(const std::string& loc, const std::size_t& var_size, const void* vs);

			void Uniform(const std::size_t& offset, const std::size_t& size, const void* vs);
			void Uniform(const std::size_t& offset, const std::size_t& array_size, const std::size_t& var_size, const void* vs);

			#undef TILIA_ENABLE_IF_UNIFORM
			
        private:

			std::size_t Push_Variable(std::size_t block_size, const std::string& name, const GLSL_Variable& variable);

            std::uint32_t m_ID{};

            std::uint32_t m_bind_point{};

            std::unordered_map<std::string, std::pair<std::size_t, GLSL_Variable>> m_variables{};

            static std::uint32_t s_bound_ID;
			static std::uint32_t s_previous_ID;

        };

    } // gfx
    

} // tilia


#endif // TILIA_UNIFORM_BUFFER_HPP