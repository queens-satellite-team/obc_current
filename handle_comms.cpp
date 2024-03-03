#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <vector>
#include "handle_comms.hpp"

// BEFORE YOU MAKE CHANGES:   git fetch
//                            git pull

// When you've made changes:  git add <filename>
//                            git commit -m "your message here"
//                            git push

Comms::Comms(){
    // set the ID's for the "task" functions
    this->methodMap[0] = std::make_pair(0, &Comms::modeSwitch);
    this->methodMap[1] = std::make_pair(1, &Comms::fileTransfer);

    // set the ID's for the "mode" functions
    this->modeMap[0] = std::make_pair(0, &Comms::defaultMode);
    this->modeMap[1] = std::make_pair(1, &Comms::mode1);
}

Comms::~Comms(){}

// method key setters
void Comms::setModeSwitchKey(int key){
    this->methodMap[0] = std::make_pair(key, &Comms::modeSwitch);
    return;
}

void Comms::setFileTransferKey(int key){
    this->methodMap[1] = std::make_pair(key, &Comms::fileTransfer);
    return;
}


// helper functions
uint8_t Comms::read8(int reg) {
    // This function is a placeholder for reading data, will actually be comms data from i2c
    return 1 + reg;
}

void Comms::callFunction() {
    // Function is responsible for looking up a function pointer in methodMap based on a given function ID obtained from read8(0). If it finds a matching function, it calls it. Otherwise, it reports an error.
    // Reads function_id
    uint8_t function_id = read8(0);
    std::cout << "function ID: " << function_id << "\n";
    
    //To simplify:
    //methodMap.count(function_id); // 0 if not there, 1 if there
    //methodMap[function_id].first



    // Searches methodMap
    auto it = methodMap.find(function_id);
    


    if (it != methodMap.end()) {
        // If result is not an error, then call function
        int numArgs = it->second.first;
        std::vector<int> args(numArgs);
 
        for(int i=0; i < numArgs; i++){
            //read additional arguments based on the functions argument count? maybe?
            args[i] = read8(1 + i);
        }

        // Print the detected number of arguments and their values
        std::cout << "Detected " << numArgs << " arguments: ";
        for (int i = 0; i < numArgs; i++) {
            std::cout << args[i] << " ";
        }
        std::cout << std::endl;

        it->second.second(args); // passes args array into the funtion? I think?
        

    } else {
        // If result is error, print to cerr
        std::cerr << "Can't find function ID in map" << std::endl;
    }
}


//commands available:
void Comms::modeSwitch(std::vector<int> args){
    //Example usage of this function: methodMap[1] = &modeSwitch; 
    //                                methodMap[2] = &fileTransfer;
    //                                callFunction();
    // Each command reads in its own data
    int param1 = args[0];
    int param2 = args[1];
    std::cout << "Switching Mode with arguments: " << param1 << ", " << param2 << std::endl;
}

void Comms::fileTransfer(std::vector<int> args){
    int param1 = args[0];
    int param2 = args[1];
    int param3 = args[2];
    std::cout << "Transfering File with arguments: " << param1 << ", " << param2 << ", " << param3 << std::endl;
}


//modes available:
void Comms::defaultMode(std::vector<int> args){
    // sets the satellite into default mode
}

void Comms::mode1(std::vector<int> args){
    // example mode
    // calls functions to communicate to the satellite to get into "mode 1"
}