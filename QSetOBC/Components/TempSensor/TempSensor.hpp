// ======================================================================
// \title  TempSensor.hpp
// \author jax
// \brief  hpp file for TempSensor component implementation class
// ======================================================================

#ifndef Components_TempSensor_HPP
#define Components_TempSensor_HPP

#include "Components/TempSensor/TempSensorComponentAc.hpp"

namespace Components {

  class TempSensor :
    public TempSensorComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct TempSensor object
      TempSensor(
          const char* const compName //!< The component name
      );

      //! Destroy TempSensor object
      ~TempSensor();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      //! Handler implementation for command TODO
      //!
      //! TODO
      void TODO_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq //!< The command sequence number
      ) override;

  };

}

#endif
