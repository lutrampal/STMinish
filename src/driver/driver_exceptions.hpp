
/*******************************************************************************
 * [...]
 ******************************************************************************/

#ifndef _STMINNISH_DRIVER_DRIVER_EXCEPTIONS_HPP
#define _STMINNISH_DRIVER_DRIVER_EXCEPTIONS_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include <exception>

namespace stminish
{
namespace device
{
/*******************************************************************************
 * CLASS DEFINITIONS
 ******************************************************************************/

class StartAsyncOpFailure : std::exception
{
    const char* what()
    {
        return "Failed to start asynchronous operation";
    }
};

class CancelAsyncOpFailure : std::exception
{
    const char* what()
    {
        return "Failed to cancel asynchronous operation";
    }
};


}  // namespace device
}  // namespace stminish

#endif
