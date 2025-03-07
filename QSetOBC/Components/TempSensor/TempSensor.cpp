// ======================================================================
// \title  TempSensor.cpp
// \author jax
// \brief  cpp file for TempSensor component implementation class
// ======================================================================

#include "Components/TempSensor/TempSensor.hpp"

namespace Components {

TempSensor::TempSensor(const char* const compName) : TempSensorComponentBase(compName), m_temp_reads(0) {}

TempSensor::~TempSensor() {}

void TempSensor::READ_TEMPERATURE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {
    return;
}

F32 TempSensor::readTemperature() {
    if (!this->isConnected_writeRead()) {
        this->log_WARNING_HI_SensorInitError(1);
        m_errorCount++;
        return -1000.0f;
    }

    // 1. Prepare Write Buffer (Register Address)
    U8 writeData[1] = {MCP9808_REG_TEMP};
    Fw::Buffer writeBuffer(writeData, sizeof(writeData));  // Create Fw::Buffer

    // 2. Prepare Read Buffer (Allocate Memory)
    U8 readData[2];                                     // Raw buffer to hold the *data* read from I2C
    Fw::Buffer readBuffer(readData, sizeof(readData));  // Create Fw::Buffer, manage readData

    Drv::I2cStatus status;

    // 3. Call the Port
    status = this->writeRead_out(0, m_deviceAddr, writeBuffer, readBuffer);

    if (status != Drv::I2cStatus::I2C_OK) {
        this->log_WARNING_HI_I2CReadError(status.e);
        m_errorCount++;
        this->tlmWrite_ErrorCount(m_errorCount);
        return -1000.0f;
    }

    // MCP9808 temperature conversion:
    U16 rawTemp = ((readData[0] & 0x1F) << 8) | readData[1];
    F32 temperature;

    if (readData[0] & 0x10) {
        rawTemp = 0x1000 - (rawTemp & 0x0FFF);
        temperature = -1.0f * rawTemp * 0.0625f;
    } else {
        temperature = rawTemp * 0.0625f;
    }

    return temperature;
}

bool TempSensor::isConnected_writeRead(void) {
    return this->isConnected_writeRead_OutputPort(0);
}

}  // namespace Components
