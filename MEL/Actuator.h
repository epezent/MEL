#pragma once
#include "Device.h"
#include "Daq.h"

namespace mel {

    class Actuator : public Device {        

    public:

        enum class EnableMode { None, Low, High };

        //---------------------------------------------------------------------
        // CONSTRUCTOR(S) / DESTRUCTOR(S)
        //---------------------------------------------------------------------

        Actuator();
        Actuator(std::string name, EnableMode enable_mode);

        //---------------------------------------------------------------------
        // PUBLIC FUNCTIONS
        //---------------------------------------------------------------------

        /// This function should set the desired torque to be generated at the Actuator
        virtual void set_torque(double actuator_torque) = 0;

    protected:

        //---------------------------------------------------------------------
        // PROTECTED VARIABLES
        //---------------------------------------------------------------------

        EnableMode enable_mode_; ///< the enable mode of the actuator (None, Low, or High)

        //---------------------------------------------------------------------
        // STATE VARIABLES
        //---------------------------------------------------------------------

        double torque_; ///< stores the Actuator torque since the last call to set_torque()

    };
}