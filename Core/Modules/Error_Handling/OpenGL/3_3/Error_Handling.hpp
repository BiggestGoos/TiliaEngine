/**************************************************************************************************
 * @file   Error_Handling.hpp
 * 
 * @brief  A header containing the declarations of functions used for handling errors as 
 *         well as declarations and definitions of macros for handling errors.
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *************************************************************************************************/

#ifndef TILIA_OPENGL_3_3_ERROR_HANDLING_HPP
#define TILIA_OPENGL_3_3_ERROR_HANDLING_HPP

// Tilia
#include "Core/Values/Directories.hpp"
#include TILIA_TILIA_EXCEPTION_INCLUDE

#if !defined(NDEBUG) && defined(_DEBUG)
#include <cstdlib>
/**
 * @brief Calls std::exit() with code 1 if given expression is false
 */
#define ASSERT(x) if(!(x)) std::exit(1);
#else
#define ASSERT
#endif

#define GL_ASSERT_(x)          tilia::utils::GL_Clear_Error();\
                              x;\
                              ASSERT(tilia::utils::GL_Check_Error())

#define GL_CALL_(x)            tilia::utils::GL_Clear_Error();\
                              x;\
                              tilia::utils::Handle_GL_Error("", __LINE__, __FILE__, #x);

#define GL_CALL_MESSAGE_(x, y) tilia::utils::GL_Clear_Error();\
                              x;\
                              tilia::utils::Handle_GL_Error(y, __LINE__, __FILE__, #x);


/**
 * @brief Calls the given function and then if there are any errors it throws an exception with 
 * info about the error.
 */
#define GL_CALL(func)                  tilia::utils::Clear_openGL_Error();\
                                       func;\
                                       tilia::utils::Handle_openGL_Error(TILIA_LOCATION, #func);

 /**
  * @brief Calls the given function and then if there are any errors it throws an exception with 
  * the given message and info about the error.
  */
#define GL_CALL_MESSAGE(message, func) tilia::utils::Clear_openGL_Error();\
                                       func;\
                                       tilia::utils::Handle_openGL_Error(TILIA_LOCATION, #func, \
                                       message);

namespace tilia {

    namespace utils {

        void Handle_openGL_Error(const char* file, const size_t& line,
            const char* function);

        void Handle_openGL_Error(const char* file, const size_t& line,
            const char* function, Exception_Data& message);

        void Clear_openGL_Error();

    } // utils

} // tilia

#endif // TILIA_OPENGL_3_3_ERROR_HANDLING_HPP