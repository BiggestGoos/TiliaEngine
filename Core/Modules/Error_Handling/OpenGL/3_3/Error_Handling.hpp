/*****************************************************************//**
 * @file   Error_Handling.hpp
 * @brief  A header containing the declarations of functions used for handling errors as 
 *         well as declarations and definitions of macros for handling errors.
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

#ifndef TILIA_OPENGL_3_3_ERROR_HANDLING_HPP
#define TILIA_OPENGL_3_3_ERROR_HANDLING_HPP

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

#define GL_CALL(x) 

namespace tilia {

    namespace utils {

        void Handle_GL_Error(const char* message, const size_t& line, const char* file, 
            const char* function);

        bool GL_Check_Error();

        void GL_Clear_Error();

        

    }

}

#endif // TILIA_OPENGL_3_3_ERROR_HANDLING_HPP