/*****************************************************************//**
 * @file   Batch.cpp
 * @brief  Defines all non-inline member functions and constructors/destructors of the @Batch class in @include "headers/Batch.h"
 * 
 * Dependencies:
 * @include "dependencies/glad/include/glad/glad.h"
 * 
 * Standard:
 * @include <unordered_map>
 * @include <map>
 * 
 * Headers:
 * @include "headers/Batch.h"
 * @include "headers/Error_Handling.h"
 * @include "headers/Enums.h"
 * @include "headers/Utils.h"
 * 
 * @author Gustav Fagerlind
 * @date   28/05/2022
 *********************************************************************/

// Dependencies
#include "vendor/glad/include/glad/glad.h"

// Standard
#include <unordered_map>
#include <map>
#include <cstring>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Batch.hpp"
#include "Core/Modules/Rendering/OpenGL/3.3/Error_Handling.hpp"
#include "Core/Values/OpenGL/3.3/Enums.hpp"
#include "Core/Values/OpenGL/3.3/Utils.hpp"

#include <iostream>

/**
 * Sets the batch to be compatible with the given mesh_data. Also calls Set_Vertex_Attribs, generates some buffers, and allocates memory for them.
 */
tilia::gfx::Batch::Batch(std::weak_ptr<Mesh_Data> mesh_data)
	: // Sets batch to be compatible
	m_textures{  },
	// Todo: remove posibly
	//m_shader		  { *mesh_data.lock()->shader },
	m_shader_data	  { *mesh_data.lock()->shader_data },
	m_transparent	  { *mesh_data.lock()->transparent },
	m_primitive	 	  { *mesh_data.lock()->primitive },
	m_polymode		  { *mesh_data.lock()->polymode },
	m_cull_face		  { *mesh_data.lock()->cull_face },
	m_depth_func	  { *mesh_data.lock()->depth_func },
	m_stencil_masks	  { *mesh_data.lock()->stencil_masks },
	m_stencil_funcs	  { *mesh_data.lock()->stencil_funcs },
	m_compare_s_values{ *mesh_data.lock()->compare_s_values },
	m_compare_s_masks { *mesh_data.lock()->compare_s_masks },
	m_stencil_actions { mesh_data.lock()->stencil_actions[0], mesh_data.lock()->stencil_actions[1], mesh_data.lock()->stencil_actions[2] },
	m_vertex_size	  { mesh_data.lock()->vertex_size },
	m_vertex_info	  { *mesh_data.lock()->vertex_info }
{

	m_textures.resize(32);

	// Generates buffers
	GL_CALL(glGenVertexArrays(1, &m_vao));
	GL_CALL(glGenBuffers(1, &m_vbo));
	GL_CALL(glGenBuffers(1, &m_ebo));

	GL_CALL(glBindVertexArray(m_vao));

	// Allocates memory for vertex buffer
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * m_vertex_size * *enums::Misc::Max_Vertices), nullptr, GL_DYNAMIC_DRAW));

	// Calls Set_Vertex_Attribs to set vertex attributes
	Set_Vertex_Attribs();

	// Allocates memory for element buffer
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast <GLsizeiptr>(sizeof(uint32_t) * *enums::Misc::Max_Indices), nullptr, GL_DYNAMIC_DRAW));

}

/**
 * Deletes openGL buffers
 */
tilia::gfx::Batch::~Batch()
{

	GL_CALL(glDeleteVertexArrays(1, &m_vao));
	GL_CALL(glDeleteBuffers(1, &m_vbo));
	GL_CALL(glDeleteBuffers(1, &m_ebo));

}

/**
 * Makes the batch compatible with mesh_data, clears some data, and then calls Set_Vertex_Attribs
 */
void tilia::gfx::Batch::Reset(std::weak_ptr<Mesh_Data> mesh_data)
{
	// Sets batch to be compatible
	// Todo: Remove posibly
	//m_shader		   = *mesh_data.lock()->shader;
	m_shader_data	   = *mesh_data.lock()->shader_data;
	m_transparent	   = *mesh_data.lock()->transparent;
	m_primitive		   = *mesh_data.lock()->primitive;
	m_polymode		   = *mesh_data.lock()->polymode;
	m_cull_face		   = *mesh_data.lock()->cull_face;
	m_depth_func	   = *mesh_data.lock()->depth_func;
	m_stencil_masks	   = *mesh_data.lock()->stencil_masks;
	m_stencil_funcs	   = *mesh_data.lock()->stencil_funcs;
	m_compare_s_values = *mesh_data.lock()->compare_s_values;
	m_compare_s_masks  = *mesh_data.lock()->compare_s_masks;
	memcpy_s(m_stencil_actions, 3 * sizeof(enums::Test_Action), mesh_data.lock()->stencil_actions, 3 * sizeof(enums::Test_Action));
	m_vertex_size	   = mesh_data.lock()->vertex_size;
	m_vertex_info	   = *mesh_data.lock()->vertex_info;

	// Clears some data
	Clear();

	GL_CALL(glBindVertexArray(m_vao));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	
	// Calls Set_Vertex_Attribs to set vertex attributes
	Set_Vertex_Attribs();

}

void tilia::gfx::Batch::Generate_Texture_Offsets(std::weak_ptr<tilia::gfx::Mesh_Data> mesh_data, std::unordered_map<uint32_t, uint32_t>& offsets) {

	const size_t vertex_count{ mesh_data.lock()->vertex_data->size() };
	const size_t index_count{ mesh_data.lock()->indices->size() };
	const size_t tex_count{ mesh_data.lock()->textures->size() };

	for (size_t i = *mesh_data.lock()->texture_offset; i < vertex_count; i += m_vertex_size)\
	{
		for (size_t j = 0; j < tex_count; j++)
		{
			for (size_t k = 0; k < m_texture_count; k++) {

				if (m_textures[k].lock()->Get_ID() == (*mesh_data.lock()->textures)[j].lock()->Get_ID()) {

					offsets[static_cast<uint32_t>(i)] = static_cast<uint32_t>(k - j);
					goto end_loop;
				}
			}
			m_textures[m_texture_count++] = (*mesh_data.lock()->textures)[j];
			offsets[static_cast<uint32_t>(i)] = static_cast<uint32_t>((m_texture_count - 1) - j);
		end_loop:
			continue;
		}
	}

}

/**
 * First checks mesh_data for compatibility. Adds the textures, buffers the vertex- and 
 * index-data and then adds the mesh-, vertex-, and index-count.
 */
bool tilia::gfx::Batch::Push_Mesh(std::weak_ptr<Mesh_Data> mesh_data, float distance)
{
	// Checks mesh_data for compatibility
	if (!Check_Mesh(mesh_data))
		return false;

	//glBindVertexArray(m_vao);

	const size_t vertex_count{ mesh_data.lock()->vertex_data->size() };
	const size_t index_count{ mesh_data.lock()->indices->size() };

	std::unordered_map<uint32_t, uint32_t> texture_indices{};
	const size_t tex_count{ mesh_data.lock()->textures->size() };
	if (*mesh_data.lock()->texture_offset != -1)
	{
		Generate_Texture_Offsets(mesh_data, texture_indices);
	}
	else
	{
		for (size_t i = 0; i < tex_count; i++)
			m_textures[m_texture_count++] = (*mesh_data.lock()->textures)[i];
	}

	const std::vector<float>& new_vertex_data{ *mesh_data.lock()->vertex_data };
	std::vector<uint32_t> new_index_data{ *mesh_data.lock()->indices };

	if (m_transparent)
	{
		Sort_Mesh_Data(new_vertex_data, new_index_data, 0, 2);
	}

	for (size_t i = 0; i < vertex_count; i++)
	{
		m_vertex_data.push_back(new_vertex_data[i]);
		if ((i) % m_vertex_size == *mesh_data.lock()->texture_offset) {
			m_vertex_data[m_vertex_count + i] += static_cast<float>(texture_indices[static_cast<uint32_t>(i)]);
		}
	}

	for (size_t i = 0; i < index_count; i++)
	{
		m_index_data.push_back(new_index_data[i] + static_cast<uint32_t>(m_vertex_count / m_vertex_size));
	}

	//// Buffers vertex-data
	//GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

	//BUFFER_VERTICES(*mesh_data.lock()->texture_offset, GL_CALL
	//	(glBufferSubData(GL_ARRAY_BUFFER, 
	//		m_vertex_count * sizeof(float), 
	//		vertex_count * sizeof(float), 
	//		mesh_data.lock()->vertex_data->begin()._Ptr)));

	//// Buffers index-data
	//GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
	//BUFFER_INDICES(
	//	GL_CALL(
	//		glBufferSubData(
	//			GL_ELEMENT_ARRAY_BUFFER, 
	//			m_index_count * sizeof(uint32_t), 
	//			index_count * sizeof(uint32_t), 
	//			mesh_data.lock()->indices->begin()._Ptr)));

	// Adds mesh-count
	++m_mesh_count;

	// Adds vertex- and index-count
	m_vertex_count += vertex_count;
	m_index_count += index_count;

	return true;
}

/**
 * Sets the vertex-, index-, and texture-count to 0.
 */
void tilia::gfx::Batch::Clear()
{

	m_vertex_data.resize(0);
	m_index_data.resize(0);

	m_mesh_count = 0;
	m_vertex_count = 0;
	m_index_count = 0;
	m_texture_count = 0;

}

/**
 * First sets face culling, then sets depth mask and functions, then binds 
 * all the textures, the vertex array also holding the information of the 
 * vertex buffer and element buffer, then binds the shader, sets the 
 * polygonmode and then at last draws everything.
 */
void tilia::gfx::Batch::Render()
{

	//if (m_transparent)
	//{
	//	Sort_Mesh_Data(m_vertex_data, m_index_data, 0, 2);
	//}

	Map_Data();

	// Sets depth mask and functions
	if (m_depth_func != enums::Test_Func::None) {
		GL_CALL(glDepthMask(true));
		GL_CALL(glDepthFunc(*m_depth_func));
	}
	else
	{
		GL_CALL(glDepthMask(false));
	}

	// Sets stencil functions and actions
	//if (m_stencil_func != enums::Test_Func::None) {
	//	GL_CALL(glStencilMask(m_stencil_mask));
	//	GL_CALL(glStencilFunc(*m_stencil_func, m_compare_s_value, m_compare_s_mask));
	//	GL_CALL(glStencilOp(*m_stencil_actions[0], *m_stencil_actions[1], *m_stencil_actions[2]));
	//}
	//else
	//{
	//	GL_CALL(glStencilMask(0x00));
	//	GL_CALL(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
	//}

	// Binds textures
	for (size_t i = 0; i < m_texture_count; i++) {
		m_textures[i].lock()->Bind(static_cast<uint32_t>(i));
	}

	// Binds vertex array
	GL_CALL(glBindVertexArray(m_vao));

#if 1

	//if (m_shader_data.lock().get()) 
	//	m_shader.lock()->Uniform(*m_shader_data.lock());

	// Binds shader
	m_shader_data.lock()->Bind();

#else

	throw 1;

#endif

	// Sets polygonmode
	GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, *m_polymode));

	if (!m_transparent) 
	{
		// Sets face culling
		if (m_cull_face != enums::Face::None) {
			GL_CALL(glEnable(GL_CULL_FACE));
			GL_CALL(glCullFace(*m_cull_face));
		}
		else {
			GL_CALL(glDisable(GL_CULL_FACE));
		}

		// Draws stuff
		GL_CALL(glDrawElements(*m_primitive, static_cast<GLsizei>(m_index_count), GL_UNSIGNED_INT, nullptr));
	}
	else
	{
		// Sets face culling
		//GL_CALL(glDepthMask(true));
		GL_CALL(glDepthMask(false));
		GL_CALL(glDisable(GL_CULL_FACE));
		//GL_CALL(glEnable(GL_CULL_FACE));

		//GL_CALL(glCullFace(GL_FRONT));

		// Draws stuff
		GL_CALL(glDrawElements(*m_primitive, static_cast<GLsizei>(m_index_count), GL_UNSIGNED_INT, nullptr));

		//GL_CALL(glCullFace(GL_BACK));

		//// Draws stuff
		//GL_CALL(glDrawElements(m_primitive, static_cast<GLsizei>(m_index_count), GL_UNSIGNED_INT, nullptr));

		GL_CALL(glDepthMask(true));

	}

	//std::cout << "Vertex data: " << m_transparent << '\n';

	//float* vertex_data{ new float[m_vertex_count] };

	//GL_CALL(glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_vertex_count * sizeof(float), vertex_data));

	//for (size_t i = 0; i < m_vertex_count; i++)
	//{
	//	std::cout << vertex_data[i] << " : " << m_vertex_data[i];
	//	if ((i + 1) % m_vertex_size == 0)
	//		std::cout << '\n';
	//	else
	//		std::cout << ", ";
	//}

	//std::cout << "Index data: " << m_transparent << '\n';

	//uint32_t* index_data{ new uint32_t[m_index_count] };

	//GL_CALL(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_index_count * sizeof(uint32_t), index_data));

	//for (size_t i = 0; i < m_index_count; i++)
	//{
	//	std::cout << index_data[i] << " : " << m_index_data[i];
	//	if ((i + 1) % 3 == 0)
	//		std::cout << '\n';
	//	else
	//		std::cout << ", ";
	//}

}

void tilia::gfx::Batch::Map_Data() const
{

	GL_CALL(glBindVertexArray(m_vao));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

	GL_CALL(void* vertex_buffer{ glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY) });

	const size_t vertex_byte_count{ m_vertex_count * sizeof(float) };
	std::copy(m_vertex_data.begin(), m_vertex_data.end(), static_cast<float*>(vertex_buffer));

	GL_CALL(glUnmapBuffer(GL_ARRAY_BUFFER));

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));

	GL_CALL(void* index_buffer{ glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY) });

	const size_t index_byte_count{ m_index_count * sizeof(uint32_t) };
	std::copy(m_index_data.begin(), m_index_data.end(), static_cast<uint32_t*>(index_buffer));

	GL_CALL(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));

}

void tilia::gfx::Batch::Sort_Mesh_Data(const std::vector<float>& vertex_data, std::vector<uint32_t>& index_data, const uint32_t& start, const uint32_t& end)
{

	const size_t seq_length{ 6 };
	using sequence = std::array<uint32_t, seq_length>;

	std::map<float, std::vector<sequence>> sorted_data{};

	const size_t vertex_count{ vertex_data.size() };
	const size_t index_count{ index_data.size() };
	for (size_t i = 0; i < index_count; i += seq_length)
	{

		float length{};

		sequence seq{};

		std::unordered_map<uint32_t, bool> used_vertices;

		for (size_t j = 0; j < seq_length; j++)
		{
			if (used_vertices.find(index_data[i + j]) == used_vertices.end()) {
				for (size_t k = start; k <= end; k++)
				{
					length += fabs(vertex_data[(index_data[i + j] * m_vertex_size) + k] - m_camera_pos[static_cast<glm::vec3::length_type>(k - start)]);
				}
				used_vertices[index_data[i + j]] = true;
			}

			for (size_t k = 0; k < vertex_count; k += m_vertex_size)
			{
				if (index_data[i + j] == (k / m_vertex_size)) {
					seq[j] = static_cast<uint32_t>(k / m_vertex_size);
					break;
				}
			}

		}

		sorted_data[length].push_back(seq);

	}

	std::vector<uint32_t> new_index_data{};

	for (auto it = sorted_data.rbegin(); it != sorted_data.rend(); ++it)
	{
		//std::cout << it->first << " : " << it->second.size();
		const size_t data_count{ it->second.size() };
		for (size_t j = 0; j < data_count; j++)
		{
			for (int32_t i = 0; i < seq_length; i++)
			{
				new_index_data.push_back(it->second[j][i]);
			}
			//for (size_t k = 0; k < index_count; k++)
			//{
			//	if (it->second[j].first / m_vertex_size == index_data[k]) {
			//		it->second[j].second.push_back(static_cast<uint32_t>(it->second[j].first / m_vertex_size));
			//		//sorted_indices[it->first].push_back(static_cast<uint32_t>(it->second[j].second / m_vertex_size));
			//	}
			//}
		}
	}

	//for (auto it = sorted_indices.rbegin(); it != sorted_indices.rend(); ++it) {
	//	const size_t s_index_cout{ it->second.size() };
	//	for (size_t i = 0; i < s_index_cout; i++)
	//	{
	//		new_index_data.push_back(it->second[i]);
	//	}
	//}

	index_data = new_index_data;

}

/**
 * First gets the amount of vertex attributes to set. This is based on the amount of .sizes in
 * m_vertex_info. Then it sets the size, stride, and offset of each attribute to be set. If there
 * only is one stride in the .strides vector then it sets that stride for every attribute.
 */
void tilia::gfx::Batch::Set_Vertex_Attribs() const
{

	// Gets attribute count
	const size_t attrib_count{ m_vertex_info.sizes.size() };
	for (size_t i = 0; i < attrib_count; i++)
	{
		// If there only is one stride, sets same for all
		int32_t stride{ (m_vertex_info.strides.size() > 1) ? m_vertex_info.strides[i] : m_vertex_info.strides[0] };

		const void* offset{ reinterpret_cast<const void*>(static_cast<int64_t>(m_vertex_info.offsets[i] * sizeof(float))) };
		
		// Sets attributes
		GL_CALL(glEnableVertexAttribArray(static_cast<int32_t>(i)));
		GL_CALL(glVertexAttribPointer(static_cast<int32_t>(i), m_vertex_info.sizes[i], GL_FLOAT, GL_FALSE, stride * sizeof(float),
			offset));
	}

}

/**
 * Checks if the mesh is compatible with this batch. If so then returns true.
 */
bool tilia::gfx::Batch::Check_Mesh(std::weak_ptr<Mesh_Data> mesh_data) const
{
	std::shared_ptr<Mesh_Data> temp{ mesh_data };
	// Checks if vertex count is too big
	if ((m_vertex_count / m_vertex_size) + (temp->vertex_data->size() / temp->vertex_size) > *enums::Misc::Max_Vertices)
		return false;
	// Checks if index count is too big
	if (m_index_count + temp->indices->size()    > *enums::Misc::Max_Indices)
		return false;
	// Checks if texture count is too big
	if (m_texture_count + temp->textures->size() > utils::Get_Max_Textures())
		return false;

#if 1

#if 0

	// Checks if shader is same
	if (m_shader.lock()->Get_ID()      != temp->shader->lock()->Get_ID())
		return false;

	// Checks if shader data is same
	if (m_shader_data.lock().get() && temp->shader_data->lock().get()) {
		if (*m_shader_data.lock() != *temp->shader_data->lock())
			return false;
	}
	else if (temp->shader_data->lock().get())
	{
		return false;
	}

#endif

	if (m_shader_data.lock()->Get_ID() != temp->shader_data->lock()->Get_ID())
		return false;

#else

throw 1;

#endif

	// Checks if transparency is same
	if (m_transparent != *temp->transparent)
		return false;
	// Checks if primitve is same
	if (m_primitive   != *temp->primitive)
		return false;
	// Checks if polymode is same
	if (m_polymode    != *temp->polymode)
		return false;
	// Checks if cull face is same
	if (m_cull_face   != *temp->cull_face)
		return false;
	// Checks if depth_func is same
	if (m_depth_func  != *temp->depth_func)
		return false;
	// Checks if stencil_mask is same
	if (m_stencil_masks != *temp->stencil_masks)
		return false;
	// Checks if stencil_func is same
	if (m_stencil_funcs != *temp->stencil_funcs)
		return false;
	// Checks if compare_s_value is same
	if (m_compare_s_values != *temp->compare_s_values)
		return false;
	// Checks if compare_s_mask is same
	if (m_compare_s_masks != *temp->compare_s_masks)
		return false;
	// Checks if stencil_actions is same
	if (memcmp(m_stencil_actions, temp->stencil_actions, 3 * sizeof(enums::Test_Action)))
		return false;
	// Checks if vertex size is same
	if (m_vertex_size != temp->vertex_size)
		return false;
	// Checks if vertex info is same
	if (m_vertex_info != *temp->vertex_info)
		return false;

	// Everything is same returns true
	return true;
}