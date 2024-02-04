#include "tests/fff.h"
#include "handle_comms.h"
#include <cassert>
#include <iostream>


DEFINE_FFF_GLOBALS;

// Define fake functions for FFF.h
FAKE_VOID_FUNC(read8, int);
FAKE_VOID_FUNC(modeSwitch, std::vector<int>);
FAKE_VOID_FUNC(fileTransfer, std::vector<int>);

void test_modeSwitch()
{
    std::cout << "test_modeSwitch: Testing modeSwitch function" << std::endl;

    //reset fake function call history
    RESET_FAKE(read8);
    RESET_FAKE(modeSwitch);

    //fake return value for read8
    read8_fake.return_val = 1;

    // Simulate receiving a function ID and arguments for modeSwitch
    uint8_t function_id = 0;  // Function ID for modeSwitch
    std::vector<int> args = {5, 6};

    // Call the function
    Comms::callFunction();

    // Assert that read8 was called to get the function ID
    assert(read8_fake.call_count == 1);

    // Assert that modeSwitch was called with the correct arguments
    assert(modeSwitch_fake.call_count == 1);
    assert(modeSwitch_fake.arg0_val[0] == 5);
    assert(modeSwitch_fake.arg0_val[1] == 6);
}

void test_fileTransfer()
{
    std::cout << "test_fileTransfer: Testing fileTransfer function" << std::endl;

    RESET_FAKE(read8);
    RESET_FAKE(fileTransfer);

    read8_fake.return_val = 1;

    // Simulate receiving a function ID and arguments for fileTransfer
    uint8_t function_id = 1;  // Function ID for fileTransfer
    std::vector<int> args = {1, 2, 3};

    // Call the function
    Comms::callFunction();

    // Assert that read8 was called to get the function ID
    assert(read8_fake.call_count == 1);

    // Assert that fileTransfer was called with the correct arguments
    assert(fileTransfer_fake.call_count == 1);
    assert(fileTransfer_fake.arg0_val[0] == 1);
    assert(fileTransfer_fake.arg0_val[1] == 2);
    assert(fileTransfer_fake.arg0_val[2] == 3);
}

int main()
{
    test_modeSwitch();
    test_fileTransfer();

    // Add more tests as needed

    return 0;
}


