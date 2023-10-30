
class InterfaceEPS{
    public:
/*
        Read/Write Command
        7-bit "slave" address followed by data bytes 
        Recall: Byte means 8 bit
        7-bit means...

        Binary: 
        Index:    6   5   4   3   2   1   0
                  0   0   0   0   0   0   0
        So in Hex should be from 0x00 to 0x7F 
*/
/*
        Write Command
        7-bit address + data bytes
        Data bytes:
            - Command Byte
            - Data Parameter Byte (0x00 for no specifc params.)
*/
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

        // Start byte
        int start;

        // Acknowledge bit
        // Receiving device is required to acknowledge 
        int aB;

        // read/write bit
        int rwB;

        // Function for determining command
        int command(int sAddress, int rwb);

        // Function to use if command is read (might not use, seems redundant)
        int readC(int sAddress);

        // Function to use if command is write (might not use, seems redundant)
        int writeC();

        // Each slave can be identified with a specific address
        // Will be used inside readC and writeC function
        // If address match, return 1
        int sId();

        /*
        
        1) Master sends start condition to every connected slave device

        2) Master sends each slave the 7-bit address of the slave it wants to 
           communicate with along with the read/write bit

        3) Each slave must compare its own address with the address sent by
           the master. If the address is a match, return acknowledge byte

        */

};


int main(){

}



// Side note: What does this mean?
//         Each command has a delay associated with it, this is required to
//         allow the microcontroller time to process each request. During this delay, the correct response may
//         not be returned, and commands sent during the period may be ignored.