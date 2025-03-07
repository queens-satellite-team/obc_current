// ======================================================================
// \title  TempSensor.hpp
// \author jax
// \brief  hpp file for TempSensor component implementation class
// ======================================================================

#ifndef Components_TempSensor_HPP
#define Components_TempSensor_HPP

#include <Drv/I2cDriverPorts/I2cWriteReadPortAc.hpp>
#include <cstring>
#include "Components/TempSensor/TempSensorComponentAc.hpp"
#include "FpConfig.hpp"
#include "Fw/Buffer/Buffer.hpp"
#include "Fw/Types/BasicTypes.hpp"

#define MCP9808_I2CADDR_DEFAULT 0x18   ///< I2C address
#define MCP9808_REG_AMBIENT_TEMP 0x05  ///< ambient temperature

namespace Components {

class TempSensor : public TempSensorComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct TempSensor object
    TempSensor(const char* const compName  //!< The component name
    );

    //! Destroy TempSensor object
    ~TempSensor();

    bool isConnected_writeRead(void);

    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command READ_TEMPERATURE
    //!
    //! Command to read the Temperature
    void READ_TEMPERATURE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) override;

  private:
    static const U8 MCP9808_REG_CONFIG = 0x01;
    static const U8 MCP9808_REG_TEMP = 0x05;
    static const U8 MCP9808_ADDR = 0x18;  // Default address, can be configured

    U8 m_deviceAddr;
    U32 m_errorCount;
    U64 m_temp_reads = 0;  //! The number of temperature reads that have occured
    F32 readTemperature();
    void initSensor();
};

}  // namespace Components

#endif
