
/*******************************************************************************
 * [...]
 ******************************************************************************/

#ifndef _STMINNISH_EVENT_LOOP_HPP
#define _STMINNISH_EVENT_LOOP_HPP

/*******************************************************************************
 * INCLUDE DIRECTIVES
 ******************************************************************************/

#include <functional>
#include <list>


namespace stminish
{
/*******************************************************************************
 * CLASS DEFINITION
 ******************************************************************************/

class EventLoop
{
  public:
    void run();
    void pushEvent(std::function<void()>&& event_handler);

  private:
    std::list<std::function<void()>> event_queue;
};

}  // namespace stminish

#endif
