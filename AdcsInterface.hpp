/*
 * AdcsInterface.hpp
 * 
 * Communication with ADCS subsystem.
 * 
 * Controls subsystem power state, orbit,
 * and point at locations on earth
 * 
 */

#ifndef OBC_ADCS_H
#define OBC_ADCS_H

class AdcsInterface {

    public:

        // Condensed form of a two line element containing
        // only the values used to calculate orbit.
        struct TwoLineElement {
            float meanMotion;
            float meanMotionDerivative; // Ballistic Coefficient
            float meanMotionSecondDerivative;
            float bStar;
            float inclination;
            float rightAscension;
            float eccentricity;
            float perigeeArgument;
            float meanAnomaly;
            float epochDay;
            int epochYear;
            int revolutionsAtEpoch;
        };

        // Send the boot command to the ADCS
        void EnableAdcs();
        
        // Point at a position on earth
        void PointAt(float latitude, float longitude);

        // Modify the satellite's orbit
        void SetOrbit(TwoLineElement tle);

    private:
        // ADCS I2C commands
        enum Command {
            BOOT = 1,
            INTERRUPT = 2,
            POINT = 3,
            TLE = 4,
            POWERMODE = 5
        };
        
        // Placeholder for sending serial data to ADCS
        void SendCommand(int command);
        void SendCommandWithData(int command, void* data, size_t length);

};

#endif // OBC_ADCS_H