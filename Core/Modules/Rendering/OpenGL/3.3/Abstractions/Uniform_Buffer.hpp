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

			struct GLSL_Variable
			{
				enums::GLSL_Scalar_Type scalar_type{};
				enums::GLSL_Container_Type container_type{};
				std::size_t array_count{};
				GLSL_Variable(const enums::GLSL_Scalar_Type& type) : scalar_type{ type }, container_type{ enums::GLSL_Container_Type::Scalar }, array_count{ 0 } { }
				GLSL_Variable(const enums::GLSL_Scalar_Type& type, const std::size_t& count) : scalar_type{ type }, container_type{ enums::GLSL_Container_Type::Scalar }, array_count{ count } { }
				GLSL_Variable(const enums::GLSL_Scalar_Type& type, const enums::GLSL_Container_Type& c_type) : scalar_type{ type }, container_type{ c_type }, array_count{ 0 } { }
				GLSL_Variable(const enums::GLSL_Scalar_Type& type, const enums::GLSL_Container_Type& c_type, const std::size_t& count) : scalar_type{ type }, container_type{ c_type }, array_count{ count } { }

			};

            void Init(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables);


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

			template<typename T,
			std::enable_if_t<std::is_same<float, T>::value || std::is_same<std::int32_t, T>::value || std::is_same<std::uint32_t, T>::value>* = nullptr>
			void Uniform(const std::string& loc, std::initializer_list<T> vs)
			{
				Uniform(loc, vs.begin());
			}

			template<typename T, glm::length_t size, glm::qualifier Q,
			std::enable_if_t<std::is_same<float, T>::value || std::is_same<std::int32_t, T>::value || std::is_same<std::uint32_t, T>::value>* = nullptr>
			void Uniform(const std::string& loc, glm::vec<size, T, Q> v)
			{
				Uniform(loc, glm::value_ptr(v));
			}

			template<glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, glm::mat<size_x, size_y, float, Q> v)
			{
				Uniform(loc, glm::value_ptr(v));
			}

			void Uniform(const std::string& loc, const void* vs);

        private:

            std::uint32_t m_ID{};

            std::uint32_t m_bind_point{};

            std::unordered_map<std::string, std::pair<std::size_t, std::size_t>> m_variables{};

            static std::uint32_t s_bound_ID;
			static std::uint32_t s_previous_ID;

        };

    } // gfx
    

} // tilia


#endif // TILIA_UNIFORM_BUFFER_HPP