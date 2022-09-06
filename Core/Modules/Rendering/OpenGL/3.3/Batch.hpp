/*****************************************************************//**
 * @file   Batch.h
 * @brief  Declares and defines a class called @Batch which holds information about the rendering 
 *		   batch that the @Renderer class will generate
 * 
 * @define TILIA_BATCH_H
 * 
 * Standard:
 * @include <vector>
 * @include <array>
 * @include <map>
 * 
 * Headers:
 * @include "headers/Mesh.h"
 * @include "headers/Enums.h"
 * 
 * @author Gustav Fagerlind
 * @date   28/05/2022
 *********************************************************************/

#ifndef TILIA_BATCH_H
#define TILIA_BATCH_H

// Standard
#include <vector>
#include <array>
#include <map>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Mesh.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Mesh.hpp"
#include "Core/Values/OpenGL/3.3/Enums.hpp"

namespace tilia {

	namespace gfx {

		/**
		 * @brief Holds information which can be used to draw a "batch". A "batch" 
		 * is like a collection of information used to draw things to the screen 
		 * and they are used because of limitations such as the amount of textures 
		 * which can be bound at once.
		 */
		class Batch {
		public:

			/**
			 * @brief Sets all information specific to the given mesh_data to all such memebers in this Batch
			 * 
			 * @param mesh_data - The mesh_data to set members to its data
			 */
			Batch(std::weak_ptr<Mesh_Data> mesh_data);

			/**
			 * @brief Deletes all of the openGL buffers
			 */
			~Batch();

			/**
			 * @brief Sets all information specific to the given mesh_data to all such memebers in this Batch
			 * and also calls Clear.
			 *
			 * @param mesh_data - The mesh_data to set members to its data
			 */
			void Reset(std::weak_ptr<Mesh_Data> mesh_data);

			/**
			 * @brief Checks if mesh_data fits in batch. If so then it adds the textures(If any) to the textures 
			 * vector and adds the vertices and indices to the respective openGL buffer of the batch.
			 * 
			 * @param mesh_data - The mesh_data to check and push
			 * 
			 * @return Returns true if data fits and is pushed. Otherwise returns false.
			 */
			bool Push_Mesh(std::weak_ptr<Mesh_Data> mesh_data, float distance = -1.0f);

			/**
			 * @brief Gets the amount of pushed meshes
			 *
			 * @return The amount of pushed meshes
			 */
			inline size_t Get_Mesh_Count() const { return m_mesh_count; }

			/**
			 * @brief Gets the amount of pushed vertices * m_vertex_size
			 * 
			 * @return The amount of pushed vertices * m_vertex_size
			 */
			inline size_t Get_Vertex_Count() const { return m_vertex_count; }
			/**
			 * @brief Gets the size of each vertex of the batch in amount of floats.
			 * 
			 * @return The size of each vertex.
			 */
			inline size_t Get_Vertex_Size() const { return m_vertex_size; }
			/**
			 * @brief Gets the amount of pushed indices
			 *
			 * @return The amount of pushed indices
			 */
			inline size_t Get_Index_Count() const { return m_index_count; }

			/**
			 * @brief Resets m_texture_count, m_vertex_count, and m_index_count to 0.
			 */
			void Clear();

			/**
			 * @brief Binds all buffers, then textures, the polygonmode, and then draws the data to the
			 * bound framebuffer using glDrawElements.
			 */
			void Render();

			// Todo: Placeholder
			glm::vec3 m_camera_pos{};

		private:

			// Buffers

			uint32_t m_vao{}, // The id to the openGL vertex array object
				m_vbo{}, // The id to the openGL vertex buffer object
				m_ebo{}; // The id to the openGL element buffer object

			std::vector<std::weak_ptr<Texture>> m_textures{}; // The texture to be bound and used to draw with

			// Misc

			// Holds offsets for the vertex data. Floats is for distance, first 
			// uint32_t is for starting offset to data and second uint32_t is for 
			// the size of the data.
			std::map<float, std::pair<uint32_t, uint32_t>> m_vertex_data_offsets{}; 

			std::vector<float> m_vertex_data{}; // The vertex data to be mapped to the buffers and then rendered.
			std::vector<uint32_t> m_index_data{}; // The index data to be mapped to the buffers and then rendered.

			size_t m_mesh_count{}; // The amount of meshes stored in the buffers

			size_t m_texture_count{}; // The amount of textures to be drawn

			std::weak_ptr<Shader> m_shader{}; // The shader of the batch

			std::weak_ptr<Shader_Data> m_shader_data{}; // The shader data

			size_t m_vertex_size{}; // The size of the vertices bound to the openGL objects

			Vertex_Info m_vertex_info{}; // The info for each vertex in the vertex buffer

			size_t m_vertex_count{}, // The amount of vertices * m_vertex_size currently in the vertex buffer(m_vbo)
				m_index_count{};  // The amount of indices currently in the element buffer(m_ebo)

			// Rendering settings

			bool m_transparent{};

			enums::Primitive m_primitive{}; // The primitve to be used to draw with.
			enums::Polymode	 m_polymode{};  // The polygonmode to be used to draw with.
			enums::Face m_cull_face{}; // The face that will be culled.
			enums::Test_Func m_depth_func{}; // The depth func to be used to draw with.

			// Stencil test values

			std::pair<uint8_t, uint8_t> m_stencil_masks{}; // The mask value used when applying new stencil buffer value
			std::pair<enums::Test_Func, enums::Test_Func> m_stencil_funcs{}; // The function which will be used to compare the stencil values
			std::pair<uint8_t, uint8_t> m_compare_s_values{}; // The value to compare against in the stencil test
			std::pair<uint8_t, uint8_t> m_compare_s_masks{}; // The mask value used when comparing between the previous stencil value and the comparison value

			// The actions to take wheter or not different tests passes or fails. The Tests are: 
			// Stencil test fails,
			// Stencil test passes and depth test fails,
			// Both stencil- and depthtest pass.
			std::pair<enums::Test_Action, enums::Test_Action> m_stencil_actions[3]{};

			void Generate_Texture_Offsets(std::weak_ptr<tilia::gfx::Mesh_Data> mesh_data, std::unordered_map<uint32_t, uint32_t>& offsets);

			void Map_Data() const;

			void Sort_Mesh_Data(const std::vector<float>& vertex_data, std::vector<uint32_t>& index_data, const uint32_t& start, const uint32_t& end);

			 /**
			  * @brief Sets the vertex attributes for the vertex array using the information in m_vertex_info
			  */
			void Set_Vertex_Attribs() const;

			/**
			 * @brief Checks if the given mesh_data can be pushed to the Batch.
			 *
			 * @param mesh_data - The mesh_data to check if it will fit
			 *
			 * @return True if mesh_data fits and false if not
			 */
			bool Check_Mesh(std::weak_ptr<Mesh_Data> mesh_data) const;

		};

	}

}

#endif