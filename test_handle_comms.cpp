#include "tests/fff.h"
#include "handle_comms.h"
#include <cassert>
#include <iostream>


DEFINE_FFF_GLOBALS;

// Define fake functions for fff.h
FAKE_VALUE_FUNC(int, read8, int);
FAKE_VOID_FUNC(modeSwitch, std::vector<int>);
FAKE_VOID_FUNC(fileTransfer, std::vector<int>);
FAKE_VALUE_FUNC(int, cerr, const char*);


void test_construction_and_callFunction(){
    // Tests the constructor and calling the function.

    // Create an instance of the Comms class
    Comms commsInstance;

    // Define function mappings
    commsInstance.setModeSwitchKey(12);
    commsInstance.setFileTransferKey(3);
    // commsInstance.methodMap[1] = std::make_pair(12, &Comms::modeSwitch);
    // commsInstance.methodMap[2] = std::make_pair(3, &Comms::fileTransfer);

    // Simulate receiving a function ID and arguments
    uint8_t function_id = 1;  // Choose the function ID you want to test
    
    /**int numArgs = commsInstance.methodMap[function_id].first;
    std::vector<int> args(numArgs);

    // Assign test values to arguments
    for (int i = 0; i < numArgs; i++) {
        args[i] = Comms::read8(i + 1);  // Assigning values 1, 2, 3, ... to the arguments
    }**/

    // Call the function
    commsInstance.callFunction();
    printf("done test_construction_and_callFunction");
    return;
}
    
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
    std::vector<int> args(2); //Example arguments
    args.push_back(5);
    args.push_back(6);

    // Call the function
    Comms c();

    c.callFunction();

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
    std::vector<int> args(3); //Example arguments
    args.push_back(1);
    args.push_back(2);
    args.push_back(3);

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

void test_callFunction_existingFunction()
{
    std::cout << "test_callFunction_existingFunction: Testing callFunction with an existing function" << std::endl;

    RESET_FAKE(read8);
    RESET_FAKE(modeSwitch);

    read8_fake.return_val = 1;

    // Simulate receiving a function ID for modeSwitch
    uint8_t function_id = 0;  // Function ID for modeSwitch

    // Create an instance of the Comms class
    Comms commsInstance;

    // Define function mappings
    //Comms::methodMap[function_id] = std::make_pair(2, &modeSwitch);
    commsInstance.setModeSwitchKey(2);

    // Call the function
    Comms::callFunction();

    // Assert that read8 was called to get the function ID
    assert(read8_fake.call_count == 1);

    // Assert that modeSwitch was called
    assert(modeSwitch_fake.call_count == 1);
}

void test_callFunction_nonExistingFunction()
{
    std::cout << "test_callFunction_nonExistingFunction: Testing callFunction with a non-existing function" << std::endl;

    RESET_FAKE(read8);
    RESET_FAKE(cerr);

    read8_fake.return_val = 2;  // Non-existing function ID

    // Call the function
    Comms::callFunction();

    // Assert that read8 was called to get the function ID
    assert(read8_fake.call_count == 1);

    // Assert that cerr was called to report an error
    assert(cerr_fake.call_count == 1);
}


int main()
{
    test_construction_and_callFunction();
    // test_modeSwitch();
    // test_fileTransfer();
    // test_callFunction_existingFunction();
    // test_callFunction_nonExistingFunction();

    // Add more tests as needed

    return 0;
}







