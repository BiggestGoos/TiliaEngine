/**************************************************************************************************
 * @file   Uniform_Buffer.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   11/08/2022
 *************************************************************************************************/

// Vendor

// Standard
#include <cstdint>

// Tilia

#ifndef TILIA_UNIFORM_BUFFER_HPP
#define TILIA_UNIFORM_BUFFER_HPP

namespace tilia
{
    
    namespace gfx
    {
        
        class Uniform_Buffer
        {
        public:
            
            inline auto Get_ID() {
				return m_ID;
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

        private:

            std::uint32_t m_ID{};

            std::uint32_t m_Bind_Index{};

            std::uint32_t m_base{}, m_size{};

            static std::uint32_t s_bound_ID;
			static std::uint32_t s_previous_ID;

        };

    } // gfx
    

} // tilia


#endif // TILIA_UNIFORM_BUFFER_HPP