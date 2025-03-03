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
    U8 writeData[1];
    U8 readData[2];

    Fw::Buffer writeBuffer(writeData, sizeof(writeData));
    Fw::Buffer readBuffer(readData, sizeof(readData));

    const U8 regPointer = MCP9808_REG_AMBIENT_TEMP;
    std::memcpy(writeBuffer.getData(), &regPointer, 1);

    Drv::I2cStatus status = this->i2cDriver_handler(0, MCP9808_I2CADDR_DEFAULT, writeBuffer, readBuffer);

    switch (status) {
        case Drv::I2cStatus::I2C_OK: {
            U8* dataPtr = readBuffer.getData();
            const I16 rawTemp = (static_cast<I16>(dataPtr[0]) << 8) | dataPtr[1];

            // Mask & sign-extend if needed for MCP9808
            // (typical usage is to keep only bits 0..12)
            I16 maskedTemp = rawTemp & 0x1FFF;  // keep 13 bits
            if (maskedTemp & 0x1000) {
                maskedTemp -= 8192;  // sign-extend negative
            }
            const F32 temperatureC = static_cast<F32>(maskedTemp) / 16.0f;

            // Log, output ports, command response, etc.
            this->m_temp_reads += 1;
            this->log_ACTIVITY_HI_ReadTemperatureState();
            this->tlmWrite_TemperatureValue(temperatureC);
            this->log_ACTIVITY_LO_TemperatureRead(temperatureC);
            this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
            break;
        }
        case Drv::I2cStatus::I2C_ADDRESS_ERR:
            // this->log_WARNING_HI_I2cAddressError();
            this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
            break;
        case Drv::I2cStatus::I2C_WRITE_ERR:
        case Drv::I2cStatus::I2C_READ_ERR:
            // this->log_WARNING_HI_I2cReadError();
            this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
            break;
        default:
            // this->log_WARNING_HI_I2cUnknownError();
            this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::EXECUTION_ERROR);
            break;
    }
}

}  // namespace Components
