/*****************************************************************//**
 * @file   Renderer.h
 * @brief  Not finished. Going to be a class which takes meshes and draws them to a given window 
 *		   in a given order using a given shader.
 * 
 * @define TILIA_RENDERER_H
 * 
 * Standard:
 * @include <vector>
 * @include <memory>
 * 
 * Headers:
 * @include "headers/Mesh.h"
 * @include "headers/Batch.h"
 * 
 * @author Gustav Fagerlind
 * @date   29/05/2022
 *********************************************************************/

#ifndef TILIA_RENDERER_H
#define TILIA_RENDERER_H

// Standard
#include <vector>
#include <memory>

// Headers
#include "Core/Modules/Rendering/OpenGL/3.3/Mesh.h"
#include "Core/Modules/Rendering/OpenGL/3.3/Batch.h"

namespace tilia {

	namespace render {

		class Renderer {
		public:

			inline void Add_Mesh(std::weak_ptr<Mesh_Data> mesh_data) { m_mesh_data.push_back(mesh_data); };

			inline void Remove_Mesh(Mesh_Data* mesh_data) {
				size_t size{ m_mesh_data.size() };
				size_t index{};
				for (size_t i = 0; i < size; i++)
				{
					if (m_mesh_data[i].lock().get() == mesh_data)
						index = i;
				}
				m_mesh_data.erase(m_mesh_data.begin() + index);
			}

			void Render();

			// Todo: Placeholder
			glm::vec3 m_camera_pos{};

		private:

			std::vector<std::weak_ptr<Mesh_Data>> m_mesh_data{};

			std::vector<std::unique_ptr<Batch>> m_batches{};

		};

	}

}

#endif