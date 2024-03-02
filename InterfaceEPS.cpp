#include "interfaceEPS.h"
#include <iostream>
#include <iomanip>

using namespace std;
class InterfaceEPS{
    public:
/*
        Read/Write Command
        7-bit "slave" address followed by data bytes 
        Recall: Byte means 8 bit
        7-bit means...

        Binary: 
        Index:    6   5   4   3   2   1   0  R/W
                  0   0   0   0   0   0   0  1/0 
        So in Hex should be from 0x00 to 0x7F 
*/

/*
        Write Command
        7-bit address + data bytes
        Data bytes:
            - Command Byte
            - Data Parameter Byte (0x00 for no specifc params.)

            
*/
void i2cWrite(uint8_t deviceAddress, uint8_t data) {
    // implementation of the write operation here
    // This function should send the appropriate I2C start, device address, data, and stop signals
    // Replace the following print statement with the actual I2C communication code
    std::cout << "Write to device 0x" << std::hex << std::setw(2) << std::setfill('0') << +deviceAddress
              << " Data: 0x" << std::hex << std::setw(2) << std::setfill('0') << +data << std::endl;
}





/*
        Read Command
        7-bit address + data bytes
        Data bytes:
            - Data[0] (most significant byte)
            - Data[1] (least signficant byte)
        Data bytes should be read out together 
*/
/*
        Command Error: 0xFFFF
            If an error has generated from a command 
                - Ex. such as writing a non-existent address from 
                either read or write command)
            Return value will be 0xFFFF 
*/
        int sAddress;

        // Start byte (stop = 0)
        int start;

        // Acknowledge bit
        // Receiving device is required to acknowledge 
        int aB;

        // read/write bit (bit 1 = read, bit 0 = write)
        // Probably best to include that in the sAddress
        // rather than its own int var
        int rwB;

        // Command byte
        int cmd;

        // Data Parameter byte
        int dataP;

        // Data [1] and Data[0]

        // Function for determining command
        int command(int start, int sAddress, int rwb){
            /*
                if (start = 1){
                    Figure out how to actually send address
                    switch (rwb) {
                        case 0:
                            write function
                            break;
                        case 1:
                            read function
                            break;
                        default:
                            cout << "Error: Invalid rwb byte"
                    }
                }
            */
        }

        // Function to use if command is read (might not use, seems redundant)
        int readC(int sAddress);

        // Function to use if command is write (might not use, seems redundant)
        int writeC(int sAddress);

        // Each slave can be identified with a specific address
        // Will be used inside readC and writeC function
        // If address match, return 1
        int sId();

        /*
            Depending on the command read first two bytes represents
            information from the motherboard and furth two bytes are the
            daughterboard. Overall bytes will be returned, need to translate
            the definition for that information.
        */


        

};


int main(){
    
        /*
        
        1) Master sends start condition to every connected slave device
        Assuming this is required for read command since there is a stop bit

        2) Master sends each slave the 7-bit address of the slave it wants to 
           communicate with along with the read/write bit

        3) Each slave must compare its own address with the address sent by
           the master. If the address is a match, return acknowledge byte

        4) Master will send data to that slave device or read data from it

        5) Extra note for read command:
           A stop condition should be set to stop reading from the device

        */
    InterfaceEPS myEPS;
    
    
}

/* Side notes:
        What does this mean?
        Each command has a delay associated with it, this is required to
        allow the microcontroller time to process each request. During this delay, the correct response may
        not be returned, and commands sent during the period may be ignored.

        User prompt?

*/