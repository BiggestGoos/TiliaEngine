/**************************************************************************************************
 * @file   Directories.hpp
 * 
 * @brief  
 * 
 * @author Gustav Fagerlind
 * @date   12/01/2023
 *************************************************************************************************/

#ifndef TILIA_DIRECTORIES_HPP
#define TILIA_DIRECTORIES_HPP

#define TILIA_LOGGING_INCLUDE "Core/Modules/Console/Logging.hpp"

#define TILIA_EXCEPTION_HANDLER_INCLUDE "Core/Modules/Exceptions/Exception_Handler.hpp"
#define TILIA_TILIA_EXCEPTION_INCLUDE "Core/Modules/Exceptions/Tilia_Exception.hpp"

#define TILIA_WINDOWS_FILE_SYSTEM_INCLUDE "Core/Modules/File_System/Windows/File_System.hpp"

#define TILIA_OPENGL_3_3_ERROR_HANDLING_INCLUDE "Core/Modules/Error_Handling/OpenGL/3_3/Error_Handling.hpp"

#define TILIA_OPENGL_3_3_SHADER_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Shader_Files/Shader.hpp"
#define TILIA_OPENGL_3_3_SHADER_DATA_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Shader_Files/Shader_Data.hpp"
#define TILIA_OPENGL_3_3_SHADER_PART_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Shader_Files/Shader_Part.hpp"
#define TILIA_OPENGL_3_3_UNIFORM_BUFFER_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Shader_Files/Uniform_Buffer.hpp"

#define TILIA_OPENGL_3_3_CUBE_MAP_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Texture_Files/Cube_Map.hpp"
#define TILIA_OPENGL_3_3_CUBE_MAP_DATA_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Texture_Files/Cube_Map_Data.hpp"
#define TILIA_OPENGL_3_3_TEXTURE__INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Texture_Files/Texture_.hpp"
#define TILIA_OPENGL_3_3_TEXTURE_2D__INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Texture_Files/Texture_2D_.hpp"

#define TILIA_OPENGL_3_3_BUFFER_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Buffer.hpp"

#define TILIA_OPENGL_3_3_BATCH_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Batch.hpp"
#define TILIA_OPENGL_3_3_MESH_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Mesh.hpp"
#define TILIA_OPENGL_3_3_RENDERER_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Renderer.hpp"
#define TILIA_OPENGL_3_3_VERTEX_INCLUDE "Core/Modules/Rendering/OpenGL/3_3/Vertex.hpp"

#define TILIA_TEXTURE_INCLUDE "Core/Modules/Rendering/OpenGL/Textures/Texture.hpp"
#define TILIA_TEXTURE_2D_INCLUDE "Core/Modules/Rendering/OpenGL/Textures/Texture_2D.hpp"

#define TILIA_FRAMEBUFFER_INCLUDE "Core/Modules/Rendering/OpenGL/Framebuffer.hpp"

#define TILIA_MONITOR_INCLUDE "Core/Modules/Windowing/Monitor.hpp"
#define TILIA_WINDOW_INCLUDE "Core/Modules/Windowing/Window.hpp"
#define TILIA_WINDOW_PROPERTIES_INCLUDE "Core/Modules/Windowing/Window_Properties.hpp"

#define TILIA_IMAGE_INCLUDE "Core/Modules/Images/Image.hpp"

#define TILIA_TEMP_CAMERA_INCLUDE "Core/Temp/Camera.hpp"
#define TILIA_TEMP_INPUT_INCLUDE "Core/Temp/Input.hpp"
#define TILIA_TEMP_LIMIT_FPS_INCLUDE "Core/Temp/Limit_Fps.hpp"
#define TILIA_TEMP_STOPWATCH_INCLUDE "Core/Temp/Stopwatch.hpp"

#define TILIA_OPENGL_3_3_CONSTANTS_INCLUDE "Core/Values/OpenGL/3_3/Constants.hpp"
#define TILIA_OPENGL_3_3_UTILS_INCLUDE "Core/Values/OpenGL/3_3/Utils.hpp"

#define TILIA_DIRECTORIES_INCLUDE "Core/Values/Directories.hpp"
#define TILIA_CONSTANTS_INCLUDE "Core/Values/Constants.hpp"

#endif // TILIA_DIRECTORIES_HPP

#if 0

#include "Core/Modules/Console/Logging.hpp"

#include "Core/Modules/Exceptions/Exception_Handler.hpp"
#include "Core/Modules/Exceptions/Tilia_Exception.hpp"

#include "Core/Modules/File_System/Windows/File_System.hpp"

#include "Core/Modules/Rendering/OpenGL/3_3/Error_Handling.hpp"

#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Shader_Files/Shader.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Shader_Files/Shader_Data.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Shader_Files/Shader_Part.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Shader_Files/Uniform_Buffer.hpp"

#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Texture_Files/Cube_Map.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Texture_Files/Cube_Map_Data.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Texture_Files/Texture_.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Texture_Files/Texture_2D_.hpp"

#include "Core/Modules/Rendering/OpenGL/3_3/Abstractions/Buffer.hpp"

#include "Core/Modules/Rendering/OpenGL/3_3/Batch.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Mesh.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Renderer.hpp"
#include "Core/Modules/Rendering/OpenGL/3_3/Vertex.hpp"

#include "Core/Modules/Rendering/Textures/Texture.hpp"
#include "Core/Modules/Rendering/Textures/Texture_2D.hpp"

#include "Core/Modules/Rendering/Framebuffer.hpp"

#include "Core/Modules/Windowing/Monitor.hpp"
#include "Core/Modules/Windowing/Window.hpp"
#include "Core/Modules/Windowing/Window_Properties.hpp"

#include "Core/Modules/Images/Image.hpp"

#include "Core/Temp/Camera.hpp"
#include "Core/Temp/Input.hpp"
#include "Core/Temp/Limit_Fps.hpp"
#include "Core/Temp/Stopwatch.hpp"

#include "Core/Values/OpenGL/3_3/Constants.hpp"
#include "Core/Values/OpenGL/3_3/Utils.hpp"

#include "Core/Values/Directories.hpp"
#include "Core/Values/Constants.hpp"


#endif // 0