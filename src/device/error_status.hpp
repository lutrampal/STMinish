
/*******************************************************************************
 * [...]
 ******************************************************************************/

#ifndef _STMINNISH_DEVICE_ERROR_STATUS_HPP
#define _STMINNISH_DEVICE_ERROR_STATUS_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include "error_code.hpp"

namespace stminish
{
namespace device
{
/*******************************************************************************
 * CLASS DEFINITION
 ******************************************************************************/

class ErrorStatus
{
  public:
    ErrorStatus(ErrorCode code = ErrorCode::Success);

    ErrorCode get_code() const;

    /* @brief boolean conversion operator.
     * @details
     * Returns true if error code is not equal 0 i.e. any error will return
     * true, success value will return false. */
    operator bool() const;

    /** @brief Same as !(static_cast<bool>(*this)). */
    bool operator!() const;

  private:
    ErrorCode code;
};


}  // namespace device
}  // namespace stminish

#endif
