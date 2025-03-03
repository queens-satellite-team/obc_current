// ======================================================================
// \title  TempSensor.cpp
// \author jax
// \brief  cpp file for TempSensor component implementation class
// ======================================================================

#include "Components/TempSensor/TempSensor.hpp"
#include "FpConfig.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  TempSensor ::
    TempSensor(const char* const compName) :
      TempSensorComponentBase(compName)
  {

  }

  TempSensor ::
    ~TempSensor()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void TempSensor ::
    TODO_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq
    )
  {
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
