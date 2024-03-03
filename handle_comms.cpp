#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <vector>
#include <random>
#include <algorithm>
#include "handle_comms.hpp"

using random_bytes_engine = std::independent_bits_engine<std::default_random_engine, 1, unsigned char>;

// BEFORE YOU MAKE CHANGES:   git fetch
//                            git pull

// When you've made changes:  git add <filename>
//                            git commit -m "your message here"
//                            git push

Comms::Comms(){
    // set the ID's for the "task" functions
    this->taskMap[0] = std::make_pair(0, &Comms::modeSwitch);
    this->taskMap[1] = std::make_pair(1, &Comms::fileTransfer);

    // set the ID's for the "mode" functions
    this->modeMap[0] = std::make_pair(0, &Comms::defaultMode);
    this->modeMap[1] = std::make_pair(1, &Comms::mode1);
}

Comms::~Comms(){}

void Comms::handleData(){
    // reads a byte, based on that byte, it either calls modeSwitch or fileTransfer
    uint8_t taskID = read8(2);

    // get the arguments from Comms
    std::vector<int> args;
    uint8_t nextByte = 0;
    
    while(nextByte != -1){          // I assume when they're done sending info they'll send a -1
        nextByte = getRandByte();
        args.push_back(nextByte);
        nextByte = -1;              // setting this manually for now
    }

    // Calls a task - either switching methods, or transfering files
    callFunction(true, taskID); //TODO: not sure how to pass in the arguments

}

// method key setters
void Comms::setModeSwitchKey(int key){
    this->taskMap[0] = std::make_pair(key, &Comms::modeSwitch);
    return;
}

void Comms::setFileTransferKey(int key){
    this->taskMap[1] = std::make_pair(key, &Comms::fileTransfer);
    return;
}


// helper functions
uint8_t Comms::read8(int reg) {
    return 1 + reg;
}

uint8_t getRandByte() {
    // This function is a placeholder for reading data, will actually be comms data from i2c
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the distribution for uint8_t values (0 to 255)
    std::uniform_int_distribution<uint16_t> distribution(0, 255);

    // Generate a random uint8_t value
    uint8_t random_uint8 = static_cast<uint8_t>(distribution(gen));

    return random_uint8;
}

//TODO: need to be able to pass in arguments, make sure that works
void Comms::callFunction(bool task, uint8_t function_id) {
    /*Function is responsible for looking up a function pointer in taskMap based on a given function ID obtained from read8(0). If it finds a matching function, it calls it. Otherwise, it reports an error.

    Args:
        task (bool): task = 1 means we need to search in the taskMap, task = 0 means we need to search in the modeMap
    
    */
    
    //To simplify:
    //taskMap.count(function_id); // 0 if not there, 1 if there
    //taskMap[function_id].first

    if(task){
        auto it = taskMap.find(function_id);
        if(it != taskMap.end()){
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
            

        }else{
            // If result is error, print to cerr
            std::cerr << "Can't find function ID in map" << std::endl;
        }
    }else{
       auto it = modeMap.find(function_id); 
       if(it != modeMap.end()){
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
            

        }else{
            // If result is error, print to cerr
            std::cerr << "Can't find function ID in map" << std::endl;
        }
    }
         
}


//commands available:
void Comms::modeSwitch(std::vector<int> args){
    int modeID = args[0];
    int modeArgs = args[1];
    std::cout << "Switching to mode with ID: " << modeID << ", and params" << modeArgs << std::endl;
    
    callFunction(false, modeID);    // false because we are now calling modes, not tasks
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