module Components {
    @ Temperature Sensor Checking Component
    active component TempSensor {

        @ Command to read the Temperature
        async command READ_TEMPERATURE(
        )

        @ Reports the action of reading the temperature
        event StartTemperatureRead() \
            severity activity high \
            format "Started temperature read"

        @ Reports the temperature 
        event EndTemperatureRead(Temperature: F32) \
            severity activity high \
            format "Finished temperature read {} Celsius"

        @ Telemetry channel to report temperature.
        telemetry TemperatureValue: F32

        @ Telemetry channel to report number of temperature checks.
        telemetry TemperatureReadings: U32

        # @ Port receiving calls from the rate group
        # async input port run: Svc.Sched

        @ Port sending calls to the i2c driver
        sync input port i2cDriver: Drv.I2cWriteRead

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}