/*****************************************************************//**
 * @file   Mesh.h
 * @brief  A mesh which will be given to the @Renderer. It will hold information like the vertices and indices.
 * 
 * Standard:
 * @include <vector>
 * 
 * Headers:
 * @include "headers/Vertex.h"
 * @include "headers/Texture_2D.h"
 * @include "headers/Texture.h"
 * @include "headers/Shader.h"
 * @include "headers/Shader_Data.h"
 * @include "headers/Enums.h"
 * @include "headers/Logging.h"
 * 
 * @author Gustav Fagerlind
 * @date   28/05/2022
 *********************************************************************/

#ifndef TILIA_MESH_H
#define TILIA_MESH_H

// Standard
#include <vector>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Vertex.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture_2D.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Texture.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader.hpp"
//#include "Core/Modules/Rendering/OpenGL/3.3/Abstractions/Shader_Data.hpp"
#include "Core/Values/OpenGL/3.3/Enums.hpp"
#include "Core/Modules/Console/Logging.hpp"

namespace tilia {

	namespace gfx {

		/**
		 * @brief Holds information about the mesh. Used to pass data to the Renderer
		 * 
		 * @param vertex_size	   - The size of the vertices, ie. how many floats it consists of.
		 * @param vertex_data	   - All of the floats building up the vertices.
		 * @param indices		   - The indices of the mesh.
		 * @param shader		   - The shader of the mesh.
		 * @param shader_data	   - The shader data to be set while rendering.
		 * @param transparent	   - Wheter or not the mesh is transparent.
		 * @param primitive		   - The primitive of the mesh.
		 * @param polymode		   - The polygon mode of the mesh.
		 * @param cull_face		   - The culling face of the mesh.
		 * @param depth func	   - The depth function of the mesh.
		 * @param stencil_mask	   - 
		 * @param stencil_func     - 
		 * @param compare_s_value  - 
		 * @param compare_s_mask   - 
		 * @param stencil_actions  - 
		 * @param vertex_pos_start - The start offset to the position of the vertex.
		 * @param vertex_pos_end   - The end offset to the position of the vertex.
		 * @param texture_offset   - The texture index offset of the mesh.
		 * @param textures		   - The textures of the mesh.
		 * @param vertex_info	   - The vertex info of the mesh.
		 */
		struct Mesh_Data {
			size_t											   vertex_size{};
			std::vector<float>*								   vertex_data{};
			std::vector<uint32_t>*							   indices{};
// Todo: hello
			//std::weak_ptr<Shader>*							   shader{};
			//std::weak_ptr<Shader_Data>*						   shader_data{};
			bool*											   transparent{};
			enums::Primitive*								   primitive{};
			enums::Polymode*								   polymode{};
			enums::Face*    								   cull_face{};
			enums::Test_Func*								   depth_func{};
			glm::bvec4*										   color_mask{};
			std::pair<uint8_t, uint8_t>*					   stencil_masks{};
			std::pair<enums::Test_Func, enums::Test_Func>*	   stencil_funcs{};
			std::pair<uint8_t, uint8_t>*					   compare_s_values{};
			std::pair<uint8_t, uint8_t>*					   compare_s_masks{};
			std::pair<enums::Test_Action, enums::Test_Action>* stencil_actions{};
			uint32_t*										   vertex_pos_start{};
			uint32_t*										   vertex_pos_end{};
			int32_t*										   texture_offset{};
			std::vector<std::weak_ptr<Texture>>*			   textures{};
			Vertex_Info* 									   vertex_info{};
		};

		/**
		 * @brief A mesh class which can be given to the Renderer. Takes in a vertex size as a template 
		 * argument. This decides the amount of floats in each vertex. It holds information about 
		 * vertices, vertex layout, indices, textures, a shader, the primitve, and the polygon mode.
		 */
		template<size_t vert_size>
		class Mesh {
		public:

			std::vector<Vertex<vert_size>> vertices{}; // The vertices of the mesh.
			std::vector<uint32_t> indices{}; // The indices of the mesh.

			/**
			 * @brief Adds a texture to the m_textures vector. The texture will be used to 
			 * draw this mesh if it is set in the shader.
			 * 
			 * @param texture - The texture to be added.
			 */
			inline void Add_Texture(std::weak_ptr<Texture> texture) { m_textures.push_back(texture); }
			/**
			 * @brief Removes the texture from the m_textures vector.
			 * 
			 * @param texture - The texture to be removed.
			 */
			inline void Remove_Texture(Texture* texture)
			{ 
				size_t size{ m_textures.size() };
				size_t index{};
				for (size_t i = 0; i < size; i++)
				{
					if (m_textures[i].lock().get() == texture)
						index = i;
				}
				m_textures.erase(m_textures.begin() + index);
			}

			/**
			 * @brief Gets the amount of floats in a vertex.
			 *
			 * @return The amount of floats in a vertex.
			 */
			inline auto Get_Var_Vertex_Size() {
				return vert_size;
			}

			/**
			 * @brief Sets m_shader to the given shader.
			 *
			 * @param shader - The shader to set m_shader to.
			 */			
			inline auto Set_Shader() {
				return 0;
				//return [this](std::weak_ptr<Shader> shader) { m_shader = shader; };
			}
			/**
			 * @brief Gets the shader of the mesh.
			 * 
			 * @return m_shader - The shader of the mesh.
			 */
			inline auto Get_Shader() {
				return 0;
				//return m_shader;
			}

			/**
			 * @brief Sets the shader data of the mesh.
			 * 
			 * @param shader_data - The new shader data which m_shader_data is set to.
			 */
			inline auto Set_Shader_Data() {
				//return [this](std::weak_ptr<Shader_Data> shader_data) { m_shader_data = shader_data; };
			}
			/**
			 * @brief Gets the shader data.
			 * 
			 * @return m_shader_data - The shader data of the mesh.
			 */
			inline auto Get_Shader_Data() {
				//return m_shader_data;
			}

			/**
			 * @brief Set m_transparent. It will tell the renderer to render this mesh with transparency.
			 *
			 * @param transparent - Set m_transparent to this.
			 */
			inline auto Set_Transparent() {
				return [this](const bool& transparent) { m_transparent = transparent; };
			}
			/**
			 * @brief Gets m_transparent.
			 *
			 * @return m_transparent - Wheter the mesh is transparent or not.
			 */
			inline auto Get_Transparent() {
				return m_transparent;
			}

			/**
			 * @brief Set m_primitive.
			 *
			 * @param primitive - Set m_primitive to this.
			 */
			inline auto Set_Primitive() {
				return [this](const enums::Primitive& primitive) { m_primitive = primitive; };
			}
			/**
			 * @brief Gets the primitive.
			 *
			 * @return m_primitive - The primitive of the mesh.
			 */
			inline auto Get_Primitive() {
				return m_primitive;
			}

			/**
			 * @brief Set m_polymode.
			 *
			 * @param polymode - Set m_polymode to this.
			 */
			inline auto Set_Polymode() {
				return [this](const enums::Polymode& polymode) { m_polymode = polymode; };
			}
			/**
			 * @brief Gets the polymode.
			 *
			 * @return m_polymode - The polymode of the mesh.
			 */
			inline auto Get_Polymode() {
				return m_polymode;
			}

			/**
			 * @brief Set m_cull_face.
			 *
			 * @param cull_face - Set m_cull_face to this.
			 */
			inline auto Set_Cull_Face() {
				return [this](const enums::Face& cull_face) { m_cull_face = cull_face; };
			}
			/**
			 * @brief Gets the culling face.
			 *
			 * @return m_cull_face - The culling face of the mesh.
			 */
			inline auto Get_Cull_Face() {
				return m_cull_face;
			}

			/**
			 * @brief Set m_depth_func.
			 *
			 * @param depth_func - Set m_depth_func to this.
			 */
			inline auto Set_Depth_Func() {
				return [this](const enums::Test_Func& depth_func) { m_depth_func = depth_func; };
			}
			/**
			 * @brief Gets the depth function.
			 *
			 * @return m_depth_func - The depth function of the mesh.
			 */
			inline auto Get_Depth_Func() {
				return m_depth_func;
			}
			
			/**
			 * @brief Sets the stencil mask which is used in rendering.
			 * 
			 * @param face - The face for the mask to affect.
			 * @param stencil_mask - The mask to set this meshes mask to.
			 */
			inline auto Set_Stencil_Mask() {
				return [this](const enums::Face& face, const uint8_t& stencil_mask) 
				{ 
					if (face == enums::Face::Front)
						m_stencil_masks.first = stencil_mask;
					if (face == enums::Face::Back)
						m_stencil_masks.second = stencil_mask;
					if (face != enums::Face::Both) 
						return;
					m_stencil_masks.first = stencil_mask;
					m_stencil_masks.second = stencil_mask;
				};
			}
			/**
			 * @brief Gets the stencil mask of the mesh.
			 * 
			 * @return m_stencil_mask - The stencil mask of the mesh.
			 */
			inline auto Get_Stencil_Mask() {
				return m_stencil_masks;
			}

			/**
			 * @brief Sets the values used in the stencil func function.
			 * 
			 * @param face - The face to affect with the stencil func, the comparison value and the mask.
			 * @param stencil_func - The stencil func to be used.
			 * @param compare_stencil_value - The comparison value to be used.
			 * @param compare_stencil_mask - The comparison mask to be used.
			 */
			inline auto Set_Stencil_Func() {
				return [this](const enums::Face& face, const enums::Test_Func& stencil_func, const uint8_t& compare_stencil_value, const uint8_t& compare_stencil_mask)
				{ 
					if (face == enums::Face::Front)
					{
						m_stencil_funcs.first = stencil_func;
						m_compare_s_values.first = compare_stencil_value;
						m_compare_s_masks.first = compare_stencil_mask;
					}
					if (face == enums::Face::Back)
					{
						m_stencil_funcs.second = stencil_func;
						m_compare_s_values.second = compare_stencil_value;
						m_compare_s_masks.second = compare_stencil_mask;
					}
					if (face != enums::Face::Both)
						return;
					m_stencil_funcs.first = stencil_func;
					m_compare_s_values.first = compare_stencil_value;
					m_compare_s_masks.first = compare_stencil_mask;
					m_stencil_funcs.second = stencil_func;
					m_compare_s_values.second = compare_stencil_value;
					m_compare_s_masks.second = compare_stencil_mask;
				};
			}
			/**
			 * @brief Gets the stencil func variables.
			 * 
			 * @return The stencil func variables.
			 */
			inline auto Get_Stencil_Func() {
				struct Stencil_Test_Container {
					std::pair<enums::Test_Func, enums::Test_Func> stencil_funcs{};
					std::pair<uint8_t, uint8_t> compare_values{};
					std::pair<uint8_t, uint8_t> compare_masks{};
				} container{ m_stencil_funcs, m_compare_s_values, m_compare_s_masks };
				return container;
			}

			/**
			 * @brief Sets the actions to take when different tests pass for the stencil buffer.
			 * 
			 * @param face     - The face for the tests to affect
			 * @param action_1 - The action to take if the stencil test fails.
			 * @param action_2 - The action to take if the stencil test passes but the depth test fails.
			 * @param action_3 - The action to take if both the stencil- and depth test passes.
			 */
			inline auto Set_Stencil_Op() {
				return [this](const enums::Face& face, const enums::Test_Action& action_1, const enums::Test_Action& action_2, const enums::Test_Action& action_3)
				{
					if (face == enums::Face::Front)
					{
						m_stencil_actions[0].first = action_1;
						m_stencil_actions[1].first = action_2;
						m_stencil_actions[2].first = action_3;
					}
					if (face == enums::Face::Back)
					{
						m_stencil_actions[0].second = action_1;
						m_stencil_actions[1].second = action_2;
						m_stencil_actions[2].second = action_3;
					}
					if (face != enums::Face::Both)
						return;
					m_stencil_actions[0].first = action_1;
					m_stencil_actions[1].first = action_2;
					m_stencil_actions[2].first = action_3;
					m_stencil_actions[0].second = action_1;
					m_stencil_actions[1].second = action_2;
					m_stencil_actions[2].second = action_3;
				};
			}
			/**
			 * @brief Gets the stencil actions.
			 * 
			 * @return The stencil actions.
			 */
			inline auto Get_Stencil_Op() {
				struct Stencil_Actions_Container {
					std::pair<enums::Test_Action, enums::Test_Action> stencil_fail{};
					std::pair<enums::Test_Action, enums::Test_Action> depth_fail{};
					std::pair<enums::Test_Action, enums::Test_Action> depth_pass{};
				} container{ m_stencil_actions[0], m_stencil_actions[1], m_stencil_actions[2] };
				return container;
			}

			/**
			 * @brief Sets the offsets to the start and end of the position in the vertex data.
			 * It will be used when sorting transparent meshes.
			 * 
			 * @param start_offset - The offset to the start of the position in amount of floats.
			 * @param end_offset - The offset to the end of the position in amount of floats.
			 */
			inline auto Set_Postion_Offsets() {
				return [this](const uint32_t& start_offset, const uint32_t& end_offset)
				{
					m_vertex_pos_start = start_offset;
					m_vertex_pos_end = end_offset;
				};
			}
			/**
			 * @brief Gets the offsets to the start and the end of the position in the vertex data.
			 * 
			 * @return - The offsets to the start and end of the position.
			 */
			inline auto Get_Postion_Offsets() {
				struct Offsets_Container {
					uint32_t start_offset{};
					uint32_t end_offset{};
				} container{ m_vertex_pos_start, m_vertex_pos_end };
				return container;
			}

			/**
			 * @brief Set m_texture_index_offset. It will update the vertex data so that
			 * texture indices will point to the right texture. The offset is in amount of
			 * previous floats to where the texture index is.
			 *
			 * @param texture_offset - Set m_texture_index_offset to this.
			 */
			inline auto Set_Texture_Offset() {
				return [this](const uint32_t& texture_offset)
				{
					m_texture_index_offset = texture_offset;
				};
			}
			/**
			 * @brief Gets m_texture_index_offset
			 *
			 * @return m_texture_index_offset - The texture index offset of the mesh
			 */
			inline auto Get_Texture_Offset() {
				return m_texture_index_offset;
			}

			/**
			 * @brief Sets the vertex layout of the mesh
			 *
			 * @param vertex_info - The vertex layout which will be used to set m_vertex_info
			 */
			inline auto Set_Vertex_Info() {
				return [this](const Vertex_Info& vertex_info)
				{
					m_vertex_info = vertex_info;
				};
			}
			/**
			 * @brief Gets the vertex layout of the mesh
			 *
			 * @return The vertex layout
			 */
			inline auto Get_Vertex_Info() {
				return m_vertex_info;
			}

			/**
			 * @brief Gets the data from the mesh in a simpler state. Give to Renderer.
			 * 
			 * @return The mesh data
			 */
			inline std::weak_ptr<Mesh_Data> Get_Mesh_Data() {
				if (!m_mesh_data.get())
				{
					m_mesh_data = std::make_shared<Mesh_Data>();
					m_mesh_data->vertex_size = vert_size;
					m_mesh_data->vertex_data = static_cast<std::vector<float>*>(static_cast<void*>(&vertices));
					m_mesh_data->indices = &indices;
					// Todo: Fix
					//m_mesh_data->shader = &m_shader;
					//m_mesh_data->shader_data = &m_shader_data;
					m_mesh_data->transparent = &m_transparent;
					m_mesh_data->primitive = &m_primitive;
					m_mesh_data->polymode = &m_polymode;
					m_mesh_data->cull_face = &m_cull_face;
					m_mesh_data->depth_func = &m_depth_func;
					m_mesh_data->stencil_masks = &m_stencil_masks;
					m_mesh_data->stencil_funcs = &m_stencil_funcs;
					m_mesh_data->compare_s_values = &m_compare_s_values;
					m_mesh_data->compare_s_masks = &m_compare_s_masks;
					m_mesh_data->stencil_actions = m_stencil_actions;
					m_mesh_data->vertex_pos_start = &m_vertex_pos_start;
					m_mesh_data->vertex_pos_end = &m_vertex_pos_end;
					m_mesh_data->texture_offset = &m_texture_index_offset;
					m_mesh_data->textures = &m_textures;
					m_mesh_data->vertex_info = &m_vertex_info;
				}

				return m_mesh_data;
			}

		private:

			bool m_transparent{ false }; // Wheter or not the mesh is transparent.

			std::vector<std::weak_ptr<Texture>> m_textures{}; // A vector of all of the textures used in the mesh. 

			int32_t m_texture_index_offset{ -1 }; // The offset to the texture index.

			uint32_t m_vertex_pos_start{}; // Start of the position in the vertices.
			uint32_t m_vertex_pos_end{}; // End of the position in the vertices.

			// Todo: remove posibly
			//std::weak_ptr<Shader> m_shader{}; // The shader this mesh should be drawn with

			//std::weak_ptr<Shader_Data> m_shader_data{}; // The shader data of the mesh.

			enums::Primitive m_primitive{ enums::Primitive::Points }; // The primitve this mesh should be drawn with

			enums::Polymode m_polymode{ enums::Polymode::Fill }; // The polymode this mesh should be drawn with

			enums::Face m_cull_face{ enums::Face::None }; // The face to be culled

			enums::Test_Func m_depth_func{ enums::Test_Func::Less }; // The depth func this mesh should be drawn with

			std::shared_ptr<Mesh_Data> m_mesh_data{}; // The Mesh_Data of the mesh

			Vertex_Info m_vertex_info{}; // The vertex layout of each vertex in the mesh

			// Stencil test values

			std::pair<uint8_t, uint8_t> m_stencil_masks{ 0xFF, 0xFF }; // The mask values used when applying new stencil buffer value

			// The function which will be used to compare the stencil values
			std::pair<enums::Test_Func, enums::Test_Func> m_stencil_funcs{ enums::Test_Func::None, enums::Test_Func::None };

			std::pair<uint8_t, uint8_t> m_compare_s_values{ 0x00, 0x00 }; // The value to compare against in the stencil test

			// The mask value used when comparing between the previous stencil value and the comparison value
			std::pair<uint8_t, uint8_t> m_compare_s_masks{ 0xFF, 0xFF };

			// The actions to take wheter or not different tests passes or fails. The Tests are: 
			// Stencil test fails,
			// Stencil test passes and depth test fails,
			// Both stencil- and depthtest pass.
			std::pair<enums::Test_Action, enums::Test_Action> m_stencil_actions[3]{
				{ enums::Test_Action::Keep, enums::Test_Action::Keep },
				{ enums::Test_Action::Keep, enums::Test_Action::Keep },
				{ enums::Test_Action::Keep, enums::Test_Action::Keep }
			};

		};

	}

}

#endif