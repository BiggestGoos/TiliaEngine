/*****************************************************************//**
 * @file   Renderer.cpp
 * @brief  The source file of the header @include @Renderer.h.
 * 
 * Standard:
 * @include <map>
 * 
 * Headers:
 * @include "headers/Renderer.h"
 * 
 * @author Gustav Fagerlind
 * @date   29/05/2022
 *********************************************************************/

// Standard
#include <map>

// Headers
#include "headers/Rendering/OpenGL/3.3/Renderer.h"

#include <iostream>

void tilia::render::Renderer::Render()
{

	std::map<float, std::vector<std::weak_ptr<Mesh_Data>>> transparent_meshes{};

	const size_t mesh_count{ m_mesh_data.size() };

	for (size_t i = 0; i < mesh_count; i++)
	{
		if (*m_mesh_data[i].lock()->transparent)
		{

			float total_length{};
			const size_t vertex_count{ m_mesh_data[i].lock()->vertex_data->size() };
			const size_t vertex_size{ m_mesh_data[i].lock()->vertex_size };

			const size_t start{ *m_mesh_data[i].lock()->vertex_pos_start };
			const size_t end{ *m_mesh_data[i].lock()->vertex_pos_end };
			for (size_t j = 0; j < vertex_count; j += vertex_size)
			{

				for (size_t k = start; k <= end; k++)
				{
					total_length += fabs((*m_mesh_data[i].lock()->vertex_data)[j + k] - m_camera_pos[static_cast<glm::vec3::length_type>(k - start)]);
				}

			}

			float average_length{ total_length / vertex_count };

			transparent_meshes[average_length].push_back(m_mesh_data[i]);

			continue;
		}

		const size_t batch_count{ m_batches.size() };
		for (size_t j = 0; j < batch_count; j++)
		{
			if (!m_batches[j]->Get_Mesh_Count()) {
				m_batches[j]->Reset(m_mesh_data[i]);
				m_batches[j]->Push_Mesh(m_mesh_data[i]);
				goto cont_o_loop;
			}
			if (m_batches[j]->Push_Mesh(m_mesh_data[i])) {
				goto cont_o_loop;
			}
		}

		m_batches.push_back(std::make_unique<Batch>(m_mesh_data[i]));

		m_batches[m_batches.size() - 1]->Push_Mesh(m_mesh_data[i]);

	cont_o_loop:
		continue;
	}

	for (auto it = transparent_meshes.rbegin(); it != transparent_meshes.rend(); it++) {
		const size_t count{ it->second.size() };
		for (int32_t j = static_cast<int32_t>(count - 1); j > -1; j--)
		{

			const size_t batch_count{ m_batches.size() };
			for (size_t k = 0; k < batch_count; k++)
			{
				if (!m_batches[k]->Get_Mesh_Count()) {
					m_batches[k]->Reset(it->second[j]);
					m_batches[k]->Push_Mesh(it->second[j]);
					goto cont_t_loop;
				}
				if (m_batches[k]->Push_Mesh(it->second[j])) {
					goto cont_t_loop;
				}
			}

			m_batches.push_back(std::make_unique<Batch>(it->second[j]));

			m_batches[m_batches.size() - 1]->Push_Mesh(it->second[j]);

		cont_t_loop:
			continue;

		}
	}

	std::cout << "Batch count: " << m_batches.size() << '\n';
	const size_t batch_count{ m_batches.size() };
	for (size_t i = 0; i < batch_count; i++)
	{
		std::cout << "Mesh count: " << m_batches[i]->Get_Mesh_Count() 
			<< " <<>> Vertex count: " << m_batches[i]->Get_Vertex_Count() / m_batches[i]->Get_Vertex_Size()
			<< " <<>> Index count: " << m_batches[i]->Get_Index_Count() << '\n';
		m_batches[i]->m_camera_pos = m_camera_pos;
		m_batches[i]->Render();
		m_batches[i]->Clear();
	}

}
