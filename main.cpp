#include "HandleComms.h"

int main()
{
    // Maps function_id (uint8_t) to function pointer
    HandleComms::methodMap[1] = std::make_pair(0, &HandleComms::mode_switch);
    HandleComms::methodMap[2] = std::make_pair(0, &HandleComms::file_transfer);

    HandleComms::callFunction();
}
