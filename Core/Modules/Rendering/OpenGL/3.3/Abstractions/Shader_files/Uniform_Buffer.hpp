/**************************************************************************************************
 * @file   Uniform_Buffer.hpp
 * 
 * @brief  The Uniform Buffer is an abstraction for an openGL ubo which automatically calculates 
 * 		   things like offsets and sizes for given variables which are represented by the GLSL 
 *         Variable class.
 * 
 * @author Gustav Fagerlind
 * @date   11/08/2022
 *************************************************************************************************/

// Vendor
#include "vendor/glm/include/glm/glm.hpp"
#include "vendor//glm//include/glm/gtc/type_ptr.hpp"

// Standard
#include <cstdint>
#include <initializer_list>
#include <utility>
#include <string>
#include <unordered_map>
#include <array>
#include <vector>
#include <memory>

// Tilia
#include "Core/Values/OpenGL/3.3/Enums.hpp"

#ifndef TILIA_UNIFORM_BUFFER_HPP
#define TILIA_UNIFORM_BUFFER_HPP

namespace tilia
{
    
    namespace gfx
    {   
        
		/**
		 * @brief Abstraction of an openGL ubo. Can be given a list of variables of different types and can then calculate all of the offsets, sizes, and strides for a GLSL std140 uniform block. Can then set the data for the variables in this block using uniform functions.
		 */
        class Uniform_Buffer
        {
		private:

			/**
			 * @brief Representation of a variable in a GLSL uniform block.
			 */
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

				// The type of scalar which the variable represents.
				enums::GLSL_Scalar_Type m_scalar_type{};
				// The type of container which the scalar is contained within.
				enums::GLSL_Container_Type m_container_type{};

				// If variable is array then array count is equal to amount of elements in array. Otherwise equal to zero.
				std::size_t m_array_count{};

			};

			// Alias for byte assuming char is 1 byte.
			using Byte = char;

        public:

			Uniform_Buffer() = default;

			/**
			 * @brief Copy constructor which also takes in a binding point. Copies variable data and generates openGL resources. If binding point is given then binds to that point.
			 * 
			 * @param other - The other uniform buffer.
			 * @param bind_point - The bind point where we want to bind to.
			 */
			Uniform_Buffer(const Uniform_Buffer& other, const std::int32_t& bind_point = -1);

			/**
			 * @brief Move constructor which also takes in a binding point. Moves variable data and openGL resources to this. If binding point is given then binds to that point.
			 * 
			 * @param other - The other uniform buffer.
			 * @param bind_point - The bind point where we want to bind to.
			 */
			Uniform_Buffer(Uniform_Buffer&& other, const std::int32_t& bind_point = -1) noexcept;

			/**
			 * @brief Copy assignment operator. Copies variable data.
			 * 
			 * @param other - The other uniform buffer.
			 * 
			 * @return A reference to this.
			 */
			Uniform_Buffer& operator=(const Uniform_Buffer& other);

			/**
			 * @brief Move assignment operator. Moves variable data and openGL resources to this. If this previously owned any openGL resources then those are deleted.
			 * 
			 * @param other - The other uniform buffer.
			 * 
			 * @return A reference to this.
			 */
			Uniform_Buffer& operator=(Uniform_Buffer&& other) noexcept;

			/**
			 * @brief Initialization function. Should be called first and olny once.
			 * 		  Stores given variables which can then be accessed in the uniform 
			 * 		  block this ubo is attached to.
			 * 
			 * @param block_variables - The variables to add.
			 * @param indexing - Whether or not to also add indexing variables.
			 * @param bind_point - The binding point to bind to.
			 */
            void Init(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables = {}, const bool& indexing = false, const std::int32_t& bind_point = -1);

			/**
			 * @brief Terminates the uniform buffer by deleting the openGL ubo. If the uniform buffer was never initialized then this will throw an exception.
			 */
			void Terminate();

			/**
			 * @brief Stores given variables which can then be accessed in the uniform 
			 * 		  block this ubo is attached to.
			 * 
			 * @param block_variables - The variables to add.
			 * @param indexing - Whether or not to also add indexing variables.
			 */
			void Reset(std::initializer_list<std::pair<std::string, GLSL_Variable>> block_variables, const bool& indexing = false);

			/**
			 * @brief Stores given variables which can then be accessed in the uniform
			 * 		  block this ubo is attached to.
			 *
			 * @param block_variables - The variables to add.
			 * @param indexing - Whether or not to also add indexing variables.
			 */
			void Reset(std::vector<std::pair<std::string, GLSL_Variable>> block_variables, const bool& indexing = false);

			/**
			 * @brief Clears the stored variables.
			 */
			void Clear();

            void debug_print();
            
			/**
			 * @brief Gets the id of the uniform buffer.
			 * 
			 * @return The id.
			 */
            inline auto Get_ID() const { return m_ID; }

			/**
			 * @brief Sets the binding point of the uniform buffer. Overwrites the previous uniform buffer which was bound to this bind point. (As far as I have found).
			 * 
			 * @param bind_point - The binding point.
			 */
            void Set_Bind_Point(const std::uint32_t& bind_point);

			/**
			 * @brief Gets the latest bound to binding point.
			 * 
			 * @return The binding point.
			 */
            inline auto Get_Bind_Point() const { return m_bind_point; }
			
			/**
			 * @brief The size of the uniform block in bytes.
			 * 
			 * @return The block size.
			 */
			inline auto Get_Block_Size() const { return m_block_size; }

			/**
			 * @brief Binds the uniform buffer.
			 */
			void Bind() const;

			/**
			 * @brief Binds the given uniform buffer.
			 *
			 * @param id - The id of the uniform buffer which will be bound.
			 */
			static void Bind(const std::uint32_t& id);

			/**
			 * @brief Unbinds the bound uniform buffer. Static version
			 * of Unbind.
			 */
			static void Unbind(const bool& save_id = false);

			/**
			 * @brief Binds the stored previously bound uniform buffer. Static version
			 * of Rebind.
			 */
			static void Rebind();

			#ifndef TILIA_UNIFORM_BUFFER_UNIFORMS
			#define TILIA_UNIFORM_BUFFER_UNIFORMS

			/**
			 * @brief Sets the data of the given name with the given data in a uniform block.
			 */
			#define TILIA_ENABLE_IF_UNIFORM std::enable_if_t<std::is_same<float, T>::value || std::is_same<std::int32_t, T>::value || std::is_same<std::uint32_t, T>::value || std::is_same<bool, T>::value>* = nullptr

			template<typename T, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::vector<T> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(T), &v.front(), delay);
			}

			template<typename T, std::size_t S, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::array<T, S> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(T), &v.front(), delay);
			}

			template<typename T, TILIA_ENABLE_IF_UNIFORM>				
			void Uniform(const std::string& loc, std::initializer_list<T> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(T), v.begin(), delay);
			}

			template<typename T, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, T v, const bool& delay = false)
			{
				Uniform(loc, sizeof(T), &v, delay);
			}

			template<typename T, glm::length_t size, glm::qualifier Q, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::vector<glm::vec<size, T, Q>> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(T) * size, &v.front(), delay);
			}

			template<typename T, std::size_t S, glm::length_t size, glm::qualifier Q, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::array<glm::vec<size, T, Q>, S> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(T) * size, &v.front(), delay);
			}

			template<typename T, glm::length_t size, glm::qualifier Q, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, std::initializer_list<glm::vec<size, T, Q>> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(T) * size, v.begin(), delay);
			}

			template<typename T, glm::length_t size, glm::qualifier Q, TILIA_ENABLE_IF_UNIFORM>
			void Uniform(const std::string& loc, glm::vec<size, T, Q> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(T) * size, glm::value_ptr(v), delay);
			}

			template<glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, std::vector<glm::mat<size_x, size_y, float, Q>> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(float) * size_y, &v.front(), delay);
			}

			template<std::size_t S, glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, std::array<glm::mat<size_x, size_y, float, Q>, S> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(float) * size_y, &v.front(), delay);
			}

			template<glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, std::initializer_list<glm::mat<size_x, size_y, float, Q>> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(float) * size_y, v.begin(), delay);
			}

			template<glm::length_t size_x, glm::length_t size_y, glm::qualifier Q>
			void Uniform(const std::string& loc, glm::mat<size_x, size_y, float, Q> v, const bool& delay = false)
			{
				Uniform(loc, sizeof(float) * size_y, glm::value_ptr(v), delay);
			}

			/**
			 * @brief Sets the data in the uniform block between the offset and the offset + the size with the given data.
			 * 
			 * @param offset - The offset to the data to be set.
			 * @param size - The size of the chunk of data to set.
			 * @param vs - A pointer to the start of the data.
			 * @param delay - Wheter or not to delay the upload of the uniform data.
			 * @param buffer - If a separate buffer is sent then the data is written to it. If buffer is nullptr then the data is written to buffer held by this.
			 */
			void Uniform(const std::size_t& offset, const std::size_t& size, const void* vs, const bool& delay = false, Byte* buffer = nullptr);

			/**
			 * @brief Uploads the locally stored data to the uniform block.
			 */
			void Map_Data();

			#undef TILIA_ENABLE_IF_UNIFORM
			#endif // TILIA_UNIFORM_BUFFER_UNIFORMS
			
        private:

			/**
			 * @brief Allocates data both on the gram and ram I.e it allocates data in a locally stored dynamic array and at the same time buffers data with openGL.
			 * The given size is also stored.
			 * 
			 * @param block_size - The amount of data in bytes to be allocated and stored.
			 */
			void Allocate_Data(const std::size_t& block_size);

			/**
			 * @brief Sets the data for a given uniform name in the bound uniform block.
			 * 
			 * @param loc - The name of the uniform to set.
			 * @param var_size - The size of the data. If uniform is matrix or array then var size is equal to the size of each element.
			 * @param vs - A pointer to the start of the data.
			 * @param delay - Wheter or not to delay the upload of the uniform data.
			 */
			void Uniform(const std::string& loc, const std::size_t& var_size, const void* vs, const bool& delay = false);

			/**
			 * @brief Adds a variable to the Uniform Buffer which can then be set using the uniform functions. The stored variable will have the starting offset of the given block size and return the new block size after adding the variable. If the given variable is not a scalar then the function will be recursively called to add all the indexing ways as seperate variables until the given starting variable is resolved.
			 * 
			 * @param block_size - The offset where the added variable will start.
			 * @param name - The name of the variable.
			 * @param variable - The variable to add.
			 * @param indexing - Whether or not to also add indexing variables.
			 * 
			 * @return The new block size.
			 */
			std::size_t Push_Variable(std::size_t block_size, const std::string& name, const GLSL_Variable& variable, const bool& indexing = false);

			// The openGL id to the underlying ubo.
            std::uint32_t m_ID{};

			// The binding point where the ubo is bound to. When calling the uniform functions you set data in the block where the ubo is bound. Set to -1 as default since bind points start at 0.
            std::int32_t m_bind_point{ -1 };

			// The stored variables and their offsets in the block the ubo is bound.
            std::unordered_map<std::string, std::pair<std::size_t, GLSL_Variable>> m_variables{};

			// Stored local data which can be added to and then mapped to the ubo data.
			std::unique_ptr<Byte[]> m_block_data{};

			// The total size in bytes that the variables take up in the uniform block.
			std::size_t m_block_size{};

			// The currently bound ubo.
            static std::uint32_t s_bound_ID;
			// The direct previously bound ubo.
			static std::uint32_t s_previous_ID;

        }; // Uniform_Buffer

    } // gfx
    
} // tilia

#endif // TILIA_UNIFORM_BUFFER_HPP