
#include "device/error_status.hpp"
#include "device/system.hpp"
#include "event_loop.hpp"

#include <chrono>
#include <functional>
#include <memory>

using namespace stminish;
using namespace std;
using namespace std::placeholders;

int main(void)
{
    device::System& sys = device::System::getInstance();

    while (true) {}
}