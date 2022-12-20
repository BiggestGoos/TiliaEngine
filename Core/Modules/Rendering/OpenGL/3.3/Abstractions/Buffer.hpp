/**************************************************************************************************
 * @file   Buffer.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   20/12/2022
 *************************************************************************************************/

#ifndef TILIA_BUFFER_HPP
#define TILIA_BUFFER_HPP

// Vendor

// Standard
#include <cstdint>
#include <cstddef>

namespace tilia
{
	namespace gfx 
	{
		class Buffer 
		{
		public:

			void Init(const std::size_t& size);

		private:

			std::uint32_t m_ID{};

		};
	} // gfx
} // tilia

#endif // TILIA_BUFFER_HPP