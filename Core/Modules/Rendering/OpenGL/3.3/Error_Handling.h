/*****************************************************************//**
 * @file   Error_Handling.h
 * @brief  A header containing the declarations of functions used for handling errors as 
 *         well as declarations and definitions of macros for handling errors.
 * 
 * @define TILIA_ERROR_HANDLING_H
 * 
 * @author Gustav Fagerlind
 * @date   15/05/2022
 *********************************************************************/

#ifndef TILIA_ERROR_HANDLING_H
#define TILIA_ERROR_HANDLING_H

#ifndef NDEBUG
/**
 * @brief Calls abort() if given value is false
 */
#define ASSERT(x) if(!(x)) abort();
#else
#define ASSERT
#endif

/**
 * @brief Wrap openGL functions. Clears openGL errors using GL_Clear_Error. 
 * If error is thrown from function, prints errors using GL_Log_Call, and 
 * calls abort() using ASSERT.
 * 
 * @param x - The openGL function
 */
#define GL_CALL(x) GL_Clear_Error();\
    x;\
    ASSERT(GL_Log_Call(#x, __FILE__, __LINE__))

/**
 * @brief Gets an error string pertaining to an error code.
 * 
 * @param error_code - The error code for which to get error string from
 * 
 * @return The error string pertaining to the error code.
 */
const char* Get_Error_String(const uint32_t & error_code);

/**
 * @brief Clears the openGL errors
 */
void GL_Clear_Error();

/**
 * @brief Checks if there was an error. If so prints the error string, 
 * function, file, and line of the openGL function and also returns false. 
 * Otherwise returns true.
 * 
 * @param function - The function which is checked for errors
 * @param file - The file containing the function
 * @param line - The line the function is on
 * 
 * @return True if no error, and false if there is.
 */
bool GL_Log_Call(const char* function, const char* file, int line);

#endif


