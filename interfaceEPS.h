#define interfaceEPS

extern "C"
{

}

//Stnadered liberies for I/O files to operate
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <fcntl.h>
#include <limits>
#include <bitset>

//Define I2C adresss for 


#ifndef interfaceEPS_h
#define interfaceEPS_h

class InterfaceEPS{
    private:

        //Slave address
        int sAddress;


    public:
        InterfaceEPS(int addr);
        /*
            setSAddress(int addr);
        */
        void setSAddress (int addr);
        /*
            sAddress = addr;
        */
        int getSAddress(){ return sAddress; }

        // Function for determining command
        int command(int someSAddress, int rwB);
        /*
            probably use get method to get slave address,
            store that into a variable and call command function
        */

        // Function to use if command is read (might not use, seems redundant)
        int readC(int someSAddress);

        // Function to use if command is write (might not use, seems redundant)
        int writeC(int someSAddress);

        // Each slave can be identified with a specific address
        // Will be used inside readC and writeC function
        // If address match, return 1
        int sId();

};


#endif /* interfaceEPS_h */