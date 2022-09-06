/*****************************************************************//**
 * @file   Vertex.h
 * @brief  Declares and defines a @Vertex struct that will hold the basic information of a vertex used to draw meshes.
 * 
 * @define TILIA_VERTEX_H
 * 
 * Dependencies:
 * @include "dependencies/glm/include/glm/glm.hpp"
 * 
 * Standard:
 * @include <array>
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

#ifndef TILIA_VERTEX_H
#define TILIA_VERTEX_H

// Standard
#include <array>

namespace tilia {

	namespace gfx {

		/**
		 * @brief The layout of a vertex. Used to set vertex attributes.
		 */
		struct Vertex_Info {
			std::vector<int32_t> sizes{};   // The sizes of the attributes in the vertex.
			std::vector<int32_t> strides;   // The strides of of the attributes in the vertex.
			std::vector<int32_t> offsets{}; // The offsets of the attributes in the vertex.
			bool operator==(const Vertex_Info& other) const {
				if (this->sizes != other.sizes)
					return false;
				if (this->strides != other.strides)
					return false;
				if (this->offsets != other.offsets)
					return false;
				return true;
			}
			bool operator!=(const Vertex_Info& other) const {
				return !(*this == other);
			}
		};

		/**
		 * @brief A struct which holds information pertaining to a basic vertex
		 */
		template <size_t count>
		struct Vertex
		{
			std::array<float, count> vertices{};
			const float& operator[](const size_t& index) {
				return vertices[index];
			}
		};

	}

}

#endif